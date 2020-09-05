/*
 * I2C bus interface to Cirrus Logic Madera codecs
 *
 * Copyright 2015 Cirrus Logic
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/err.h>
#include <linux/i2c.h>
#include <linux/module.h>
#include <linux/regmap.h>
#include <linux/slab.h>
#include <linux/of.h>

#include <linux/mfd/madera/core.h>

#include "madera.h"

static int madera_i2c_probe(struct i2c_client *i2c,
			  const struct i2c_device_id *id)
{
	struct madera *madera;
	const struct regmap_config *regmap_16bit_config = NULL;
	const struct regmap_config *regmap_32bit_config = NULL;
	unsigned long type;
	int ret;

	if (i2c->dev.of_node)
		type = madera_of_get_type(&i2c->dev);
	else
		type = id->driver_data;

	switch (type) {
	case CS47L35:
		if (IS_ENABLED(CONFIG_MFD_CS47L35)) {
			regmap_16bit_config = &cs47l35_16bit_i2c_regmap;
			regmap_32bit_config = &cs47l35_32bit_i2c_regmap;
		}
		break;
	case CS47L85:
	case WM1840:
		if (IS_ENABLED(CONFIG_MFD_CS47L85)) {
			regmap_16bit_config = &cs47l85_16bit_i2c_regmap;
			regmap_32bit_config = &cs47l85_32bit_i2c_regmap;
		}
		break;
	case CS47L90:
	case CS47L91:
		if (IS_ENABLED(CONFIG_MFD_CS47L90)) {
			regmap_16bit_config = &cs47l90_16bit_i2c_regmap;
			regmap_32bit_config = &cs47l90_32bit_i2c_regmap;
		}
		break;
	case CS47L92:
	case CS47L93:
		if (IS_ENABLED(CONFIG_MFD_CS47L92)) {
			regmap_16bit_config = &cs47l92_16bit_i2c_regmap;
			regmap_32bit_config = &cs47l92_32bit_i2c_regmap;
		}
		break;
	default:
		dev_err(&i2c->dev,
			"Unknown Madera I2C device type %ld\n", type);
		return -EINVAL;
	}

	if (!regmap_16bit_config) {
		dev_err(&i2c->dev,
			"Kernel does not include support for %s\n",
			madera_name_from_type(type));
		return -EINVAL;
	}

	madera = devm_kzalloc(&i2c->dev, sizeof(*madera), GFP_KERNEL);
	if (!madera)
		return -ENOMEM;

	madera->regmap = devm_regmap_init_i2c(i2c, regmap_16bit_config);
	if (IS_ERR(madera->regmap)) {
		ret = PTR_ERR(madera->regmap);
		dev_err(&i2c->dev,
			"Failed to allocate 16-bit register map: %d\n",	ret);
		return ret;
	}

	madera->regmap_32bit = devm_regmap_init_i2c(i2c, regmap_32bit_config);
	if (IS_ERR(madera->regmap_32bit)) {
		ret = PTR_ERR(madera->regmap_32bit);
		dev_err(&i2c->dev,
			"Failed to allocate 32-bit register map: %d\n", ret);
		return ret;
	}

	madera->type = type;
	madera->dev = &i2c->dev;
	madera->irq = i2c->irq;

	return madera_dev_init(madera);
}

static int madera_i2c_remove(struct i2c_client *i2c)
{
	struct madera *madera = dev_get_drvdata(&i2c->dev);

	madera_dev_exit(madera);
	return 0;
}

static const struct i2c_device_id madera_i2c_id[] = {
	{ "cs47l35", CS47L35 },
	{ "cs47l85", CS47L85 },
	{ "cs47l90", CS47L90 },
	{ "cs47l91", CS47L91 },
	{ "wm1840", WM1840 },
	{ "cs47l92", CS47L92 },
	{ "cs47l93", CS47L93 },
	{ }
};
MODULE_DEVICE_TABLE(i2c, madera_i2c_id);

static struct i2c_driver madera_i2c_driver = {
	.driver = {
		.name	= "madera",
		.owner	= THIS_MODULE,
		.pm	= &madera_pm_ops,
		.of_match_table	= of_match_ptr(madera_of_match),
	},
	.probe		= madera_i2c_probe,
	.remove		= madera_i2c_remove,
	.id_table	= madera_i2c_id,
};

module_i2c_driver(madera_i2c_driver);

MODULE_DESCRIPTION("Madera I2C bus interface");
MODULE_AUTHOR("Richard Fitzgerald <rf@opensource.wolfsonmicro.com>");
MODULE_LICENSE("GPL v2");
