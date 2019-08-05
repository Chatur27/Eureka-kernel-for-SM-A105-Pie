/*
 * pin-controller/pin-mux/pin-config/gpio-driver for Samsung's SoC's.
 *
 * Copyright (c) 2012 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 * Copyright (c) 2012 Linaro Ltd
 *		http://www.linaro.org
 *
 * Author: Thomas Abraham <thomas.ab@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This driver implements the Samsung pinctrl driver. It supports setting up of
 * pinmux and pinconf configurations. The gpiolib interface is also included.
 * External interrupt (gpio and wakeup) support are not included in this driver
 * but provides extensions to which platform specific implementation of the gpio
 * and wakeup interrupts can be hooked to.
 */

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/irqdomain.h>
#include <linux/spinlock.h>
#include <linux/syscore_ops.h>

#include "../core.h"
#include "pinctrl-samsung.h"
#ifdef CONFIG_SEC_GPIO_DVS
#include "secgpio_dvs.h"
#endif

#if defined(ENABLE_SENSORS_FPRINT_SECURE)
extern int fps_resume_set(void);
#endif

/* list of all possible config options supported */
static struct pin_config {
	const char *property;
	enum pincfg_type param;
} cfg_params[] = {
	{ "samsung,pin-pud", PINCFG_TYPE_PUD },
	{ "samsung,pin-drv", PINCFG_TYPE_DRV },
	{ "samsung,pin-con-pdn", PINCFG_TYPE_CON_PDN },
	{ "samsung,pin-pud-pdn", PINCFG_TYPE_PUD_PDN },
	{ "samsung,pin-val", PINCFG_TYPE_DAT },
};

/* Global list of devices (struct samsung_pinctrl_drv_data) */
static LIST_HEAD(drvdata_list);

static unsigned int pin_base;

static inline struct samsung_pin_bank *gc_to_pin_bank(struct gpio_chip *gc)
{
	return container_of(gc, struct samsung_pin_bank, gpio_chip);
}

static int samsung_get_group_count(struct pinctrl_dev *pctldev)
{
	struct samsung_pinctrl_drv_data *pmx = pinctrl_dev_get_drvdata(pctldev);

	return pmx->nr_groups;
}

static const char *samsung_get_group_name(struct pinctrl_dev *pctldev,
						unsigned group)
{
	struct samsung_pinctrl_drv_data *pmx = pinctrl_dev_get_drvdata(pctldev);

	return pmx->pin_groups[group].name;
}

static int samsung_get_group_pins(struct pinctrl_dev *pctldev,
					unsigned group,
					const unsigned **pins,
					unsigned *num_pins)
{
	struct samsung_pinctrl_drv_data *pmx = pinctrl_dev_get_drvdata(pctldev);

	*pins = pmx->pin_groups[group].pins;
	*num_pins = pmx->pin_groups[group].num_pins;

	return 0;
}

static int reserve_map(struct device *dev, struct pinctrl_map **map,
		       unsigned *reserved_maps, unsigned *num_maps,
		       unsigned reserve)
{
	unsigned old_num = *reserved_maps;
	unsigned new_num = *num_maps + reserve;
	struct pinctrl_map *new_map;

	if (old_num >= new_num)
		return 0;

	new_map = krealloc(*map, sizeof(*new_map) * new_num, GFP_KERNEL);
	if (!new_map) {
		dev_err(dev, "krealloc(map) failed\n");
		return -ENOMEM;
	}

	memset(new_map + old_num, 0, (new_num - old_num) * sizeof(*new_map));

	*map = new_map;
	*reserved_maps = new_num;

	return 0;
}

static int add_map_mux(struct pinctrl_map **map, unsigned *reserved_maps,
		       unsigned *num_maps, const char *group,
		       const char *function)
{
	if (WARN_ON(*num_maps == *reserved_maps))
		return -ENOSPC;

	(*map)[*num_maps].type = PIN_MAP_TYPE_MUX_GROUP;
	(*map)[*num_maps].data.mux.group = group;
	(*map)[*num_maps].data.mux.function = function;
	(*num_maps)++;

	return 0;
}

static int add_map_configs(struct device *dev, struct pinctrl_map **map,
			   unsigned *reserved_maps, unsigned *num_maps,
			   const char *group, unsigned long *configs,
			   unsigned num_configs)
{
	unsigned long *dup_configs;

	if (WARN_ON(*num_maps == *reserved_maps))
		return -ENOSPC;

	dup_configs = kmemdup(configs, num_configs * sizeof(*dup_configs),
			      GFP_KERNEL);
	if (!dup_configs) {
		dev_err(dev, "kmemdup(configs) failed\n");
		return -ENOMEM;
	}

	(*map)[*num_maps].type = PIN_MAP_TYPE_CONFIGS_GROUP;
	(*map)[*num_maps].data.configs.group_or_pin = group;
	(*map)[*num_maps].data.configs.configs = dup_configs;
	(*map)[*num_maps].data.configs.num_configs = num_configs;
	(*num_maps)++;

	return 0;
}

static int add_config(struct device *dev, unsigned long **configs,
		      unsigned *num_configs, unsigned long config)
{
	unsigned old_num = *num_configs;
	unsigned new_num = old_num + 1;
	unsigned long *new_configs;

	new_configs = krealloc(*configs, sizeof(*new_configs) * new_num,
			       GFP_KERNEL);
	if (!new_configs) {
		dev_err(dev, "krealloc(configs) failed\n");
		return -ENOMEM;
	}

	new_configs[old_num] = config;

	*configs = new_configs;
	*num_configs = new_num;

	return 0;
}

static void samsung_dt_free_map(struct pinctrl_dev *pctldev,
				      struct pinctrl_map *map,
				      unsigned num_maps)
{
	int i;

	for (i = 0; i < num_maps; i++)
		if (map[i].type == PIN_MAP_TYPE_CONFIGS_GROUP)
			kfree(map[i].data.configs.configs);

	kfree(map);
}

static int samsung_dt_subnode_to_map(struct samsung_pinctrl_drv_data *drvdata,
				     struct device *dev,
				     struct device_node *np,
				     struct pinctrl_map **map,
				     unsigned *reserved_maps,
				     unsigned *num_maps)
{
	int ret, i;
	u32 val;
	unsigned long config;
	unsigned long *configs = NULL;
	unsigned num_configs = 0;
	unsigned reserve;
	struct property *prop;
	const char *group;
	bool has_func = false;

	ret = of_property_read_u32(np, "samsung,pin-function", &val);
	if (!ret)
		has_func = true;

	for (i = 0; i < ARRAY_SIZE(cfg_params); i++) {
		ret = of_property_read_u32(np, cfg_params[i].property, &val);
		if (!ret) {
			config = PINCFG_PACK(cfg_params[i].param, val);
			ret = add_config(dev, &configs, &num_configs, config);
			if (ret < 0)
				goto exit;
		/* EINVAL=missing, which is fine since it's optional */
		} else if (ret != -EINVAL) {
			dev_err(dev, "could not parse property %s\n",
				cfg_params[i].property);
		}
	}

	reserve = 0;
	if (has_func)
		reserve++;
	if (num_configs)
		reserve++;
	ret = of_property_count_strings(np, "samsung,pins");
	if (ret < 0) {
		dev_err(dev, "could not parse property samsung,pins\n");
		goto exit;
	}
	reserve *= ret;

	ret = reserve_map(dev, map, reserved_maps, num_maps, reserve);
	if (ret < 0)
		goto exit;

	of_property_for_each_string(np, "samsung,pins", prop, group) {
		if (has_func) {
			ret = add_map_mux(map, reserved_maps,
						num_maps, group, np->full_name);
			if (ret < 0)
				goto exit;
		}

		if (num_configs) {
			ret = add_map_configs(dev, map, reserved_maps,
					      num_maps, group, configs,
					      num_configs);
			if (ret < 0)
				goto exit;
		}
	}

	ret = 0;

exit:
	kfree(configs);
	return ret;
}

static int samsung_dt_node_to_map(struct pinctrl_dev *pctldev,
					struct device_node *np_config,
					struct pinctrl_map **map,
					unsigned *num_maps)
{
	struct samsung_pinctrl_drv_data *drvdata;
	unsigned reserved_maps;
	struct device_node *np;
	int ret;

	drvdata = pinctrl_dev_get_drvdata(pctldev);

	reserved_maps = 0;
	*map = NULL;
	*num_maps = 0;

	if (!of_get_child_count(np_config))
		return samsung_dt_subnode_to_map(drvdata, pctldev->dev,
							np_config, map,
							&reserved_maps,
							num_maps);

	for_each_child_of_node(np_config, np) {
		ret = samsung_dt_subnode_to_map(drvdata, pctldev->dev, np, map,
						&reserved_maps, num_maps);
		if (ret < 0) {
			samsung_dt_free_map(pctldev, *map, *num_maps);
			of_node_put(np);
			return ret;
		}
	}

	return 0;
}

/* GPIO register names */
static char *gpio_regs[] = {"CON", "DAT", "PUD", "DRV", "CON_PDN", "PUD_PDN"};

static void pin_to_reg_bank(struct samsung_pinctrl_drv_data *drvdata,
			unsigned pin, void __iomem **reg, u32 *offset,
			struct samsung_pin_bank **bank);

/* common debug show function */
static void samsung_pin_dbg_show_by_type(struct samsung_pin_bank *bank,
				void __iomem *reg_base, u32 pin_offset,
				struct seq_file *s, unsigned pin,
				enum pincfg_type cfg_type)
{
	const struct samsung_pin_bank_type *type;
	u32 data, width, mask, shift, cfg_reg;

	type = bank->type;

	if (!type->fld_width[cfg_type])
		return;

	width = type->fld_width[cfg_type];
	cfg_reg = type->reg_offset[cfg_type];
	mask = (1 << width) - 1;
	shift = pin_offset * width;

	data = readl(reg_base + cfg_reg);

	data >>= shift;
	data &= mask;

	seq_printf(s, " %s(0x%x)", gpio_regs[cfg_type], data);
}

/* show GPIO register status */
static void samsung_pin_dbg_show(struct pinctrl_dev *pctldev,
				struct seq_file *s, unsigned pin)
{
	struct samsung_pinctrl_drv_data *drvdata;
	struct samsung_pin_bank *bank;
	void __iomem *reg_base;
	u32 pin_offset;
	unsigned long flags;
	enum pincfg_type cfg_type;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pin_to_reg_bank(drvdata, pin - drvdata->pin_base, &reg_base,
					&pin_offset, &bank);

	spin_lock_irqsave(&bank->slock, flags);

	for (cfg_type = 0; cfg_type < PINCFG_TYPE_NUM; cfg_type++) {
		samsung_pin_dbg_show_by_type(bank, reg_base,
					pin_offset, s, pin, cfg_type);
	}

	spin_unlock_irqrestore(&bank->slock, flags);
}

/* list of pinctrl callbacks for the pinctrl core */
static const struct pinctrl_ops samsung_pctrl_ops = {
	.get_groups_count	= samsung_get_group_count,
	.get_group_name		= samsung_get_group_name,
	.get_group_pins		= samsung_get_group_pins,
	.dt_node_to_map		= samsung_dt_node_to_map,
	.dt_free_map		= samsung_dt_free_map,
	.pin_dbg_show		= samsung_pin_dbg_show,
};

/* check if the selector is a valid pin function selector */
static int samsung_get_functions_count(struct pinctrl_dev *pctldev)
{
	struct samsung_pinctrl_drv_data *drvdata;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	return drvdata->nr_functions;
}

/* return the name of the pin function specified */
static const char *samsung_pinmux_get_fname(struct pinctrl_dev *pctldev,
						unsigned selector)
{
	struct samsung_pinctrl_drv_data *drvdata;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	return drvdata->pmx_functions[selector].name;
}

/* return the groups associated for the specified function selector */
static int samsung_pinmux_get_groups(struct pinctrl_dev *pctldev,
		unsigned selector, const char * const **groups,
		unsigned * const num_groups)
{
	struct samsung_pinctrl_drv_data *drvdata;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	*groups = drvdata->pmx_functions[selector].groups;
	*num_groups = drvdata->pmx_functions[selector].num_groups;
	return 0;
}

/*
 * given a pin number that is local to a pin controller, find out the pin bank
 * and the register base of the pin bank.
 */
static void pin_to_reg_bank(struct samsung_pinctrl_drv_data *drvdata,
			unsigned pin, void __iomem **reg, u32 *offset,
			struct samsung_pin_bank **bank)
{
	struct samsung_pin_bank *b;

	b = drvdata->pin_banks;

	while ((pin >= b->pin_base) &&
			((b->pin_base + b->nr_pins - 1) < pin))
		b++;

	*reg = drvdata->virt_base + b->pctl_offset;
	*offset = pin - b->pin_base;
	if (bank)
		*bank = b;
}

/* enable or disable a pinmux function */
static void samsung_pinmux_setup(struct pinctrl_dev *pctldev, unsigned selector,
					unsigned group, bool enable)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const struct samsung_pin_bank_type *type;
	struct samsung_pin_bank *bank;
	void __iomem *reg;
	u32 mask, shift, data, pin_offset;
	unsigned long flags;
	const struct samsung_pmx_func *func;
	const struct samsung_pin_group *grp;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	func = &drvdata->pmx_functions[selector];
	grp = &drvdata->pin_groups[group];

	pin_to_reg_bank(drvdata, grp->pins[0] - drvdata->pin_base,
			&reg, &pin_offset, &bank);
	type = bank->type;
	mask = (1 << type->fld_width[PINCFG_TYPE_FUNC]) - 1;
	shift = pin_offset * type->fld_width[PINCFG_TYPE_FUNC];
	if (shift >= 32) {
		/* Some banks have two config registers */
		shift -= 32;
		reg += 4;
	}

	spin_lock_irqsave(&bank->slock, flags);

	data = readl(reg + type->reg_offset[PINCFG_TYPE_FUNC]);
	data &= ~(mask << shift);
	if (enable)
		data |= func->val << shift;
	writel(data, reg + type->reg_offset[PINCFG_TYPE_FUNC]);

	spin_unlock_irqrestore(&bank->slock, flags);
}

/* enable a specified pinmux by writing to registers */
static int samsung_pinmux_set_mux(struct pinctrl_dev *pctldev,
				  unsigned selector,
				  unsigned group)
{
	samsung_pinmux_setup(pctldev, selector, group, true);
	return 0;
}

/* list of pinmux callbacks for the pinmux vertical in pinctrl core */
static const struct pinmux_ops samsung_pinmux_ops = {
	.get_functions_count	= samsung_get_functions_count,
	.get_function_name	= samsung_pinmux_get_fname,
	.get_function_groups	= samsung_pinmux_get_groups,
	.set_mux		= samsung_pinmux_set_mux,
};

/* set or get the pin config settings for a specified pin */
static int samsung_pinconf_rw(struct pinctrl_dev *pctldev, unsigned int pin,
				unsigned long *config, bool set)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const struct samsung_pin_bank_type *type;
	struct samsung_pin_bank *bank;
	void __iomem *reg_base;
	enum pincfg_type cfg_type = PINCFG_UNPACK_TYPE(*config);
	u32 data, width, pin_offset, mask, shift;
	u32 cfg_value, cfg_reg;
	unsigned long flags;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pin_to_reg_bank(drvdata, pin - drvdata->pin_base, &reg_base,
					&pin_offset, &bank);
	type = bank->type;

	if (cfg_type >= PINCFG_TYPE_NUM || !type->fld_width[cfg_type])
		return -EINVAL;

	width = type->fld_width[cfg_type];
	cfg_reg = type->reg_offset[cfg_type];

	spin_lock_irqsave(&bank->slock, flags);

	mask = (1 << width) - 1;
	shift = pin_offset * width;
	data = readl(reg_base + cfg_reg);

	if (set) {
		cfg_value = PINCFG_UNPACK_VALUE(*config);
		data &= ~(mask << shift);
		data |= (cfg_value << shift);
		writel(data, reg_base + cfg_reg);
	} else {
		data >>= shift;
		data &= mask;
		*config = PINCFG_PACK(cfg_type, data);
	}

	spin_unlock_irqrestore(&bank->slock, flags);

	return 0;
}

/* set the pin config settings for a specified pin */
static int samsung_pinconf_set(struct pinctrl_dev *pctldev, unsigned int pin,
				unsigned long *configs, unsigned num_configs)
{
	int i, ret;

	for (i = 0; i < num_configs; i++) {
		ret = samsung_pinconf_rw(pctldev, pin, &configs[i], true);
		if (ret < 0)
			return ret;
	} /* for each config */

	return 0;
}

/* get the pin config settings for a specified pin */
static int samsung_pinconf_get(struct pinctrl_dev *pctldev, unsigned int pin,
					unsigned long *config)
{
	return samsung_pinconf_rw(pctldev, pin, config, false);
}

/* set the pin config settings for a specified pin group */
static int samsung_pinconf_group_set(struct pinctrl_dev *pctldev,
			unsigned group, unsigned long *configs,
			unsigned num_configs)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const unsigned int *pins;
	unsigned int cnt;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pins = drvdata->pin_groups[group].pins;

	for (cnt = 0; cnt < drvdata->pin_groups[group].num_pins; cnt++)
		samsung_pinconf_set(pctldev, pins[cnt], configs, num_configs);

	return 0;
}

/* get the pin config settings for a specified pin group */
static int samsung_pinconf_group_get(struct pinctrl_dev *pctldev,
				unsigned int group, unsigned long *config)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const unsigned int *pins;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pins = drvdata->pin_groups[group].pins;
	samsung_pinconf_get(pctldev, pins[0], config);
	return 0;
}

/* show whole PUD, DRV, CON_PDN and PUD_PDN register status */
static void samsung_pinconf_dbg_show(struct pinctrl_dev *pctldev,
				struct seq_file *s, unsigned pin)
{
	struct samsung_pinctrl_drv_data *drvdata;
	struct samsung_pin_bank *bank;
	void __iomem *reg_base;
	u32 pin_offset;
	unsigned long flags;
	enum pincfg_type cfg_type;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pin_to_reg_bank(drvdata, pin - drvdata->pin_base, &reg_base,
					&pin_offset, &bank);

	spin_lock_irqsave(&bank->slock, flags);

	for (cfg_type = PINCFG_TYPE_PUD; cfg_type <= PINCFG_TYPE_PUD_PDN
					; cfg_type++) {
		samsung_pin_dbg_show_by_type(bank, reg_base,
					pin_offset, s, pin, cfg_type);
	}

	spin_unlock_irqrestore(&bank->slock, flags);
}

/* show group's PUD, DRV, CON_PDN and PUD_PDN register status */
static void samsung_pinconf_group_dbg_show(struct pinctrl_dev *pctldev,
				struct seq_file *s, unsigned group)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const unsigned int *pins;
	int i;

	drvdata = pinctrl_dev_get_drvdata(pctldev);
	pins = drvdata->pin_groups[group].pins;

	for (i = 0; i < drvdata->pin_groups[group].num_pins; i++) {
		seq_printf(s, "\n\t%s:", pin_get_name(pctldev, pins[i]));
		samsung_pinconf_dbg_show(pctldev, s, pins[i]);
	}
}

/* list of pinconfig callbacks for pinconfig vertical in the pinctrl code */
static const struct pinconf_ops samsung_pinconf_ops = {
	.pin_config_get		= samsung_pinconf_get,
	.pin_config_set		= samsung_pinconf_set,
	.pin_config_group_get	= samsung_pinconf_group_get,
	.pin_config_group_set	= samsung_pinconf_group_set,
	.pin_config_dbg_show	= samsung_pinconf_dbg_show,
	.pin_config_group_dbg_show = samsung_pinconf_group_dbg_show,
};

/* gpiolib gpio_set callback function */
static void samsung_gpio_set(struct gpio_chip *gc, unsigned offset, int value)
{
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	const struct samsung_pin_bank_type *type = bank->type;
	void __iomem *reg;
	u32 data;

	reg = bank->drvdata->virt_base + bank->pctl_offset;

	data = readl(reg + type->reg_offset[PINCFG_TYPE_DAT]);
	data &= ~(1 << offset);
	if (value)
		data |= 1 << offset;
	writel(data, reg + type->reg_offset[PINCFG_TYPE_DAT]);
}

static void samsung_gpio_set_value(struct gpio_chip *gc, unsigned offset, int value)
{
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	unsigned long flags;

	spin_lock_irqsave(&bank->slock, flags);
	samsung_gpio_set(gc, offset, value);
	spin_unlock_irqrestore(&bank->slock, flags);
}

/* gpiolib gpio_get callback function */
static int samsung_gpio_get(struct gpio_chip *gc, unsigned offset)
{
	void __iomem *reg;
	u32 data;
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	const struct samsung_pin_bank_type *type = bank->type;

	reg = bank->drvdata->virt_base + bank->pctl_offset;

	data = readl(reg + type->reg_offset[PINCFG_TYPE_DAT]);
	data >>= offset;
	data &= 1;
	return data;
}

/*
 * The calls to gpio_direction_output() and gpio_direction_input()
 * leads to this function call.
 */
static int samsung_gpio_set_direction(struct gpio_chip *gc,
					     unsigned offset, bool input)
{
	const struct samsung_pin_bank_type *type;
	struct samsung_pin_bank *bank;
	struct samsung_pinctrl_drv_data *drvdata;
	void __iomem *reg;
	u32 data, mask, shift;

	bank = gc_to_pin_bank(gc);
	type = bank->type;
	drvdata = bank->drvdata;

	reg = drvdata->virt_base + bank->pctl_offset +
					type->reg_offset[PINCFG_TYPE_FUNC];

	mask = (1 << type->fld_width[PINCFG_TYPE_FUNC]) - 1;
	shift = offset * type->fld_width[PINCFG_TYPE_FUNC];
	if (shift >= 32) {
		/* Some banks have two config registers */
		shift -= 32;
		reg += 4;
	}

	data = readl(reg);
	data &= ~(mask << shift);
	if (!input)
		data |= FUNC_OUTPUT << shift;
	writel(data, reg);

	return 0;
}

/* gpiolib gpio_direction_input callback function. */
static int samsung_gpio_direction_input(struct gpio_chip *gc, unsigned offset)
{
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&bank->slock, flags);
	ret = samsung_gpio_set_direction(gc, offset, true);
	spin_unlock_irqrestore(&bank->slock, flags);
	return ret;
}

/* gpiolib gpio_direction_output callback function. */
static int samsung_gpio_direction_output(struct gpio_chip *gc, unsigned offset,
							int value)
{
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&bank->slock, flags);
	samsung_gpio_set(gc, offset, value);
	ret = samsung_gpio_set_direction(gc, offset, false);
	spin_unlock_irqrestore(&bank->slock, flags);

	return ret;
}

/*
 * gpiolib gpio_to_irq callback function. Creates a mapping between a GPIO pin
 * and a virtual IRQ, if not already present.
 */
static int samsung_gpio_to_irq(struct gpio_chip *gc, unsigned offset)
{
	struct samsung_pin_bank *bank = gc_to_pin_bank(gc);
	unsigned int virq;

	if (!bank->irq_domain)
		return -ENXIO;

	virq = irq_create_mapping(bank->irq_domain, offset);

	return (virq) ? : -ENXIO;
}

static struct samsung_pin_group *samsung_pinctrl_create_groups(
				struct device *dev,
				struct samsung_pinctrl_drv_data *drvdata,
				unsigned int *cnt)
{
	struct pinctrl_desc *ctrldesc = &drvdata->pctl;
	struct samsung_pin_group *groups, *grp;
	const struct pinctrl_pin_desc *pdesc;
	int i;

	groups = devm_kzalloc(dev, ctrldesc->npins * sizeof(*groups),
				GFP_KERNEL);
	if (!groups)
		return ERR_PTR(-EINVAL);
	grp = groups;

	pdesc = ctrldesc->pins;
	for (i = 0; i < ctrldesc->npins; ++i, ++pdesc, ++grp) {
		grp->name = pdesc->name;
		grp->pins = &pdesc->number;
		grp->num_pins = 1;
	}

	*cnt = ctrldesc->npins;
	return groups;
}

static int samsung_pinctrl_create_function(struct device *dev,
				struct samsung_pinctrl_drv_data *drvdata,
				struct device_node *func_np,
				struct samsung_pmx_func *func)
{
	int npins;
	int ret;
	int i;

	if (of_property_read_u32(func_np, "samsung,pin-function", &func->val))
		return 0;

	npins = of_property_count_strings(func_np, "samsung,pins");
	if (npins < 1) {
		dev_err(dev, "invalid pin list in %s node", func_np->name);
		return -EINVAL;
	}

	func->name = func_np->full_name;

	func->groups = devm_kzalloc(dev, npins * sizeof(char *), GFP_KERNEL);
	if (!func->groups)
		return -ENOMEM;

	for (i = 0; i < npins; ++i) {
		const char *gname;

		ret = of_property_read_string_index(func_np, "samsung,pins",
							i, &gname);
		if (ret) {
			dev_err(dev,
				"failed to read pin name %d from %s node\n",
				i, func_np->name);
			return ret;
		}

		func->groups[i] = gname;
	}

	func->num_groups = npins;
	return 1;
}

static struct samsung_pmx_func *samsung_pinctrl_create_functions(
				struct device *dev,
				struct samsung_pinctrl_drv_data *drvdata,
				unsigned int *cnt)
{
	struct samsung_pmx_func *functions, *func;
	struct device_node *dev_np = dev->of_node;
	struct device_node *cfg_np;
	unsigned int func_cnt = 0;
	int ret;

	/*
	 * Iterate over all the child nodes of the pin controller node
	 * and create pin groups and pin function lists.
	 */
	for_each_child_of_node(dev_np, cfg_np) {
		struct device_node *func_np;

		if (!of_get_child_count(cfg_np)) {
			if (!of_find_property(cfg_np,
			    "samsung,pin-function", NULL))
				continue;
			++func_cnt;
			continue;
		}

		for_each_child_of_node(cfg_np, func_np) {
			if (!of_find_property(func_np,
			    "samsung,pin-function", NULL))
				continue;
			++func_cnt;
		}
	}

	functions = devm_kzalloc(dev, func_cnt * sizeof(*functions),
					GFP_KERNEL);
	if (!functions) {
		dev_err(dev, "failed to allocate memory for function list\n");
		return ERR_PTR(-EINVAL);
	}
	func = functions;

	/*
	 * Iterate over all the child nodes of the pin controller node
	 * and create pin groups and pin function lists.
	 */
	func_cnt = 0;
	for_each_child_of_node(dev_np, cfg_np) {
		struct device_node *func_np;

		if (!of_get_child_count(cfg_np)) {
			ret = samsung_pinctrl_create_function(dev, drvdata,
							cfg_np, func);
			if (ret < 0) {
				of_node_put(cfg_np);
				return ERR_PTR(ret);
			}
			if (ret > 0) {
				++func;
				++func_cnt;
			}
			continue;
		}

		for_each_child_of_node(cfg_np, func_np) {
			ret = samsung_pinctrl_create_function(dev, drvdata,
						func_np, func);
			if (ret < 0) {
				of_node_put(func_np);
				of_node_put(cfg_np);
				return ERR_PTR(ret);
			}
			if (ret > 0) {
				++func;
				++func_cnt;
			}
		}
	}

	*cnt = func_cnt;
	return functions;
}

/*
 * Parse the information about all the available pin groups and pin functions
 * from device node of the pin-controller. A pin group is formed with all
 * the pins listed in the "samsung,pins" property.
 */

static int samsung_pinctrl_parse_dt(struct platform_device *pdev,
				    struct samsung_pinctrl_drv_data *drvdata)
{
	struct device *dev = &pdev->dev;
	struct samsung_pin_group *groups;
	struct samsung_pmx_func *functions;
	unsigned int grp_cnt = 0, func_cnt = 0;

	groups = samsung_pinctrl_create_groups(dev, drvdata, &grp_cnt);
	if (IS_ERR(groups)) {
		dev_err(dev, "failed to parse pin groups\n");
		return PTR_ERR(groups);
	}

	functions = samsung_pinctrl_create_functions(dev, drvdata, &func_cnt);
	if (IS_ERR(functions)) {
		dev_err(dev, "failed to parse pin functions\n");
		return PTR_ERR(functions);
	}

	drvdata->pin_groups = groups;
	drvdata->nr_groups = grp_cnt;
	drvdata->pmx_functions = functions;
	drvdata->nr_functions = func_cnt;

	return 0;
}

/* register the pinctrl interface with the pinctrl subsystem */
static int samsung_pinctrl_register(struct platform_device *pdev,
				    struct samsung_pinctrl_drv_data *drvdata)
{
	struct pinctrl_desc *ctrldesc = &drvdata->pctl;
	struct pinctrl_pin_desc *pindesc, *pdesc;
	struct samsung_pin_bank *pin_bank;
	char *pin_names;
	int pin, bank, ret;

	ctrldesc->name = "samsung-pinctrl";
	ctrldesc->owner = THIS_MODULE;
	ctrldesc->pctlops = &samsung_pctrl_ops;
	ctrldesc->pmxops = &samsung_pinmux_ops;
	ctrldesc->confops = &samsung_pinconf_ops;

	pindesc = devm_kzalloc(&pdev->dev, sizeof(*pindesc) *
			drvdata->nr_pins, GFP_KERNEL);
	if (!pindesc) {
		dev_err(&pdev->dev, "mem alloc for pin descriptors failed\n");
		return -ENOMEM;
	}
	ctrldesc->pins = pindesc;
	ctrldesc->npins = drvdata->nr_pins;

	/* dynamically populate the pin number and pin name for pindesc */
	for (pin = 0, pdesc = pindesc; pin < ctrldesc->npins; pin++, pdesc++)
		pdesc->number = pin + drvdata->pin_base;

	/*
	 * allocate space for storing the dynamically generated names for all
	 * the pins which belong to this pin-controller.
	 */
	pin_names = devm_kzalloc(&pdev->dev, sizeof(char) * PIN_NAME_LENGTH *
					drvdata->nr_pins, GFP_KERNEL);
	if (!pin_names) {
		dev_err(&pdev->dev, "mem alloc for pin names failed\n");
		return -ENOMEM;
	}

	/* for each pin, the name of the pin is pin-bank name + pin number */
	for (bank = 0; bank < drvdata->nr_banks; bank++) {
		pin_bank = &drvdata->pin_banks[bank];
		for (pin = 0; pin < pin_bank->nr_pins; pin++) {
			snprintf(pin_names, PIN_NAME_LENGTH,
				"%s-%d", pin_bank->name, pin);
			pdesc = pindesc + pin_bank->pin_base + pin;
			pdesc->name = pin_names;
			pin_names += PIN_NAME_LENGTH;
		}
	}

	ret = samsung_pinctrl_parse_dt(pdev, drvdata);
	if (ret)
		return ret;

	drvdata->pctl_dev = pinctrl_register(ctrldesc, &pdev->dev, drvdata);
	if (IS_ERR(drvdata->pctl_dev)) {
		dev_err(&pdev->dev, "could not register pinctrl driver\n");
		return PTR_ERR(drvdata->pctl_dev);
	}

	for (bank = 0; bank < drvdata->nr_banks; ++bank) {
		pin_bank = &drvdata->pin_banks[bank];
		pin_bank->grange.name = pin_bank->name;
		pin_bank->grange.id = bank;
		pin_bank->grange.pin_base = drvdata->pin_base
						+ pin_bank->pin_base;
		pin_bank->grange.base = pin_bank->gpio_chip.base;
		pin_bank->grange.npins = pin_bank->gpio_chip.ngpio;
		pin_bank->grange.gc = &pin_bank->gpio_chip;
		pinctrl_add_gpio_range(drvdata->pctl_dev, &pin_bank->grange);
	}

	return 0;
}

static const struct gpio_chip samsung_gpiolib_chip = {
	.request = gpiochip_generic_request,
	.free = gpiochip_generic_free,
	.set = samsung_gpio_set_value,
	.get = samsung_gpio_get,
	.direction_input = samsung_gpio_direction_input,
	.direction_output = samsung_gpio_direction_output,
	.to_irq = samsung_gpio_to_irq,
	.owner = THIS_MODULE,
};

/* register the gpiolib interface with the gpiolib subsystem */
static int samsung_gpiolib_register(struct platform_device *pdev,
				    struct samsung_pinctrl_drv_data *drvdata)
{
	struct samsung_pin_bank *bank = drvdata->pin_banks;
	struct gpio_chip *gc;
	int ret;
	int i;

	for (i = 0; i < drvdata->nr_banks; ++i, ++bank) {
		bank->gpio_chip = samsung_gpiolib_chip;

		gc = &bank->gpio_chip;
		gc->base = drvdata->pin_base + bank->pin_base;
		gc->ngpio = bank->nr_pins;
		gc->dev = &pdev->dev;
		gc->of_node = bank->of_node;
		gc->label = bank->name;

		ret = gpiochip_add(gc);
		if (ret) {
			dev_err(&pdev->dev, "failed to register gpio_chip %s, error code: %d\n",
							gc->label, ret);
			goto fail;
		}
	}

	return 0;

fail:
	for (--i, --bank; i >= 0; --i, --bank)
		gpiochip_remove(&bank->gpio_chip);
	return ret;
}

/* unregister the gpiolib interface with the gpiolib subsystem */
static int samsung_gpiolib_unregister(struct platform_device *pdev,
				      struct samsung_pinctrl_drv_data *drvdata)
{
	struct samsung_pin_bank *bank = drvdata->pin_banks;
	int i;

	for (i = 0; i < drvdata->nr_banks; ++i, ++bank)
		gpiochip_remove(&bank->gpio_chip);

	return 0;
}

static const struct of_device_id samsung_pinctrl_dt_match[];

/* retrieve the soc specific data */
static const struct samsung_pin_ctrl *
samsung_pinctrl_get_soc_data(struct samsung_pinctrl_drv_data *d,
			     struct platform_device *pdev)
{
	int id;
	const struct of_device_id *match;
	struct device_node *node = pdev->dev.of_node;
	struct device_node *np;
	const struct samsung_pin_bank_data *bdata;
	const struct samsung_pin_ctrl *ctrl;
	struct samsung_pin_bank *bank;
	int i;

	id = of_alias_get_id(node, "pinctrl");
	if (id < 0) {
		dev_err(&pdev->dev, "failed to get alias id\n");
		return ERR_PTR(-ENOENT);
	}
	match = of_match_node(samsung_pinctrl_dt_match, node);
	ctrl = (struct samsung_pin_ctrl *)match->data + id;

	d->suspend = ctrl->suspend;
	d->resume = ctrl->resume;
	d->nr_banks = ctrl->nr_banks;
	d->pin_banks = devm_kcalloc(&pdev->dev, d->nr_banks,
					sizeof(*d->pin_banks), GFP_KERNEL);
	if (!d->pin_banks)
		return ERR_PTR(-ENOMEM);

	bank = d->pin_banks;
	bdata = ctrl->pin_banks;
	for (i = 0; i < ctrl->nr_banks; ++i, ++bdata, ++bank) {
		bank->type = bdata->type;
		bank->pctl_offset = bdata->pctl_offset;
		bank->nr_pins = bdata->nr_pins;
		bank->eint_func = bdata->eint_func;
		bank->eint_type = bdata->eint_type;
		bank->eint_mask = bdata->eint_mask;
		bank->eint_offset = bdata->eint_offset;
		bank->name = bdata->name;

		spin_lock_init(&bank->slock);
		bank->drvdata = d;
		bank->pin_base = d->nr_pins;
		d->nr_pins += bank->nr_pins;
	}

	for_each_child_of_node(node, np) {
		if (!of_find_property(np, "gpio-controller", NULL))
			continue;
		bank = d->pin_banks;
		for (i = 0; i < d->nr_banks; ++i, ++bank) {
			if (!strcmp(bank->name, np->name)) {
				bank->of_node = np;
				break;
			}
		}
	}

	d->pin_base = pin_base;
	pin_base += d->nr_pins;

	return ctrl;
}

static int samsung_pinctrl_probe(struct platform_device *pdev)
{
	struct samsung_pinctrl_drv_data *drvdata;
	const struct samsung_pin_ctrl *ctrl;
	struct device *dev = &pdev->dev;
	struct resource *res;
	int ret;

	if (!dev->of_node) {
		dev_err(dev, "device tree node not found\n");
		return -ENODEV;
	}

	drvdata = devm_kzalloc(dev, sizeof(*drvdata), GFP_KERNEL);
	if (!drvdata) {
		dev_err(dev, "failed to allocate memory for driver's "
				"private data\n");
		return -ENOMEM;
	}

	ctrl = samsung_pinctrl_get_soc_data(drvdata, pdev);
	if (IS_ERR(ctrl)) {
		dev_err(&pdev->dev, "driver data not available\n");
		return PTR_ERR(ctrl);
	}
	drvdata->dev = dev;

	res = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	drvdata->virt_base = devm_ioremap_resource(&pdev->dev, res);
	if (IS_ERR(drvdata->virt_base))
		return PTR_ERR(drvdata->virt_base);

	res = platform_get_resource(pdev, IORESOURCE_IRQ, 0);
	if (res)
		drvdata->irq = res->start;

	ret = samsung_gpiolib_register(pdev, drvdata);
	if (ret)
		return ret;

	ret = samsung_pinctrl_register(pdev, drvdata);
	if (ret) {
		samsung_gpiolib_unregister(pdev, drvdata);
		return ret;
	}

	drvdata->pinctrl = devm_pinctrl_get(dev);
	if (IS_ERR(drvdata->pinctrl)) {
		dev_err(dev, "could not get pinctrl\n");
		return PTR_ERR(drvdata->pinctrl);
	}

	drvdata->pins_default = pinctrl_lookup_state(drvdata->pinctrl,
						 PINCTRL_STATE_DEFAULT);
	if (IS_ERR(drvdata->pins_default))
		dev_dbg(dev, "could not get default pinstate\n");

	drvdata->pins_sleep = pinctrl_lookup_state(drvdata->pinctrl,
					      PINCTRL_STATE_SLEEP);
	if (IS_ERR(drvdata->pins_sleep))
		dev_dbg(dev, "could not get sleep pinstate\n");

	if (ctrl->eint_gpio_init)
		ctrl->eint_gpio_init(drvdata);
	if (ctrl->eint_wkup_init)
		ctrl->eint_wkup_init(drvdata);

	platform_set_drvdata(pdev, drvdata);

	/* Add to the global list */
	list_add_tail(&drvdata->node, &drvdata_list);

	return 0;
}

#ifdef CONFIG_PM

/**
 * samsung_pinctrl_suspend_dev - save pinctrl state for suspend for a device
 *
 * Save data for all banks handled by this device.
 */
static void samsung_pinctrl_suspend_dev(
	struct samsung_pinctrl_drv_data *drvdata)
{
	void __iomem *virt_base = drvdata->virt_base;
	int i;
	int ret;

	if (!drvdata->suspend)
		return;

	if (!IS_ERR(drvdata->pins_sleep)) {
		/* This is ignore to disable mux configuration. */
		drvdata->pinctrl->state = NULL;

		ret = pinctrl_select_state(drvdata->pinctrl, drvdata->pins_sleep);
		if (ret)
			dev_err(drvdata->dev, "could not set default pinstate\n");
	}

	for (i = 0; i < drvdata->nr_banks; i++) {
		struct samsung_pin_bank *bank = &drvdata->pin_banks[i];
		void __iomem *reg = virt_base + bank->pctl_offset;
		const u8 *offs = bank->type->reg_offset;
		const u8 *widths = bank->type->fld_width;
		enum pincfg_type type;

		/* Registers without a powerdown config aren't lost */
		if (!widths[PINCFG_TYPE_CON_PDN])
			continue;

		for (type = 0; type < PINCFG_TYPE_NUM; type++)
			if (widths[type])
				bank->pm_save[type] = readl(reg + offs[type]);

		if (widths[PINCFG_TYPE_FUNC] * bank->nr_pins > 32) {
			/* Some banks have two config registers */
			bank->pm_save[PINCFG_TYPE_NUM] =
				readl(reg + offs[PINCFG_TYPE_FUNC] + 4);
			pr_debug("Save %s @ %p (con %#010x %08x)\n",
				 bank->name, reg,
				 bank->pm_save[PINCFG_TYPE_FUNC],
				 bank->pm_save[PINCFG_TYPE_NUM]);
		} else {
			pr_debug("Save %s @ %p (con %#010x)\n", bank->name,
				 reg, bank->pm_save[PINCFG_TYPE_FUNC]);
		}
	}

	drvdata->suspend(drvdata);
}

/**
 * samsung_pinctrl_resume_dev - restore pinctrl state from suspend for a device
 *
 * Restore one of the banks that was saved during suspend.
 *
 * We don't bother doing anything complicated to avoid glitching lines since
 * we're called before pad retention is turned off.
 */
static void samsung_pinctrl_resume_dev(struct samsung_pinctrl_drv_data *drvdata)
{
	void __iomem *virt_base = drvdata->virt_base;
	int i;

	if (!drvdata->resume)
		return;

	drvdata->resume(drvdata);

	for (i = 0; i < drvdata->nr_banks; i++) {
		struct samsung_pin_bank *bank = &drvdata->pin_banks[i];
		void __iomem *reg = virt_base + bank->pctl_offset;
		const u8 *offs = bank->type->reg_offset;
		const u8 *widths = bank->type->fld_width;
		enum pincfg_type type;

		/* Registers without a powerdown config aren't lost */
		if (!widths[PINCFG_TYPE_CON_PDN])
			continue;

#if defined(ENABLE_SENSORS_FPRINT_SECURE)
		fps_resume_set();
#endif

		if (widths[PINCFG_TYPE_FUNC] * bank->nr_pins > 32) {
			/* Some banks have two config registers */
			pr_debug("%s @ %p (con %#010x %08x => %#010x %08x)\n",
				 bank->name, reg,
				 readl(reg + offs[PINCFG_TYPE_FUNC]),
				 readl(reg + offs[PINCFG_TYPE_FUNC] + 4),
				 bank->pm_save[PINCFG_TYPE_FUNC],
				 bank->pm_save[PINCFG_TYPE_NUM]);
			writel(bank->pm_save[PINCFG_TYPE_NUM],
			       reg + offs[PINCFG_TYPE_FUNC] + 4);
		} else {
			pr_debug("%s @ %p (con %#010x => %#010x)\n", bank->name,
				 reg, readl(reg + offs[PINCFG_TYPE_FUNC]),
				 bank->pm_save[PINCFG_TYPE_FUNC]);
		}
		for (type = 0; type < PINCFG_TYPE_NUM; type++)
			if (widths[type])
				writel(bank->pm_save[type], reg + offs[type]);
	}

	/* For changing state without writing register. */
	drvdata->pinctrl->state = drvdata->pins_default;
}

/**
 * samsung_pinctrl_suspend - save pinctrl state for suspend
 *
 * Save data for all banks across all devices.
 */
static int samsung_pinctrl_suspend(void)
{
	struct samsung_pinctrl_drv_data *drvdata;

	list_for_each_entry(drvdata, &drvdata_list, node) {
		samsung_pinctrl_suspend_dev(drvdata);
	}

	return 0;
}

/**
 * samsung_pinctrl_resume - restore pinctrl state for suspend
 *
 * Restore data for all banks across all devices.
 */
static void samsung_pinctrl_resume(void)
{
	struct samsung_pinctrl_drv_data *drvdata;

	list_for_each_entry_reverse(drvdata, &drvdata_list, node) {
		samsung_pinctrl_resume_dev(drvdata);
	}
}

u32 exynos_eint_to_pin_num(int eint)
{
	struct samsung_pinctrl_drv_data *drvdata;
	struct samsung_pin_bank *pbank;
	int i, offset = 0;

	drvdata = list_first_entry(&drvdata_list,
			struct samsung_pinctrl_drv_data, node);

	for (i = 0; i < drvdata->nr_banks; i++) {
		pbank = &drvdata->pin_banks[i];
		if (!strncmp(pbank->name, "gpa0", strlen(pbank->name)))
			break;

		offset += pbank->nr_pins;
	}

	return drvdata->pin_base + eint + offset;
}

#else
#define samsung_pinctrl_suspend		NULL
#define samsung_pinctrl_resume		NULL
#endif

static struct syscore_ops samsung_pinctrl_syscore_ops = {
	.suspend	= samsung_pinctrl_suspend,
	.resume		= samsung_pinctrl_resume,
};

#if defined(CONFIG_SEC_GPIO_DVS) && defined(CONFIG_PINCTRL_EXYNOS)
#define GET_RESULT_GPIO(a, b, c)	\
	((a<<4 & 0xF0) | (b<<1 & 0xE) | (c & 0x1))

static struct gpiomap_result_t gpiomap_result;

static u32 gpiodvs_get_by_type(struct samsung_pin_bank *bank,
				void __iomem *reg_base, u32 pin_offset,
				unsigned pin, enum pincfg_type cfg_type)
{
	const struct samsung_pin_bank_type *type;
	u32 data, width, mask, shift, cfg_reg;

	type = bank->type;

	if (!type->fld_width[cfg_type])
		return 0;

	width = type->fld_width[cfg_type];
	cfg_reg = type->reg_offset[cfg_type];
	mask = (1 << width) - 1;
	shift = pin_offset * width;

	data = readl(reg_base + cfg_reg);

	data >>= shift;
	data &= mask;

	return data;
}

static u8 gpiodvs_combine_data(u32 *data, unsigned char phonestate)
{
	u8 temp_io, temp_pdpu, temp_lh;

	/* GPIO DVS
	 * FUNC - input: 1, output: 2 eint:3 func: 0
	 * PUD - no-pull: 0, pull-down: 1, pull-up: 2 error: 7
	 * DATA - high: 1, low: 0
	 */
	if (phonestate== PHONE_INIT) {
		switch (data[PINCFG_TYPE_FUNC]) {
		case 0x0:	/* input */
			temp_io = 1;
			break;
		case 0x1:	/* output */
			temp_io = 2;
			break;
		case 0xf:	/* eint */
			temp_io = 3;
			break;
		default:	/* func */
			temp_io = 0;
			break;
		}

		if (data[PINCFG_TYPE_PUD] == 3)
			data[PINCFG_TYPE_PUD] = 2;

		temp_pdpu = data[PINCFG_TYPE_PUD];
		temp_lh = data[PINCFG_TYPE_DAT];
	} else {
		switch (data[PINCFG_TYPE_CON_PDN]) {
		case 0x0:	/* output low */
			temp_io = 2;
			temp_lh = 0;
			break;
		case 0x1:	/* output high*/
			temp_io = 2;
			temp_lh = 1;
			break;
		case 0x2:	/* input */
			temp_io = 1;
			temp_lh = data[PINCFG_TYPE_DAT];
			break;
		case 0x3:	/* previous state */
			temp_io = 4;
			temp_lh = data[PINCFG_TYPE_DAT];
			break;
		default:	/* func */
			pr_err("%s: invalid con pdn: %u\n", __func__,
					data[PINCFG_TYPE_CON_PDN]);
			temp_io = 0;
			temp_lh = 0;
			break;
		}

		if (data[PINCFG_TYPE_PUD_PDN] == 3)
			data[PINCFG_TYPE_PUD_PDN] = 2;

		temp_pdpu = data[PINCFG_TYPE_PUD_PDN];
	}

	return GET_RESULT_GPIO(temp_io, temp_pdpu, temp_lh);
}

static void gpiodvs_print_pin_state(enum gdvs_phone_status status,
		int alive, const char *bank_name, int pin_num, u32 *data)
{
	char buf[48];
	int len = 0;
	int pin_dat = 0;

	if (status == PHONE_INIT || alive == 1) {
		if (alive == 1)
			len = sprintf(buf, "gpio sleep-state: %s-%d ", bank_name, pin_num);
		else
			len = sprintf(buf, "gpio initial-state: %s-%d ", bank_name, pin_num);

		switch (data[PINCFG_TYPE_FUNC]) {
		case 0x0:	/* input */
			len += sprintf(&buf[len], "IN");
			break;
		case 0x1:	/* output */
			len += sprintf(&buf[len], "OUT");
			break;
		case 0xf:	/* eint */
			len += sprintf(&buf[len], "INT");
			break;
		default:	/* func */
			len += sprintf(&buf[len], "FUNC");
			break;
		}

		switch (data[PINCFG_TYPE_PUD]) {
		case 0x0:
			len += sprintf(&buf[len], "/NP");
			break;
		case 0x1:
			len += sprintf(&buf[len], "/PD");
			break;
		case 0x2:
			len += sprintf(&buf[len], "/PU");
			break;
		default:
			len += sprintf(&buf[len], "/UN");	/* unknown */
			break;
		}

		switch (data[PINCFG_TYPE_DAT]) {
		case 0x0:
			len += sprintf(&buf[len], "/L");
			break;
		case 0x1:
			len += sprintf(&buf[len], "/H");
			break;
		default:
			len += sprintf(&buf[len], "/U");	/* unknown */
			break;
		}
	} else {
		// PHONE_SLEEP
		len = sprintf(buf, "gpio sleep-state: %s-%d ", bank_name, pin_num);

		switch (data[PINCFG_TYPE_CON_PDN]) {
		case 0x0:	/* output low */
			len += sprintf(&buf[len], "OUT");
			pin_dat = 0;
			break;
		case 0x1:	/* output high*/
			len += sprintf(&buf[len], "OUT");
			pin_dat = 1;
			break;
		case 0x2:	/* input */
			len += sprintf(&buf[len], "IN");
			pin_dat = data[PINCFG_TYPE_DAT];
			break;
		case 0x3:	/* previous state */
			len += sprintf(&buf[len], "PREV");
			pin_dat = data[PINCFG_TYPE_DAT];
			break;
		default:	/* func */
			len += sprintf(&buf[len], "ERR");
			break;
		}

		switch (data[PINCFG_TYPE_PUD_PDN]) {
		case 0x0:
			len += sprintf(&buf[len], "/NP");
			break;
		case 0x1:
			len += sprintf(&buf[len], "/PD");
			break;
		case 0x2:
			len += sprintf(&buf[len], "/PU");
			break;
		default:
			len += sprintf(&buf[len], "/UN");	/* unknown */
			break;
		}

		switch (pin_dat) {
		case 0x0:
			len += sprintf(&buf[len], "/L");
			break;
		case 0x1:
			len += sprintf(&buf[len], "/H");
			break;
		default:
			len += sprintf(&buf[len], "/U");	/* unknown */
			break;
		}
	}

	pr_info("%s\n", buf);
}

static void gpiodvs_check_init_gpio(struct samsung_pinctrl_drv_data *drvdata,
					unsigned pin)
{
	static unsigned int init_gpio_idx;
	struct samsung_pin_bank *bank;
	void __iomem *reg_base;
	u32 pin_offset;
	unsigned long flags;
	enum pincfg_type type;
	u32 data[PINCFG_TYPE_NUM];
	static int pin_num = 0;

	pin_to_reg_bank(drvdata, pin - drvdata->pin_base,
					&reg_base, &pin_offset, &bank);

#ifdef CONFIG_ESE_SECURE
	if (!strncmp(bank->name, CONFIG_ESE_SECURE_GPIO, 4)) {
		init_gpio_idx++;
		goto out;
	}
#endif

	/* GPH ports are AUD interface (I2S, UART, PCM, SB) that should not
	 * access when AUD power is disabled
	 */
	if (!strncmp(bank->name, "gph", 3)) {
		init_gpio_idx++;
		goto out;
	}
	/* both gpj0/1 are skiped for dvs test in zero project */
#if defined(CONFIG_MST_SECURE_GPIO) && !defined(CONFIG_MST_NOBLE_TARGET) && !defined(CONFIG_MST_ZEN_TARGET)
	if (!strncmp(bank->name, "gpj", 3)) {
		init_gpio_idx++;
		goto out;
	}
#else /* only gpj0 is skiped for dvs test in noble/zen project */
	if (!strncmp(bank->name, "gpj0", 4)) {
		init_gpio_idx++;
		goto out;
	}
#endif

#ifdef CONFIG_SOC_EXYNOS5433
	/* xxxx group is dummy for align insted of GPF0 */
	if (!strcmp(bank->name, "xxxx"))
		return;
#endif

	spin_lock_irqsave(&bank->slock, flags);
	for (type = PINCFG_TYPE_FUNC; type <= PINCFG_TYPE_PUD; type++)
		data[type] = gpiodvs_get_by_type(bank, reg_base, pin_offset,
				pin, type);
	spin_unlock_irqrestore(&bank->slock, flags);

	gpiomap_result.init[init_gpio_idx++] =
		gpiodvs_combine_data(data, PHONE_INIT);

	gpiodvs_print_pin_state(PHONE_INIT, 0, bank->name, pin_num, data);
out:
	pin_num++;
	if (pin_num == bank->nr_pins) {
		pin_num = 0;
	}

	pr_debug("%s: init[%u]=0x%02x\n", __func__, init_gpio_idx - 1,
			gpiomap_result.init[init_gpio_idx - 1]);
}

static void gpiodvs_check_sleep_gpio(struct samsung_pinctrl_drv_data *drvdata,
					unsigned pin)
{
	static unsigned int sleep_gpio_idx;
	struct samsung_pin_bank *bank;
	void __iomem *reg_base;
	u32 pin_offset;
	unsigned long flags;
	enum pincfg_type type;
	u32 data[PINCFG_TYPE_NUM];
	const u8 *widths;
	const unsigned int sleep_type_mask = BIT(PINCFG_TYPE_DAT) |
		BIT(PINCFG_TYPE_CON_PDN) | BIT(PINCFG_TYPE_PUD_PDN);
	static int pin_num = 0;

	pin_to_reg_bank(drvdata, pin - drvdata->pin_base,
					&reg_base, &pin_offset, &bank);

	/* GPZ ports are AUD interface that should not access
	 * when AUD power is disabled
	 */
#if defined(CONFIG_SOC_EXYNOS7885)
	if (!strncmp(bank->name, "gpb", 3)) {
		sleep_gpio_idx++;
		goto out;
	}
#else
	if (!strncmp(bank->name, "gpz", 3)) {
		sleep_gpio_idx++;
		goto out;
	}
#endif	

	widths = bank->type->fld_width;
	if (widths[PINCFG_TYPE_CON_PDN]) {
		spin_lock_irqsave(&bank->slock, flags);
		for (type = PINCFG_TYPE_DAT; type <= PINCFG_TYPE_PUD_PDN; type++) {
			if (sleep_type_mask & BIT(type))
				data[type] = gpiodvs_get_by_type(bank, reg_base,
						pin_offset, pin, type);
		}
		spin_unlock_irqrestore(&bank->slock, flags);

		gpiomap_result.sleep[sleep_gpio_idx++] =
			gpiodvs_combine_data(data, PHONE_SLEEP);
	} else {
		/* Alive part */
		spin_lock_irqsave(&bank->slock, flags);
		for (type = PINCFG_TYPE_FUNC; type <= PINCFG_TYPE_PUD; type++)
			data[type] = gpiodvs_get_by_type(bank, reg_base, pin_offset,
					pin, type);
		spin_unlock_irqrestore(&bank->slock, flags);

		gpiomap_result.sleep[sleep_gpio_idx++] =
			gpiodvs_combine_data(data, PHONE_INIT);
	}

	gpiodvs_print_pin_state(PHONE_SLEEP, (widths[PINCFG_TYPE_CON_PDN] ? 0 : 1),
			bank->name, pin_num, data);
out:
	pin_num++;
	if (pin_num == bank->nr_pins) {
		pin_num = 0;
	}

	pr_debug("%s: sleep[%u]=0x%02x\n", __func__, sleep_gpio_idx - 1,
			gpiomap_result.sleep[sleep_gpio_idx - 1]);
}

static void gpiodvs_check_gpio_regs(
				struct samsung_pinctrl_drv_data *drvdata,
				unsigned char phonestate)
{
	int i, j;

	for (i = 0; i < drvdata->nr_groups; i++) {
		const unsigned int *pins = drvdata->pin_groups[i].pins;
		for (j = 0; j < drvdata->pin_groups[i].num_pins; j++) {
			if (phonestate  == PHONE_INIT)
				gpiodvs_check_init_gpio(drvdata, pins[j]);
			else
				gpiodvs_check_sleep_gpio(drvdata, pins[j]);
		}
	}
}

static void check_gpio_status(unsigned char phonestate)
{
	struct samsung_pinctrl_drv_data *drvdata;

	list_for_each_entry(drvdata, &drvdata_list, node) {
		gpiodvs_check_gpio_regs(drvdata, phonestate);
	}
}


struct gpio_dvs_t exynos7885_secgpio_dvs = {
	.result = &gpiomap_result,
	.check_gpio_status = check_gpio_status,
	.get_nr_gpio = exynos7885_secgpio_get_nr_gpio,
};
#endif

static const struct of_device_id samsung_pinctrl_dt_match[] = {
#ifdef CONFIG_PINCTRL_EXYNOS
	{ .compatible = "samsung,exynos3250-pinctrl",
		.data = (void *)exynos3250_pin_ctrl },
	{ .compatible = "samsung,exynos4210-pinctrl",
		.data = (void *)exynos4210_pin_ctrl },
	{ .compatible = "samsung,exynos4x12-pinctrl",
		.data = (void *)exynos4x12_pin_ctrl },
	{ .compatible = "samsung,exynos4415-pinctrl",
		.data = (void *)exynos4415_pin_ctrl },
	{ .compatible = "samsung,exynos5250-pinctrl",
		.data = (void *)exynos5250_pin_ctrl },
	{ .compatible = "samsung,exynos5260-pinctrl",
		.data = (void *)exynos5260_pin_ctrl },
	{ .compatible = "samsung,exynos5420-pinctrl",
		.data = (void *)exynos5420_pin_ctrl },
	{ .compatible = "samsung,exynos5433-pinctrl",
		.data = (void *)exynos5433_pin_ctrl },
	{ .compatible = "samsung,s5pv210-pinctrl",
		.data = (void *)s5pv210_pin_ctrl },
	{ .compatible = "samsung,exynos7-pinctrl",
		.data = (void *)exynos7_pin_ctrl },
	{ .compatible = "samsung,exynos7872-pinctrl",
		.data = (void *)exynos7872_pin_ctrl },
	{ .compatible = "samsung,exynos7885-pinctrl",
		.data = (void *)exynos7885_pin_ctrl },
	{ .compatible = "samsung,exynos8890-pinctrl",
		.data = (void *)exynos8890_pin_ctrl },
	{ .compatible = "samsung,exynos8895-pinctrl",
		.data = (void *)exynos8895_pin_ctrl },
#endif
#ifdef CONFIG_PINCTRL_S3C64XX
	{ .compatible = "samsung,s3c64xx-pinctrl",
		.data = s3c64xx_pin_ctrl },
#endif
#ifdef CONFIG_PINCTRL_S3C24XX
	{ .compatible = "samsung,s3c2412-pinctrl",
		.data = s3c2412_pin_ctrl },
	{ .compatible = "samsung,s3c2416-pinctrl",
		.data = s3c2416_pin_ctrl },
	{ .compatible = "samsung,s3c2440-pinctrl",
		.data = s3c2440_pin_ctrl },
	{ .compatible = "samsung,s3c2450-pinctrl",
		.data = s3c2450_pin_ctrl },
#endif
	{},
};
MODULE_DEVICE_TABLE(of, samsung_pinctrl_dt_match);

static struct platform_driver samsung_pinctrl_driver = {
	.probe		= samsung_pinctrl_probe,
	.driver = {
		.name	= "samsung-pinctrl",
		.of_match_table = samsung_pinctrl_dt_match,
		.suppress_bind_attrs = true,
	},
};

static int __init samsung_pinctrl_drv_register(void)
{
	/*
	 * Register syscore ops for save/restore of registers across suspend.
	 * It's important to ensure that this driver is running at an earlier
	 * initcall level than any arch-specific init calls that install syscore
	 * ops that turn off pad retention (like exynos_pm_resume).
	 */
	register_syscore_ops(&samsung_pinctrl_syscore_ops);

	return platform_driver_register(&samsung_pinctrl_driver);
}
postcore_initcall(samsung_pinctrl_drv_register);

static void __exit samsung_pinctrl_drv_unregister(void)
{
	platform_driver_unregister(&samsung_pinctrl_driver);
}
module_exit(samsung_pinctrl_drv_unregister);

MODULE_AUTHOR("Thomas Abraham <thomas.ab@samsung.com>");
MODULE_DESCRIPTION("Samsung pinctrl driver");
MODULE_LICENSE("GPL v2");
