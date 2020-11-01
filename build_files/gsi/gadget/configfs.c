#include <linux/configfs.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/nls.h>
#include <linux/usb/composite.h>
#include <linux/usb/gadget_configfs.h>
#include "configfs.h"
#include "u_f.h"
#include "u_os_desc.h"
#include <linux/soc/samsung/exynos-soc.h>

#ifdef CONFIG_USB_CONFIGFS_UEVENT
#include <linux/platform_device.h>
#include <linux/kdev_t.h>
#include <linux/usb/ch9.h>

#ifdef CONFIG_USB_CONFIGFS_F_ACC
extern int acc_ctrlrequest(struct usb_composite_dev *cdev,
				const struct usb_ctrlrequest *ctrl);
void acc_disconnect(void);
#endif
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE

#include "function/u_ether.h"
extern int terminal_ctrl_request(struct usb_composite_dev *cdev,
				const struct usb_ctrlrequest *ctrl);
#endif
static struct class *android_class;
static struct device *android_device;
static int index;

struct device *create_function_device(char *name)
{
	if (android_device && !IS_ERR(android_device))
	{
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
		if (!strcmp(name, "terminal_version")) 
			return android_device;
		else
#endif			
		{
			return device_create(android_class, android_device,
				MKDEV(0, index++), NULL, name);
		}
	}	
	else
		return ERR_PTR(-EINVAL);
}
EXPORT_SYMBOL_GPL(create_function_device);
#endif

#ifdef CONFIG_USB_TYPEC_MANAGER_NOTIFIER
void set_usb_enumeration_state(int state);
void set_usb_enable_state(void);
#else
static int usb_enum_state;
static int usb210_count;
static int usb310_count;
void set_usb_enumeration_state(int state)
{
	if(usb_enum_state != state) {
		usb_enum_state = state;
		if(usb_enum_state == 0x310)
			usb310_count++;
		else if(usb_enum_state == 0x210)
			usb210_count++;
	}
}

int microusb_get_usb210_count(void)
{
	int ret;
	ret = usb210_count;
	usb210_count = 0;
	return ret;
}
EXPORT_SYMBOL(microusb_get_usb210_count);

int microusb_get_usb310_count(void)
{
	int ret;
	ret = usb310_count;
	usb310_count = 0;
	return ret;
}
EXPORT_SYMBOL(microusb_get_usb310_count);

bool get_usb_enumeration_state(void)
{
	return usb_enum_state? 1: 0;
}
EXPORT_SYMBOL(get_usb_enumeration_state);
#endif

#define CHIPID_SIZE	(16)

int check_user_usb_string(const char *name,
		struct usb_gadget_strings *stringtab_dev)
{
	unsigned primary_lang;
	unsigned sub_lang;
	u16 num;
	int ret;

	ret = kstrtou16(name, 0, &num);
	if (ret)
		return ret;

	primary_lang = num & 0x3ff;
	sub_lang = num >> 10;

	/* simple sanity check for valid langid */
	switch (primary_lang) {
	case 0:
	case 0x62 ... 0xfe:
	case 0x100 ... 0x3ff:
		return -EINVAL;
	}
	if (!sub_lang)
		return -EINVAL;

	stringtab_dev->language = num;
	return 0;
}

#define MAX_NAME_LEN	40
#define MAX_USB_STRING_LANGS 2
#define MAX_USB_STRING_LEN	126
#define MAX_USB_STRING_WITH_NULL_LEN	(MAX_USB_STRING_LEN+1)

static const struct usb_descriptor_header *otg_desc[2];

struct gadget_info {
	struct config_group group;
	struct config_group functions_group;
	struct config_group configs_group;
	struct config_group strings_group;
	struct config_group os_desc_group;
	struct config_group *default_groups[5];

	struct mutex lock;
	struct usb_gadget_strings *gstrings[MAX_USB_STRING_LANGS + 1];
	struct list_head string_list;
	struct list_head available_func;

	const char *udc_name;
	struct usb_composite_driver composite;
	struct usb_composite_dev cdev;
	bool use_os_desc;
	char b_vendor_code;
	char qw_sign[OS_STRING_QW_SIGN_LEN];
	spinlock_t spinlock;
	bool unbind;
#ifdef CONFIG_USB_CONFIGFS_UEVENT
	bool enabled;
	bool connected;
	bool sw_connected;
	struct work_struct work;
	struct device *dev;
	struct list_head linked_func;
	char *prev_func_list;
#endif
};

static inline struct gadget_info *to_gadget_info(struct config_item *item)
{
	 return container_of(to_config_group(item), struct gadget_info, group);
}

struct config_usb_cfg {
	struct config_group group;
	struct config_group strings_group;
	struct config_group *default_groups[2];
	struct list_head string_list;
	struct usb_configuration c;
	struct list_head func_list;
	struct usb_gadget_strings *gstrings[MAX_USB_STRING_LANGS + 1];
};

static inline struct config_usb_cfg *to_config_usb_cfg(struct config_item *item)
{
	return container_of(to_config_group(item), struct config_usb_cfg,
			group);
}

struct gadget_strings {
	struct usb_gadget_strings stringtab_dev;
	struct usb_string strings[USB_GADGET_FIRST_AVAIL_IDX];
	char *manufacturer;
	char *product;
	char *serialnumber;

	struct config_group group;
	struct list_head list;
};

struct os_desc {
	struct config_group group;
};

struct gadget_config_name {
	struct usb_gadget_strings stringtab_dev;
	struct usb_string strings;
	char *configuration;

	struct config_group group;
	struct list_head list;
};

static int usb_string_copy(const char *s, char **s_copy)
{
	int ret;
	char *str;
	char *copy = *s_copy;
	ret = strlen(s);
	if (ret > MAX_USB_STRING_LEN)
		return -EOVERFLOW;
	if (copy) {
		str = copy;
	} else {
		str = kmalloc(MAX_USB_STRING_WITH_NULL_LEN, GFP_KERNEL);
		if (!str)
			return -ENOMEM;
	}
	strncpy(str, s, MAX_USB_STRING_WITH_NULL_LEN);

	if (str[ret - 1] == '\n')
		str[ret - 1] = '\0';

	*s_copy = str;
	return 0;
}
#ifndef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
static int set_alt_serialnumber(struct gadget_strings *gs)
{
	char *str;
	int ret = -ENOMEM;

	str = kmalloc(CHIPID_SIZE + 1, GFP_KERNEL);
	if (!str) {
		pr_err("%s: failed to alloc for string\n", __func__);
		return ret;
	}

	snprintf(str, CHIPID_SIZE + 1, "%016lx", (long)exynos_soc_info.unique_id);
	if (usb_string_copy(str, &gs->serialnumber))
		pr_err("%s: failed to copy alternative string\n", __func__);
	else
		ret = 0;

	kfree(str);
	return ret;
}
#endif
#define GI_DEVICE_DESC_SIMPLE_R_u8(__name)	\
static ssize_t gadget_dev_desc_##__name##_show(struct config_item *item, \
			char *page)	\
{	\
	return sprintf(page, "0x%02x\n", \
		to_gadget_info(item)->cdev.desc.__name); \
}

#define GI_DEVICE_DESC_SIMPLE_R_u16(__name)	\
static ssize_t gadget_dev_desc_##__name##_show(struct config_item *item, \
			char *page)	\
{	\
	return sprintf(page, "0x%04x\n", \
		le16_to_cpup(&to_gadget_info(item)->cdev.desc.__name)); \
}


#define GI_DEVICE_DESC_SIMPLE_W_u8(_name)		\
static ssize_t gadget_dev_desc_##_name##_store(struct config_item *item, \
		const char *page, size_t len)		\
{							\
	u8 val;						\
	int ret;					\
	ret = kstrtou8(page, 0, &val);			\
	if (ret)					\
		return ret;				\
	to_gadget_info(item)->cdev.desc._name = val;	\
	return len;					\
}

#define GI_DEVICE_DESC_SIMPLE_W_u16(_name)	\
static ssize_t gadget_dev_desc_##_name##_store(struct config_item *item, \
		const char *page, size_t len)		\
{							\
	u16 val;					\
	int ret;					\
	ret = kstrtou16(page, 0, &val);			\
	if (ret)					\
		return ret;				\
	to_gadget_info(item)->cdev.desc._name = cpu_to_le16p(&val);	\
	return len;					\
}

#define GI_DEVICE_DESC_SIMPLE_RW(_name, _type)	\
	GI_DEVICE_DESC_SIMPLE_R_##_type(_name)	\
	GI_DEVICE_DESC_SIMPLE_W_##_type(_name)

GI_DEVICE_DESC_SIMPLE_R_u16(bcdUSB);
GI_DEVICE_DESC_SIMPLE_RW(bDeviceClass, u8);
GI_DEVICE_DESC_SIMPLE_RW(bDeviceSubClass, u8);
GI_DEVICE_DESC_SIMPLE_RW(bDeviceProtocol, u8);
GI_DEVICE_DESC_SIMPLE_RW(bMaxPacketSize0, u8);
GI_DEVICE_DESC_SIMPLE_RW(idVendor, u16);
GI_DEVICE_DESC_SIMPLE_RW(idProduct, u16);
GI_DEVICE_DESC_SIMPLE_R_u16(bcdDevice);

static ssize_t is_valid_bcd(u16 bcd_val)
{
	if ((bcd_val & 0xf) > 9)
		return -EINVAL;
	if (((bcd_val >> 4) & 0xf) > 9)
		return -EINVAL;
	if (((bcd_val >> 8) & 0xf) > 9)
		return -EINVAL;
	if (((bcd_val >> 12) & 0xf) > 9)
		return -EINVAL;
	return 0;
}

static ssize_t gadget_dev_desc_bcdDevice_store(struct config_item *item,
		const char *page, size_t len)
{
	u16 bcdDevice;
	int ret;

	ret = kstrtou16(page, 0, &bcdDevice);
	if (ret)
		return ret;
	ret = is_valid_bcd(bcdDevice);
	if (ret)
		return ret;

	to_gadget_info(item)->cdev.desc.bcdDevice = cpu_to_le16(bcdDevice);
	return len;
}

static ssize_t gadget_dev_desc_bcdUSB_store(struct config_item *item,
		const char *page, size_t len)
{
	u16 bcdUSB;
	int ret;

	ret = kstrtou16(page, 0, &bcdUSB);
	if (ret)
		return ret;
	ret = is_valid_bcd(bcdUSB);
	if (ret)
		return ret;

	to_gadget_info(item)->cdev.desc.bcdUSB = cpu_to_le16(bcdUSB);
	return len;
}

static ssize_t gadget_dev_desc_UDC_show(struct config_item *item, char *page)
{
	return sprintf(page, "%s\n", to_gadget_info(item)->udc_name ?: "");
}

static int unregister_gadget(struct gadget_info *gi)
{
	int ret;

	if (!gi->udc_name)
		return -ENODEV;

	ret = usb_gadget_unregister_driver(&gi->composite.gadget_driver);
	if (ret)
		return ret;
	kfree(gi->udc_name);
	gi->udc_name = NULL;
	return 0;
}

static ssize_t gadget_dev_desc_UDC_store(struct config_item *item,
		const char *page, size_t len)
{
	struct gadget_info *gi = to_gadget_info(item);
	char *name;
	int ret;

	/* HACK: ffs_ep0_write must be called before UDC store in adb,
	 * but sometimes UDC store was executed before ffs_ep0_write.
	 * Delay was added to avoid the problem
	 */
	pr_info("%s: +++\n", __func__);

	if (strlen(page) < len)
		return -EOVERFLOW;

	name = kstrdup(page, GFP_KERNEL);
	if (!name)
		return -ENOMEM;
	if (name[len - 1] == '\n')
		name[len - 1] = '\0';

	mutex_lock(&gi->lock);

	if (!strlen(name) || strcmp(name, "none") == 0) {
		ret = unregister_gadget(gi);
		if (ret)
			goto err;
		/* prevent memory leak */
		kfree(name);
	} else {
		if (gi->udc_name) {
			ret = -EBUSY;
			goto err;
		}
		ret = usb_udc_attach_driver(name, &gi->composite.gadget_driver);
		if (ret)
			goto err;
		gi->udc_name = name;
	}
	mutex_unlock(&gi->lock);
	pr_info("%s: ---\n", __func__);

	return len;
err:
	kfree(name);
	mutex_unlock(&gi->lock);
	return ret;
}

CONFIGFS_ATTR(gadget_dev_desc_, bDeviceClass);
CONFIGFS_ATTR(gadget_dev_desc_, bDeviceSubClass);
CONFIGFS_ATTR(gadget_dev_desc_, bDeviceProtocol);
CONFIGFS_ATTR(gadget_dev_desc_, bMaxPacketSize0);
CONFIGFS_ATTR(gadget_dev_desc_, idVendor);
CONFIGFS_ATTR(gadget_dev_desc_, idProduct);
CONFIGFS_ATTR(gadget_dev_desc_, bcdDevice);
CONFIGFS_ATTR(gadget_dev_desc_, bcdUSB);
CONFIGFS_ATTR(gadget_dev_desc_, UDC);

static struct configfs_attribute *gadget_root_attrs[] = {
	&gadget_dev_desc_attr_bDeviceClass,
	&gadget_dev_desc_attr_bDeviceSubClass,
	&gadget_dev_desc_attr_bDeviceProtocol,
	&gadget_dev_desc_attr_bMaxPacketSize0,
	&gadget_dev_desc_attr_idVendor,
	&gadget_dev_desc_attr_idProduct,
	&gadget_dev_desc_attr_bcdDevice,
	&gadget_dev_desc_attr_bcdUSB,
	&gadget_dev_desc_attr_UDC,
	NULL,
};

static inline struct gadget_strings *to_gadget_strings(struct config_item *item)
{
	 return container_of(to_config_group(item), struct gadget_strings,
			 group);
}

static inline struct gadget_config_name *to_gadget_config_name(
		struct config_item *item)
{
	 return container_of(to_config_group(item), struct gadget_config_name,
			 group);
}

static inline struct usb_function_instance *to_usb_function_instance(
		struct config_item *item)
{
	 return container_of(to_config_group(item),
			 struct usb_function_instance, group);
}

static void gadget_info_attr_release(struct config_item *item)
{
	struct gadget_info *gi = to_gadget_info(item);

	WARN_ON(!list_empty(&gi->cdev.configs));
	WARN_ON(!list_empty(&gi->string_list));
	WARN_ON(!list_empty(&gi->available_func));
	kfree(gi->composite.gadget_driver.function);
	kfree(gi);
}

static struct configfs_item_operations gadget_root_item_ops = {
	.release                = gadget_info_attr_release,
};

static void gadget_config_attr_release(struct config_item *item)
{
	struct config_usb_cfg *cfg = to_config_usb_cfg(item);

	WARN_ON(!list_empty(&cfg->c.functions));
	list_del(&cfg->c.list);
	kfree(cfg->c.label);
	kfree(cfg);
}

static int config_usb_cfg_link(
	struct config_item *usb_cfg_ci,
	struct config_item *usb_func_ci)
{
	struct config_usb_cfg *cfg = to_config_usb_cfg(usb_cfg_ci);
	struct usb_composite_dev *cdev = cfg->c.cdev;
	struct gadget_info *gi = container_of(cdev, struct gadget_info, cdev);

	struct config_group *group = to_config_group(usb_func_ci);
	struct usb_function_instance *fi = container_of(group,
			struct usb_function_instance, group);
	struct usb_function_instance *a_fi;
	struct usb_function *f;
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	static u8      ethaddr[ETH_ALEN]={0,};
	int i;
	char *src;
	struct gadget_strings *gs;
#endif
	int ret;

	mutex_lock(&gi->lock);
	/*
	 * Make sure this function is from within our _this_ gadget and not
	 * from another gadget or a random directory.
	 * Also a function instance can only be linked once.
	 */
	list_for_each_entry(a_fi, &gi->available_func, cfs_list) {
		if (a_fi == fi)
			break;
	}
	if (a_fi != fi) {
		ret = -EINVAL;
		goto out;
	}

	list_for_each_entry(f, &cfg->func_list, list) {
		if (f->fi == fi) {
			ret = -EEXIST;
			goto out;
		}
	}
	/* usb tethering */
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	if (fi->set_inst_eth_addr) {
		list_for_each_entry(gs, &gi->string_list, list) {
			src = gs->serialnumber;
		}

		if (src) {
			for (i = 0; i < ETH_ALEN; i++)
				ethaddr[i] = 0;
			/* create a fake MAC address from our serial number.
			 * first byte is 0x02 to signify locally administered.
			 */
			ethaddr[0] = 0x02;
			for (i = 0; (i < 256) && *src; i++) {
				/* XOR the USB serial across the remaining bytes */
				ethaddr[i % (ETH_ALEN - 1) + 1] ^= *src++;
			}

			fi->set_inst_eth_addr(fi, ethaddr);
		}
	}
#endif

	f = usb_get_function(fi);
	if (f == NULL) {
		/* Are we trying to symlink PTP without MTP function? */
		ret = -EINVAL; /* Invalid Configuration */
		goto out;
	}
	if (IS_ERR(f)) {
		ret = PTR_ERR(f);
		goto out;
	}

#ifdef CONFIG_USB_CONFIGFS_UEVENT
	list_add_tail(&f->list, &gi->linked_func);
#else
	/* stash the function until we bind it to the gadget */
	list_add_tail(&f->list, &cfg->func_list);
#endif
	ret = 0;
out:
	mutex_unlock(&gi->lock);
	return ret;
}

static int config_usb_cfg_unlink(
	struct config_item *usb_cfg_ci,
	struct config_item *usb_func_ci)
{
	struct config_usb_cfg *cfg = to_config_usb_cfg(usb_cfg_ci);
	struct usb_composite_dev *cdev = cfg->c.cdev;
	struct gadget_info *gi = container_of(cdev, struct gadget_info, cdev);

	struct config_group *group = to_config_group(usb_func_ci);
	struct usb_function_instance *fi = container_of(group,
			struct usb_function_instance, group);
	struct usb_function *f;

	/*
	 * ideally I would like to forbid to unlink functions while a gadget is
	 * bound to an UDC. Since this isn't possible at the moment, we simply
	 * force an unbind, the function is available here and then we can
	 * remove the function.
	 */
	mutex_lock(&gi->lock);
	if (gi->udc_name)
		unregister_gadget(gi);
	WARN_ON(gi->udc_name);

	list_for_each_entry(f, &cfg->func_list, list) {
		if (f->fi == fi) {
			list_del(&f->list);
			usb_put_function(f);
			mutex_unlock(&gi->lock);
			return 0;
		}
	}
	mutex_unlock(&gi->lock);
	WARN(1, "Unable to locate function to unbind\n");
	return 0;
}

static struct configfs_item_operations gadget_config_item_ops = {
	.release                = gadget_config_attr_release,
	.allow_link             = config_usb_cfg_link,
	.drop_link              = config_usb_cfg_unlink,
};


static ssize_t gadget_config_desc_MaxPower_show(struct config_item *item,
		char *page)
{
	return sprintf(page, "%u\n", to_config_usb_cfg(item)->c.MaxPower);
}

static ssize_t gadget_config_desc_MaxPower_store(struct config_item *item,
		const char *page, size_t len)
{
	u16 val;
	int ret;
	ret = kstrtou16(page, 0, &val);
	if (ret)
		return ret;
	if (DIV_ROUND_UP(val, 8) > 0xff)
		return -ERANGE;
	to_config_usb_cfg(item)->c.MaxPower = val;
	return len;
}

static ssize_t gadget_config_desc_bmAttributes_show(struct config_item *item,
		char *page)
{
	return sprintf(page, "0x%02x\n",
		to_config_usb_cfg(item)->c.bmAttributes);
}

static ssize_t gadget_config_desc_bmAttributes_store(struct config_item *item,
		const char *page, size_t len)
{
	u8 val;
	int ret;
	ret = kstrtou8(page, 0, &val);
	if (ret)
		return ret;
	if (!(val & USB_CONFIG_ATT_ONE))
		return -EINVAL;
	if (val & ~(USB_CONFIG_ATT_ONE | USB_CONFIG_ATT_SELFPOWER |
				USB_CONFIG_ATT_WAKEUP))
		return -EINVAL;
	to_config_usb_cfg(item)->c.bmAttributes = val;
	return len;
}

CONFIGFS_ATTR(gadget_config_desc_, MaxPower);
CONFIGFS_ATTR(gadget_config_desc_, bmAttributes);

static struct configfs_attribute *gadget_config_attrs[] = {
	&gadget_config_desc_attr_MaxPower,
	&gadget_config_desc_attr_bmAttributes,
	NULL,
};

static struct config_item_type gadget_config_type = {
	.ct_item_ops	= &gadget_config_item_ops,
	.ct_attrs	= gadget_config_attrs,
	.ct_owner	= THIS_MODULE,
};

static struct config_item_type gadget_root_type = {
	.ct_item_ops	= &gadget_root_item_ops,
	.ct_attrs	= gadget_root_attrs,
	.ct_owner	= THIS_MODULE,
};

static void composite_init_dev(struct usb_composite_dev *cdev)
{
	spin_lock_init(&cdev->lock);
	INIT_LIST_HEAD(&cdev->configs);
	INIT_LIST_HEAD(&cdev->gstrings);
}

static struct config_group *function_make(
		struct config_group *group,
		const char *name)
{
	struct gadget_info *gi;
	struct usb_function_instance *fi;
	char buf[MAX_NAME_LEN];
	char *func_name;
	char *instance_name;
	int ret;

	ret = snprintf(buf, MAX_NAME_LEN, "%s", name);
	if (ret >= MAX_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	func_name = buf;
	instance_name = strchr(func_name, '.');
	if (!instance_name) {
		pr_err("Unable to locate . in FUNC.INSTANCE\n");
		return ERR_PTR(-EINVAL);
	}
	*instance_name = '\0';
	instance_name++;

	fi = usb_get_function_instance(func_name);
	if (IS_ERR(fi))
		return ERR_CAST(fi);

	ret = config_item_set_name(&fi->group.cg_item, "%s", name);
	if (ret) {
		usb_put_function_instance(fi);
		return ERR_PTR(ret);
	}
	if (fi->set_inst_name) {
		ret = fi->set_inst_name(fi, instance_name);
		if (ret) {
			usb_put_function_instance(fi);
			return ERR_PTR(ret);
		}
	}

	gi = container_of(group, struct gadget_info, functions_group);

	mutex_lock(&gi->lock);
	list_add_tail(&fi->cfs_list, &gi->available_func);
	mutex_unlock(&gi->lock);
	return &fi->group;
}

static void function_drop(
		struct config_group *group,
		struct config_item *item)
{
	struct usb_function_instance *fi = to_usb_function_instance(item);
	struct gadget_info *gi;

	gi = container_of(group, struct gadget_info, functions_group);

	mutex_lock(&gi->lock);
	list_del(&fi->cfs_list);
	mutex_unlock(&gi->lock);
	config_item_put(item);
}

static struct configfs_group_operations functions_ops = {
	.make_group     = &function_make,
	.drop_item      = &function_drop,
};

static struct config_item_type functions_type = {
	.ct_group_ops   = &functions_ops,
	.ct_owner       = THIS_MODULE,
};

GS_STRINGS_RW(gadget_config_name, configuration);

static struct configfs_attribute *gadget_config_name_langid_attrs[] = {
	&gadget_config_name_attr_configuration,
	NULL,
};

static void gadget_config_name_attr_release(struct config_item *item)
{
	struct gadget_config_name *cn = to_gadget_config_name(item);

	kfree(cn->configuration);

	list_del(&cn->list);
	kfree(cn);
}

USB_CONFIG_STRING_RW_OPS(gadget_config_name);
USB_CONFIG_STRINGS_LANG(gadget_config_name, config_usb_cfg);

static struct config_group *config_desc_make(
		struct config_group *group,
		const char *name)
{
	struct gadget_info *gi;
	struct config_usb_cfg *cfg;
	char buf[MAX_NAME_LEN];
	char *num_str;
	u8 num;
	int ret;

	gi = container_of(group, struct gadget_info, configs_group);
	ret = snprintf(buf, MAX_NAME_LEN, "%s", name);
	if (ret >= MAX_NAME_LEN)
		return ERR_PTR(-ENAMETOOLONG);

	num_str = strchr(buf, '.');
	if (!num_str) {
		pr_err("Unable to locate . in name.bConfigurationValue\n");
		return ERR_PTR(-EINVAL);
	}

	*num_str = '\0';
	num_str++;

	if (!strlen(buf))
		return ERR_PTR(-EINVAL);

	ret = kstrtou8(num_str, 0, &num);
	if (ret)
		return ERR_PTR(ret);

	cfg = kzalloc(sizeof(*cfg), GFP_KERNEL);
	if (!cfg)
		return ERR_PTR(-ENOMEM);
	cfg->c.label = kstrdup(buf, GFP_KERNEL);
	if (!cfg->c.label) {
		ret = -ENOMEM;
		goto err;
	}
	cfg->c.bConfigurationValue = num;
	cfg->c.MaxPower = CONFIG_USB_GADGET_VBUS_DRAW;
	cfg->c.bmAttributes = USB_CONFIG_ATT_ONE;
	INIT_LIST_HEAD(&cfg->string_list);
	INIT_LIST_HEAD(&cfg->func_list);

	cfg->group.default_groups = cfg->default_groups;
	cfg->default_groups[0] = &cfg->strings_group;

	config_group_init_type_name(&cfg->group, name,
				&gadget_config_type);
	config_group_init_type_name(&cfg->strings_group, "strings",
			&gadget_config_name_strings_type);

	ret = usb_add_config_only(&gi->cdev, &cfg->c);
	if (ret)
		goto err;

	return &cfg->group;
err:
	kfree(cfg->c.label);
	kfree(cfg);
	return ERR_PTR(ret);
}

static void config_desc_drop(
		struct config_group *group,
		struct config_item *item)
{
	config_item_put(item);
}

static struct configfs_group_operations config_desc_ops = {
	.make_group     = &config_desc_make,
	.drop_item      = &config_desc_drop,
};

static struct config_item_type config_desc_type = {
	.ct_group_ops   = &config_desc_ops,
	.ct_owner       = THIS_MODULE,
};

GS_STRINGS_RW(gadget_strings, manufacturer);
GS_STRINGS_RW(gadget_strings, product);
GS_STRINGS_RW(gadget_strings, serialnumber);

static struct configfs_attribute *gadget_strings_langid_attrs[] = {
	&gadget_strings_attr_manufacturer,
	&gadget_strings_attr_product,
	&gadget_strings_attr_serialnumber,
	NULL,
};

static void gadget_strings_attr_release(struct config_item *item)
{
	struct gadget_strings *gs = to_gadget_strings(item);

	kfree(gs->manufacturer);
	kfree(gs->product);
	kfree(gs->serialnumber);

	list_del(&gs->list);
	kfree(gs);
}

USB_CONFIG_STRING_RW_OPS(gadget_strings);
USB_CONFIG_STRINGS_LANG(gadget_strings, gadget_info);

static inline struct os_desc *to_os_desc(struct config_item *item)
{
	return container_of(to_config_group(item), struct os_desc, group);
}

static inline struct gadget_info *os_desc_item_to_gadget_info(
		struct config_item *item)
{
	return to_gadget_info(to_os_desc(item)->group.cg_item.ci_parent);
}

static ssize_t os_desc_use_show(struct config_item *item, char *page)
{
	return sprintf(page, "%d",
			os_desc_item_to_gadget_info(item)->use_os_desc);
}

static ssize_t os_desc_use_store(struct config_item *item, const char *page,
				 size_t len)
{
	struct gadget_info *gi = os_desc_item_to_gadget_info(item);
	int ret;
	bool use;

	mutex_lock(&gi->lock);
	ret = strtobool(page, &use);
	if (!ret) {
		gi->use_os_desc = use;
		ret = len;
	}
	mutex_unlock(&gi->lock);

	return ret;
}

static ssize_t os_desc_b_vendor_code_show(struct config_item *item, char *page)
{
	return sprintf(page, "%d",
			os_desc_item_to_gadget_info(item)->b_vendor_code);
}

static ssize_t os_desc_b_vendor_code_store(struct config_item *item,
					   const char *page, size_t len)
{
	struct gadget_info *gi = os_desc_item_to_gadget_info(item);
	int ret;
	u8 b_vendor_code;

	mutex_lock(&gi->lock);
	ret = kstrtou8(page, 0, &b_vendor_code);
	if (!ret) {
		gi->b_vendor_code = b_vendor_code;
		ret = len;
	}
	mutex_unlock(&gi->lock);

	return ret;
}

static ssize_t os_desc_qw_sign_show(struct config_item *item, char *page)
{
	struct gadget_info *gi = os_desc_item_to_gadget_info(item);

	memcpy(page, gi->qw_sign, OS_STRING_QW_SIGN_LEN);
	return OS_STRING_QW_SIGN_LEN;
}

static ssize_t os_desc_qw_sign_store(struct config_item *item, const char *page,
				     size_t len)
{
	struct gadget_info *gi = os_desc_item_to_gadget_info(item);
	int res, l;

	l = min((int)len, OS_STRING_QW_SIGN_LEN >> 1);
	if (page[l - 1] == '\n')
		--l;

	mutex_lock(&gi->lock);
	res = utf8s_to_utf16s(page, l,
			      UTF16_LITTLE_ENDIAN, (wchar_t *) gi->qw_sign,
			      OS_STRING_QW_SIGN_LEN);
	if (res > 0)
		res = len;
	mutex_unlock(&gi->lock);

	return res;
}

CONFIGFS_ATTR(os_desc_, use);
CONFIGFS_ATTR(os_desc_, b_vendor_code);
CONFIGFS_ATTR(os_desc_, qw_sign);

static struct configfs_attribute *os_desc_attrs[] = {
	&os_desc_attr_use,
	&os_desc_attr_b_vendor_code,
	&os_desc_attr_qw_sign,
	NULL,
};

static void os_desc_attr_release(struct config_item *item)
{
	struct os_desc *os_desc = to_os_desc(item);
	kfree(os_desc);
}

static int os_desc_link(struct config_item *os_desc_ci,
			struct config_item *usb_cfg_ci)
{
	struct gadget_info *gi = container_of(to_config_group(os_desc_ci),
					struct gadget_info, os_desc_group);
	struct usb_composite_dev *cdev = &gi->cdev;
	struct config_usb_cfg *c_target =
		container_of(to_config_group(usb_cfg_ci),
			     struct config_usb_cfg, group);
	struct usb_configuration *c;
	int ret;

	mutex_lock(&gi->lock);
	list_for_each_entry(c, &cdev->configs, list) {
		if (c == &c_target->c)
			break;
	}
	if (c != &c_target->c) {
		ret = -EINVAL;
		goto out;
	}

	if (cdev->os_desc_config) {
		ret = -EBUSY;
		goto out;
	}

	cdev->os_desc_config = &c_target->c;
	ret = 0;

out:
	mutex_unlock(&gi->lock);
	return ret;
}

static int os_desc_unlink(struct config_item *os_desc_ci,
			  struct config_item *usb_cfg_ci)
{
	struct gadget_info *gi = container_of(to_config_group(os_desc_ci),
					struct gadget_info, os_desc_group);
	struct usb_composite_dev *cdev = &gi->cdev;

	mutex_lock(&gi->lock);
	if (gi->udc_name)
		unregister_gadget(gi);
	cdev->os_desc_config = NULL;
	WARN_ON(gi->udc_name);
	mutex_unlock(&gi->lock);
	return 0;
}

static struct configfs_item_operations os_desc_ops = {
	.release                = os_desc_attr_release,
	.allow_link		= os_desc_link,
	.drop_link		= os_desc_unlink,
};

static struct config_item_type os_desc_type = {
	.ct_item_ops	= &os_desc_ops,
	.ct_attrs	= os_desc_attrs,
	.ct_owner	= THIS_MODULE,
};

static inline struct usb_os_desc_ext_prop
*to_usb_os_desc_ext_prop(struct config_item *item)
{
	return container_of(item, struct usb_os_desc_ext_prop, item);
}

static ssize_t ext_prop_type_show(struct config_item *item, char *page)
{
	return sprintf(page, "%d", to_usb_os_desc_ext_prop(item)->type);
}

static ssize_t ext_prop_type_store(struct config_item *item,
				   const char *page, size_t len)
{
	struct usb_os_desc_ext_prop *ext_prop = to_usb_os_desc_ext_prop(item);
	struct usb_os_desc *desc = to_usb_os_desc(ext_prop->item.ci_parent);
	u8 type;
	int ret;

	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	ret = kstrtou8(page, 0, &type);
	if (ret)
		goto end;
	if (type < USB_EXT_PROP_UNICODE || type > USB_EXT_PROP_UNICODE_MULTI) {
		ret = -EINVAL;
		goto end;
	}

	if ((ext_prop->type == USB_EXT_PROP_BINARY ||
	    ext_prop->type == USB_EXT_PROP_LE32 ||
	    ext_prop->type == USB_EXT_PROP_BE32) &&
	    (type == USB_EXT_PROP_UNICODE ||
	    type == USB_EXT_PROP_UNICODE_ENV ||
	    type == USB_EXT_PROP_UNICODE_LINK))
		ext_prop->data_len <<= 1;
	else if ((ext_prop->type == USB_EXT_PROP_UNICODE ||
		   ext_prop->type == USB_EXT_PROP_UNICODE_ENV ||
		   ext_prop->type == USB_EXT_PROP_UNICODE_LINK) &&
		   (type == USB_EXT_PROP_BINARY ||
		   type == USB_EXT_PROP_LE32 ||
		   type == USB_EXT_PROP_BE32))
		ext_prop->data_len >>= 1;
	ext_prop->type = type;
	ret = len;

end:
	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);
	return ret;
}

static ssize_t ext_prop_data_show(struct config_item *item, char *page)
{
	struct usb_os_desc_ext_prop *ext_prop = to_usb_os_desc_ext_prop(item);
	int len = ext_prop->data_len;

	if (ext_prop->type == USB_EXT_PROP_UNICODE ||
	    ext_prop->type == USB_EXT_PROP_UNICODE_ENV ||
	    ext_prop->type == USB_EXT_PROP_UNICODE_LINK)
		len >>= 1;
	memcpy(page, ext_prop->data, len);

	return len;
}

static ssize_t ext_prop_data_store(struct config_item *item,
				   const char *page, size_t len)
{
	struct usb_os_desc_ext_prop *ext_prop = to_usb_os_desc_ext_prop(item);
	struct usb_os_desc *desc = to_usb_os_desc(ext_prop->item.ci_parent);
	char *new_data;
	size_t ret_len = len;

	if (page[len - 1] == '\n' || page[len - 1] == '\0')
		--len;
	new_data = kmemdup(page, len, GFP_KERNEL);
	if (!new_data)
		return -ENOMEM;

	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	kfree(ext_prop->data);
	ext_prop->data = new_data;
	desc->ext_prop_len -= ext_prop->data_len;
	ext_prop->data_len = len;
	desc->ext_prop_len += ext_prop->data_len;
	if (ext_prop->type == USB_EXT_PROP_UNICODE ||
	    ext_prop->type == USB_EXT_PROP_UNICODE_ENV ||
	    ext_prop->type == USB_EXT_PROP_UNICODE_LINK) {
		desc->ext_prop_len -= ext_prop->data_len;
		ext_prop->data_len <<= 1;
		ext_prop->data_len += 2;
		desc->ext_prop_len += ext_prop->data_len;
	}
	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);
	return ret_len;
}

CONFIGFS_ATTR(ext_prop_, type);
CONFIGFS_ATTR(ext_prop_, data);

static struct configfs_attribute *ext_prop_attrs[] = {
	&ext_prop_attr_type,
	&ext_prop_attr_data,
	NULL,
};

static void usb_os_desc_ext_prop_release(struct config_item *item)
{
	struct usb_os_desc_ext_prop *ext_prop = to_usb_os_desc_ext_prop(item);

	kfree(ext_prop); /* frees a whole chunk */
}

static struct configfs_item_operations ext_prop_ops = {
	.release		= usb_os_desc_ext_prop_release,
};

static struct config_item *ext_prop_make(
		struct config_group *group,
		const char *name)
{
	struct usb_os_desc_ext_prop *ext_prop;
	struct config_item_type *ext_prop_type;
	struct usb_os_desc *desc;
	char *vlabuf;

	vla_group(data_chunk);
	vla_item(data_chunk, struct usb_os_desc_ext_prop, ext_prop, 1);
	vla_item(data_chunk, struct config_item_type, ext_prop_type, 1);

	vlabuf = kzalloc(vla_group_size(data_chunk), GFP_KERNEL);
	if (!vlabuf)
		return ERR_PTR(-ENOMEM);

	ext_prop = vla_ptr(vlabuf, data_chunk, ext_prop);
	ext_prop_type = vla_ptr(vlabuf, data_chunk, ext_prop_type);

	desc = container_of(group, struct usb_os_desc, group);
	ext_prop_type->ct_item_ops = &ext_prop_ops;
	ext_prop_type->ct_attrs = ext_prop_attrs;
	ext_prop_type->ct_owner = desc->owner;

	config_item_init_type_name(&ext_prop->item, name, ext_prop_type);

	ext_prop->name = kstrdup(name, GFP_KERNEL);
	if (!ext_prop->name) {
		kfree(vlabuf);
		return ERR_PTR(-ENOMEM);
	}
	desc->ext_prop_len += 14;
	ext_prop->name_len = 2 * strlen(ext_prop->name) + 2;
	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	desc->ext_prop_len += ext_prop->name_len;
	list_add_tail(&ext_prop->entry, &desc->ext_prop);
	++desc->ext_prop_count;
	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);

	return &ext_prop->item;
}

static void ext_prop_drop(struct config_group *group, struct config_item *item)
{
	struct usb_os_desc_ext_prop *ext_prop = to_usb_os_desc_ext_prop(item);
	struct usb_os_desc *desc = to_usb_os_desc(&group->cg_item);

	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	list_del(&ext_prop->entry);
	--desc->ext_prop_count;
	kfree(ext_prop->name);
	desc->ext_prop_len -= (ext_prop->name_len + ext_prop->data_len + 14);
	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);
	config_item_put(item);
}

static struct configfs_group_operations interf_grp_ops = {
	.make_item	= &ext_prop_make,
	.drop_item	= &ext_prop_drop,
};

static ssize_t interf_grp_compatible_id_show(struct config_item *item,
					     char *page)
{
	memcpy(page, to_usb_os_desc(item)->ext_compat_id, 8);
	return 8;
}

static ssize_t interf_grp_compatible_id_store(struct config_item *item,
					      const char *page, size_t len)
{
	struct usb_os_desc *desc = to_usb_os_desc(item);
	int l;

	l = min_t(int, 8, len);
	if (page[l - 1] == '\n')
		--l;
	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	memcpy(desc->ext_compat_id, page, l);

	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);

	return len;
}

static ssize_t interf_grp_sub_compatible_id_show(struct config_item *item,
						 char *page)
{
	memcpy(page, to_usb_os_desc(item)->ext_compat_id + 8, 8);
	return 8;
}

static ssize_t interf_grp_sub_compatible_id_store(struct config_item *item,
						  const char *page, size_t len)
{
	struct usb_os_desc *desc = to_usb_os_desc(item);
	int l;

	l = min_t(int, 8, len);
	if (page[l - 1] == '\n')
		--l;
	if (desc->opts_mutex)
		mutex_lock(desc->opts_mutex);
	memcpy(desc->ext_compat_id + 8, page, l);

	if (desc->opts_mutex)
		mutex_unlock(desc->opts_mutex);

	return len;
}

CONFIGFS_ATTR(interf_grp_, compatible_id);
CONFIGFS_ATTR(interf_grp_, sub_compatible_id);

static struct configfs_attribute *interf_grp_attrs[] = {
	&interf_grp_attr_compatible_id,
	&interf_grp_attr_sub_compatible_id,
	NULL
};

int usb_os_desc_prepare_interf_dir(struct config_group *parent,
				   int n_interf,
				   struct usb_os_desc **desc,
				   char **names,
				   struct module *owner)
{
	struct config_group **f_default_groups, *os_desc_group,
				**interface_groups;
	struct config_item_type *os_desc_type, *interface_type;

	vla_group(data_chunk);
	vla_item(data_chunk, struct config_group *, f_default_groups, 2);
	vla_item(data_chunk, struct config_group, os_desc_group, 1);
	vla_item(data_chunk, struct config_group *, interface_groups,
		 n_interf + 1);
	vla_item(data_chunk, struct config_item_type, os_desc_type, 1);
	vla_item(data_chunk, struct config_item_type, interface_type, 1);

	char *vlabuf = kzalloc(vla_group_size(data_chunk), GFP_KERNEL);
	if (!vlabuf)
		return -ENOMEM;

	f_default_groups = vla_ptr(vlabuf, data_chunk, f_default_groups);
	os_desc_group = vla_ptr(vlabuf, data_chunk, os_desc_group);
	os_desc_type = vla_ptr(vlabuf, data_chunk, os_desc_type);
	interface_groups = vla_ptr(vlabuf, data_chunk, interface_groups);
	interface_type = vla_ptr(vlabuf, data_chunk, interface_type);

	parent->default_groups = f_default_groups;
	os_desc_type->ct_owner = owner;
	config_group_init_type_name(os_desc_group, "os_desc", os_desc_type);
	f_default_groups[0] = os_desc_group;

	os_desc_group->default_groups = interface_groups;
	interface_type->ct_group_ops = &interf_grp_ops;
	interface_type->ct_attrs = interf_grp_attrs;
	interface_type->ct_owner = owner;

	while (n_interf--) {
		struct usb_os_desc *d;

		d = desc[n_interf];
		d->owner = owner;
		config_group_init_type_name(&d->group, "", interface_type);
		config_item_set_name(&d->group.cg_item, "interface.%s",
				     names[n_interf]);
		interface_groups[n_interf] = &d->group;
	}

	return 0;
}
EXPORT_SYMBOL(usb_os_desc_prepare_interf_dir);

static int configfs_do_nothing(struct usb_composite_dev *cdev)
{
	WARN_ON(1);
	return -EINVAL;
}

int composite_dev_prepare(struct usb_composite_driver *composite,
		struct usb_composite_dev *dev);

int composite_os_desc_req_prepare(struct usb_composite_dev *cdev,
				  struct usb_ep *ep0);

static void purge_configs_funcs(struct gadget_info *gi)
{
	struct usb_configuration	*c;

	list_for_each_entry(c, &gi->cdev.configs, list) {
		struct usb_function *f, *tmp;
		struct config_usb_cfg *cfg;

		cfg = container_of(c, struct config_usb_cfg, c);

		list_for_each_entry_safe(f, tmp, &c->functions, list) {

			list_move_tail(&f->list, &cfg->func_list);
			if (f->unbind) {
				dev_err(&gi->cdev.gadget->dev, "unbind function"
						" '%s'/%pK\n", f->name, f);
				f->unbind(c, f);
			}
		}
		c->next_interface_id = 0;
		memset(c->interface, 0, sizeof(c->interface));
		c->superspeed = 0;
		c->highspeed = 0;
		c->fullspeed = 0;
	}
}

static int configfs_composite_bind(struct usb_gadget *gadget,
		struct usb_gadget_driver *gdriver)
{
	struct usb_composite_driver     *composite = to_cdriver(gdriver);
	struct gadget_info		*gi = container_of(composite,
						struct gadget_info, composite);
	struct usb_composite_dev	*cdev = &gi->cdev;
	struct usb_configuration	*c;
	struct usb_string		*s;
	unsigned			i;
	int				ret;

	/* the gi->lock is hold by the caller */
	gi->unbind = 0;
	cdev->gadget = gadget;
	set_gadget_data(gadget, cdev);
	ret = composite_dev_prepare(composite, cdev);
	if (ret)
		return ret;
	/* and now the gadget bind */
	ret = -EINVAL;

	if (list_empty(&gi->cdev.configs)) {
		pr_err("Need at least one configuration in %s.\n",
				gi->composite.name);
		goto err_comp_cleanup;
	}


	list_for_each_entry(c, &gi->cdev.configs, list) {
		struct config_usb_cfg *cfg;

		cfg = container_of(c, struct config_usb_cfg, c);
		if (list_empty(&cfg->func_list)) {
			pr_err("Config %s/%d of %s needs at least one function.\n",
			      c->label, c->bConfigurationValue,
			      gi->composite.name);
			goto err_comp_cleanup;
		}
	}

	/* init all strings */
	if (!list_empty(&gi->string_list)) {
		struct gadget_strings *gs;

		i = 0;
		list_for_each_entry(gs, &gi->string_list, list) {

			gi->gstrings[i] = &gs->stringtab_dev;
			gs->stringtab_dev.strings = gs->strings;
			gs->strings[USB_GADGET_MANUFACTURER_IDX].s =
				gs->manufacturer;
			gs->strings[USB_GADGET_PRODUCT_IDX].s = gs->product;
#ifndef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
			if (gs->serialnumber && !set_alt_serialnumber(gs))
				pr_info("usb: serial number: %s\n",
						gs->serialnumber);
#endif
			gs->strings[USB_GADGET_SERIAL_IDX].s = gs->serialnumber;
			i++;
		}
		gi->gstrings[i] = NULL;
		s = usb_gstrings_attach(&gi->cdev, gi->gstrings,
				USB_GADGET_FIRST_AVAIL_IDX);
		if (IS_ERR(s)) {
			ret = PTR_ERR(s);
			goto err_comp_cleanup;
		}

		gi->cdev.desc.iManufacturer = s[USB_GADGET_MANUFACTURER_IDX].id;
		gi->cdev.desc.iProduct = s[USB_GADGET_PRODUCT_IDX].id;
		gi->cdev.desc.iSerialNumber = s[USB_GADGET_SERIAL_IDX].id;
	}

	if (gi->use_os_desc) {
		cdev->use_os_string = true;
		cdev->b_vendor_code = gi->b_vendor_code;
		memcpy(cdev->qw_sign, gi->qw_sign, OS_STRING_QW_SIGN_LEN);
	}

	if (gadget_is_otg(gadget) && !otg_desc[0]) {
		struct usb_descriptor_header *usb_desc;

		usb_desc = usb_otg_descriptor_alloc(gadget);
		if (!usb_desc) {
			ret = -ENOMEM;
			goto err_comp_cleanup;
		}
		usb_otg_descriptor_init(gadget, usb_desc);
		otg_desc[0] = usb_desc;
		otg_desc[1] = NULL;
	}

	/* Go through all configs, attach all functions */
	list_for_each_entry(c, &gi->cdev.configs, list) {
		struct config_usb_cfg *cfg;
		struct usb_function *f;
		struct usb_function *tmp;
		struct gadget_config_name *cn;

		if (gadget_is_otg(gadget))
			c->descriptors = otg_desc;

		cfg = container_of(c, struct config_usb_cfg, c);
		if (!list_empty(&cfg->string_list)) {
			i = 0;
			list_for_each_entry(cn, &cfg->string_list, list) {
				cfg->gstrings[i] = &cn->stringtab_dev;
				cn->stringtab_dev.strings = &cn->strings;
				cn->strings.s = cn->configuration;
				i++;
			}
			cfg->gstrings[i] = NULL;
			s = usb_gstrings_attach(&gi->cdev, cfg->gstrings, 1);
			if (IS_ERR(s)) {
				ret = PTR_ERR(s);
				goto err_comp_cleanup;
			}
			c->iConfiguration = s[0].id;
		}

		list_for_each_entry_safe(f, tmp, &cfg->func_list, list) {
			list_del(&f->list);
			ret = usb_add_function(c, f);
			if (ret) {
				list_add(&f->list, &cfg->func_list);
				goto err_purge_funcs;
			}
		}
		usb_ep_autoconfig_reset(cdev->gadget);
	}
	if (cdev->use_os_string) {
		ret = composite_os_desc_req_prepare(cdev, gadget->ep0);
		if (ret)
			goto err_purge_funcs;
	}

	usb_ep_autoconfig_reset(cdev->gadget);
	return 0;

err_purge_funcs:
	purge_configs_funcs(gi);
err_comp_cleanup:
	composite_dev_cleanup(cdev);
	return ret;
}

#ifdef CONFIG_USB_CONFIGFS_UEVENT
static void android_work(struct work_struct *data)
{
	struct gadget_info *gi = container_of(data, struct gadget_info, work);
	struct usb_composite_dev *cdev = &gi->cdev;
	char *disconnected[2] = { "USB_STATE=DISCONNECTED", NULL };
	char *connected[2]    = { "USB_STATE=CONNECTED", NULL };
	char *configured[2]   = { "USB_STATE=CONFIGURED", NULL };
	/* 0-connected 1-configured 2-disconnected*/
	bool status[3] = { false, false, false };
	unsigned long flags;
	bool uevent_sent = false;

	if (!android_device && IS_ERR(android_device))
		return;

	spin_lock_irqsave(&cdev->lock, flags);
	if (cdev->config)
		status[1] = true;

	if (gi->connected != gi->sw_connected) {
		if (gi->connected)
			status[0] = true;
		else
			status[2] = true;
		gi->sw_connected = gi->connected;
	}

	spin_unlock_irqrestore(&cdev->lock, flags);

	if (status[0]) {
		kobject_uevent_env(&android_device->kobj,
					KOBJ_CHANGE, connected);
		pr_info("%s: sent uevent %s\n", __func__, connected[0]);
		uevent_sent = true;
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		store_usblog_notify(NOTIFY_USBSTATE, (void *)connected[0], NULL);
#endif
		if (cdev->desc.bcdUSB == 0x310) {
			set_usb_enumeration_state(0x310); // Super-Speed
		} else {
			set_usb_enumeration_state(0x210); // High-Speed
		}
	}

	if (status[1]) {
		kobject_uevent_env(&android_device->kobj,
					KOBJ_CHANGE, configured);
		pr_info("%s: sent uevent %s\n", __func__, configured[0]);
		uevent_sent = true;
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		store_usblog_notify(NOTIFY_USBSTATE, (void *)configured[0], NULL);
#endif
	}

	if (status[2]) {
		kobject_uevent_env(&android_device->kobj,
					KOBJ_CHANGE, disconnected);
		pr_info("%s: sent uevent %s\n", __func__, disconnected[0]);
		uevent_sent = true;
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		store_usblog_notify(NOTIFY_USBSTATE, (void *)disconnected[0], NULL);
#endif
#ifndef CONFIG_USB_TYPEC_MANAGER_NOTIFIER
		usb_enum_state = 0;
#endif
	}

	if (!uevent_sent) {
		pr_info("%s: did not send uevent (%d %d %pK)\n", __func__,
			gi->connected, gi->sw_connected, cdev->config);
	}
}
#endif

static void configfs_composite_unbind(struct usb_gadget *gadget)
{
	struct usb_composite_dev	*cdev;
	struct gadget_info		*gi;
	unsigned long flags;

	/* the gi->lock is hold by the caller */

	cdev = get_gadget_data(gadget);
	gi = container_of(cdev, struct gadget_info, cdev);
	spin_lock_irqsave(&gi->spinlock, flags);
	gi->unbind = 1;
	spin_unlock_irqrestore(&gi->spinlock, flags);

	kfree(otg_desc[0]);
	otg_desc[0] = NULL;
	purge_configs_funcs(gi);
	composite_dev_cleanup(cdev);
	usb_ep_autoconfig_reset(cdev->gadget);
	spin_lock_irqsave(&gi->spinlock, flags);
	cdev->gadget = NULL;
	set_gadget_data(gadget, NULL);
	spin_unlock_irqrestore(&gi->spinlock, flags);
}

static int configfs_composite_setup(struct usb_gadget *gadget,
		const struct usb_ctrlrequest *ctrl)
{
	struct usb_composite_dev *cdev;
	struct gadget_info *gi;
	unsigned long flags;
	int ret;

	cdev = get_gadget_data(gadget);
	if (!cdev)
		return 0;

	gi = container_of(cdev, struct gadget_info, cdev);
	spin_lock_irqsave(&gi->spinlock, flags);
	cdev = get_gadget_data(gadget);
	if (!cdev || gi->unbind) {
		spin_unlock_irqrestore(&gi->spinlock, flags);
		return 0;
	}

	ret = composite_setup(gadget, ctrl);
	spin_unlock_irqrestore(&gi->spinlock, flags);
	return ret;
}

static void configfs_composite_disconnect(struct usb_gadget *gadget)
{
	struct usb_composite_dev *cdev;
	struct gadget_info *gi;
	unsigned long flags;

	cdev = get_gadget_data(gadget);
	if (!cdev)
		return;

	gi = container_of(cdev, struct gadget_info, cdev);
	spin_lock_irqsave(&gi->spinlock, flags);
	cdev = get_gadget_data(gadget);
	if (!cdev || gi->unbind) {
		spin_unlock_irqrestore(&gi->spinlock, flags);
		return;
	}

	composite_disconnect(gadget);
	spin_unlock_irqrestore(&gi->spinlock, flags);
}

static void configfs_composite_suspend(struct usb_gadget *gadget)
{
	struct usb_composite_dev *cdev;
	struct gadget_info *gi;
	unsigned long flags;

	cdev = get_gadget_data(gadget);
	if (!cdev)
		return;

	gi = container_of(cdev, struct gadget_info, cdev);
	spin_lock_irqsave(&gi->spinlock, flags);
	cdev = get_gadget_data(gadget);
	if (!cdev || gi->unbind) {
		spin_unlock_irqrestore(&gi->spinlock, flags);
		return;
	}

	composite_suspend(gadget);
	spin_unlock_irqrestore(&gi->spinlock, flags);
}

static void configfs_composite_resume(struct usb_gadget *gadget)
{
	struct usb_composite_dev *cdev;
	struct gadget_info *gi;
	unsigned long flags;

	cdev = get_gadget_data(gadget);
	if (!cdev)
		return;

	gi = container_of(cdev, struct gadget_info, cdev);
	spin_lock_irqsave(&gi->spinlock, flags);
	cdev = get_gadget_data(gadget);
	if (!cdev || gi->unbind) {
		spin_unlock_irqrestore(&gi->spinlock, flags);
		return;
	}

	composite_resume(gadget);
	spin_unlock_irqrestore(&gi->spinlock, flags);
}
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
static void android_gadget_complete(struct usb_ep *ep, struct usb_request *req)
{
	if(req->status || req->actual != req->length)
			printk(KERN_DEBUG "usb: %s: %d, %d/%d\n", __func__,
				req->status, req->actual, req->length);
}
#endif
#ifdef CONFIG_USB_CONFIGFS_UEVENT
static int android_setup(struct usb_gadget *gadget,
			const struct usb_ctrlrequest *c)
{
	struct usb_composite_dev *cdev = get_gadget_data(gadget);
	unsigned long flags;
	struct gadget_info *gi = container_of(cdev, struct gadget_info, cdev);
	int value = -EOPNOTSUPP;
	struct usb_configuration *configuration;
	struct usb_function *f;
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	struct usb_request		*req = cdev->req;

	req->complete = android_gadget_complete;
#endif
	spin_lock_irqsave(&cdev->lock, flags);
	if (!gi->connected) {
		gi->connected = 1;
		schedule_work(&gi->work);
	}

	spin_unlock_irqrestore(&cdev->lock, flags);
	list_for_each_entry(configuration, &cdev->configs, list) {
		list_for_each_entry(f, &configuration->functions, list) {
			if (f != NULL && f->ctrlrequest != NULL) {
				value = f->ctrlrequest(f, c);
				if (value >= 0)
					break;
				}
			}
		}	

#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	if (value < 0)
		value = terminal_ctrl_request(cdev, c);
#endif

#ifdef CONFIG_USB_CONFIGFS_F_ACC
	if (value < 0)
		value = acc_ctrlrequest(cdev, c);
#endif

	if (value < 0)
		value = composite_setup(gadget, c);

	spin_lock_irqsave(&cdev->lock, flags);

#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	if (c->bRequest == USB_REQ_SET_CONFIGURATION &&
			cdev->mute_switch == true)
		cdev->mute_switch = false;
#endif
	
	if (c->bRequest == USB_REQ_SET_CONFIGURATION &&
						cdev->config) {
		schedule_work(&gi->work);
	}
	spin_unlock_irqrestore(&cdev->lock, flags);

	return value;
}

static void android_disconnect(struct usb_gadget *gadget)
{
	struct usb_composite_dev        *cdev = get_gadget_data(gadget);
	struct gadget_info *gi = container_of(cdev, struct gadget_info, cdev);

	/* FIXME: There's a race between usb_gadget_udc_stop() which is likely
	 * to set the gadget driver to NULL in the udc driver and this drivers
	 * gadget disconnect fn which likely checks for the gadget driver to
	 * be a null ptr. It happens that unbind (doing set_gadget_data(NULL))
	 * is called before the gadget driver is set to NULL and the udc driver
	 * calls disconnect fn which results in cdev being a null ptr.
	 */
	if (cdev == NULL) {
		WARN(1, "%s: gadget driver already disconnected\n", __func__);
		return;
	}

	/* accessory HID support can be active while the
		accessory function is not actually enabled,
		so we need to inform it when we are disconnected.
	*/

#ifdef CONFIG_USB_CONFIGFS_F_ACC
	acc_disconnect();
#endif
	gi->connected = 0;

#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	printk(KERN_DEBUG "usb: %s con(%d), sw(%d)\n",
		 __func__, gi->connected, gi->sw_connected);
	/* avoid sending a disconnect switch event
	 * until after we disconnect.
	 */
	if (cdev->mute_switch) {
		gi->sw_connected = gi->connected;
		printk(KERN_DEBUG"usb: %s mute_switch con(%d) sw(%d)\n",
			 __func__, gi->connected, gi->sw_connected);
	} else {
	
	//	set_ncm_ready(false);
		if (cdev->force_disconnect) {
			gi->sw_connected = 1;
			printk(KERN_DEBUG"usb: %s force_disconnect\n",
				 __func__);
			cdev->force_disconnect = 0;
		}
		printk(KERN_DEBUG"usb: %s schedule_work con(%d) sw(%d)\n",
			 __func__, gi->connected, gi->sw_connected);
		schedule_work(&gi->work);
	}
	composite_disconnect(gadget);
#else
	schedule_work(&gi->work);
	composite_disconnect(gadget);
#endif	
}
#endif

static const struct usb_gadget_driver configfs_driver_template = {
	.bind           = configfs_composite_bind,
	.unbind         = configfs_composite_unbind,
#ifdef CONFIG_USB_CONFIGFS_UEVENT
	.setup          = android_setup,
	.reset          = android_disconnect,
	.disconnect     = android_disconnect,
#else
	.setup          = configfs_composite_setup,
	.reset          = configfs_composite_disconnect,
	.disconnect     = configfs_composite_disconnect,
#endif
	.suspend	= configfs_composite_suspend,
	.resume		= configfs_composite_resume,

	.max_speed	= USB_SPEED_SUPER,
	.driver = {
		.owner          = THIS_MODULE,
		.name		= "configfs-gadget",
	},
};

#ifdef CONFIG_USB_CONFIGFS_UEVENT
static ssize_t
functions_show(struct device *pdev, struct device_attribute *attr, char *buf)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev;
	struct usb_configuration *c;
	struct usb_function *f;
	char *buff = buf;

	cdev = &dev->cdev;
	if (!cdev)
		return -ENODEV;

	mutex_lock(&dev->lock);

	list_for_each_entry(c, &cdev->configs, list) {
		list_for_each_entry(f, &c->functions, list) {
			buff += sprintf(buff, "%s,", f->name);
		}
	}

	mutex_unlock(&dev->lock);

	if (buff != buf)
		*(buff-1) = '\n';

	return buff - buf;
}

static void make_usb_func_link(struct gadget_info *dev, char * buf)
{
	struct usb_composite_dev *cdev;
	struct usb_configuration *c;
	struct config_usb_cfg *cfg;
	struct usb_function *f, *tmp;
	char *name;
	char *b;

	cdev = &dev->cdev;
	b = strim(buf);

#ifdef CONFIG_USB_NOTIFY_PROC_LOG
	store_usblog_notify(NOTIFY_USBMODE, (void *)b, NULL);
#endif

	while (b) {
		name = strsep(&b, ",");
		if (!name)
			continue;

		list_for_each_entry(c, &cdev->configs, list) {
			cfg = container_of(c, struct config_usb_cfg, c);
			list_for_each_entry_safe(f, tmp, &dev->linked_func, list) {
				if (!strcmp(f->name, name)) {
					pr_err("usb: %s: enable device[%s]\n", __func__, name);
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
					if (!strcmp(f->name, "acm")) {
						printk(KERN_DEBUG "usb: acm is enabled. (bcdDevice=0x400)\n");
						/* Samsung KIES needs fixed bcdDevice number */
						cdev->desc.bcdDevice = cpu_to_le16(0x0400);
					}
					if (!strcmp(f->name, "conn_gadget")) {
						if (cdev->desc.bcdDevice == cpu_to_le16(0x0400))	{
							printk(KERN_DEBUG "usb: conn_gadget + kies (bcdDevice=0xC00)\n");
							cdev->desc.bcdDevice = cpu_to_le16(0x0C00);
						} else {
							printk(KERN_DEBUG "usb: conn_gadget only (bcdDevice=0x800)\n");
							cdev->desc.bcdDevice = cpu_to_le16(0x0800);
						}
					}
#endif					
					list_move_tail(&f->list, &cfg->func_list);
				}
			}
		}
	}	
}

static ssize_t
functions_store(struct device *pdev, struct device_attribute *attr,
					const char *buff, size_t size)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev;
	char buf[256];

	cdev = &dev->cdev;
	if (!cdev)
		return -ENODEV;

	mutex_lock(&dev->lock);

	if (dev->enabled) {
		mutex_unlock(&dev->lock);
		pr_err("usb: %s: gadget is enabled\n", __func__);
		return -EBUSY;
	}

	strlcpy(buf, buff, sizeof(buf));

	/* save current function set */
	if (dev->prev_func_list)
		kfree(dev->prev_func_list);

	dev->prev_func_list = kstrdup(buf, GFP_KERNEL);
	if (!dev->prev_func_list)
		pr_err("usb: %s: prev_func_list mem alloc fail [%s]\n", __func__, buf);

	make_usb_func_link(dev, buf);

	mutex_unlock(&dev->lock);

	return size;
}

static ssize_t enable_show(struct device *pdev, struct device_attribute *attr,
			   char *buf)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);
	return sprintf(buf, "%d\n", dev->enabled);
}

static ssize_t enable_store(struct device *pdev, struct device_attribute *attr,
			    const char *buff, size_t size)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev;
	struct usb_gadget *gadget;
	struct usb_configuration *c;
	struct config_usb_cfg *cfg;
	struct usb_function *f, *tmp;
	char *dwc_name = "10c00000.dwc3";
	char *name;
	int ret, len;
	int enabled = 0;

	if (!dev)
		return -ENODEV;

	cdev = &dev->cdev;
	if (!cdev)
		return -ENODEV;

	gadget = cdev->gadget;
	mutex_lock(&dev->lock);

	sscanf(buff, "%d", &enabled);
	if (enabled && !dev->enabled) {
		pr_info("usb: %s: Connect gadget: enabled=%d, dev->enabled=%d\n",
				__func__, enabled, dev->enabled);
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		store_usblog_notify(NOTIFY_USBMODE_EXTRA, "enable 1", NULL);
#endif
#ifdef 	CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
		cdev->next_string_id = 4;  //composite string index
#else
		cdev->next_string_id = 0;
#endif

		if (!dev->udc_name && dev->prev_func_list) {
			/* config gadget is not bined yet */
			/* we need to call usb function bind */
			list_for_each_entry(c, &cdev->configs, list) {
				cfg = container_of(c, struct config_usb_cfg, c);

				if (list_empty(&cfg->func_list)) {
					make_usb_func_link(dev, dev->prev_func_list);
				}
			}
			name = kstrdup(dwc_name, GFP_KERNEL);
			if (name) {		
				len = sizeof(dwc_name);
				if (name[len - 1] == '\n')
					name[len - 1] = '\0';

				ret = usb_udc_attach_driver(name, &dev->composite.gadget_driver);
				if (ret) {
					pr_info("usb: %s: configfs gadget bind fails: %d\n", __func__, ret);
					kfree(name);
				} else {
					dev->udc_name = name;
					gadget = cdev->gadget;
				}
			}
		}	

		if (!gadget) {
			pr_info("usb: %s: Gadget is NULL\n", __func__);
			mutex_unlock(&dev->lock);
			return -ENODEV;
		}

		usb_gadget_connect(gadget);
		dev->enabled = true;
#ifdef CONFIG_USB_TYPEC_MANAGER_NOTIFIER
		set_usb_enable_state();
#endif
	} else if (!enabled && dev->enabled) {
		pr_info("usb: %s: Disconnect gadget: enabled=%d, dev->enabled=%d\n",
				__func__, enabled, dev->enabled);
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
		/* avoid sending a disconnect switch event
		 * until after we disconnect.
		 */
		cdev->mute_switch = true;
#endif
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		store_usblog_notify(NOTIFY_USBMODE_EXTRA, "enable 0", NULL);
#endif
		unregister_gadget(dev);
		list_for_each_entry(c, &cdev->configs, list) {
			cfg = container_of(c, struct config_usb_cfg, c);
			list_for_each_entry_safe(f, tmp, &cfg->func_list, list) {
				list_move_tail(&f->list, &dev->linked_func);
			}
			c->next_interface_id = 0;
			memset(c->interface, 0, sizeof(c->interface));
		}
		dev->enabled = false;
	} else {
#ifdef CONFIG_USB_NOTIFY_PROC_LOG
		if (dev->enabled)
			store_usblog_notify(NOTIFY_USBMODE_EXTRA, "already 1", NULL);
		else
			store_usblog_notify(NOTIFY_USBMODE_EXTRA, "already 0", NULL);
#endif
		pr_err("usb: %s: already %s\n", __func__,
				dev->enabled ? "enabled" : "disabled");
	}

	mutex_unlock(&dev->lock);
	return size;
}

static ssize_t state_show(struct device *pdev, struct device_attribute *attr,
			char *buf)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);
	struct usb_composite_dev *cdev;
	char *state = "DISCONNECTED";
	unsigned long flags;

	if (!dev)
		goto out;

	cdev = &dev->cdev;

	if (!cdev)
		goto out;

	spin_lock_irqsave(&cdev->lock, flags);
	if (cdev->config)
		state = "CONFIGURED";
	else if (dev->connected)
		state = "CONNECTED";
	spin_unlock_irqrestore(&cdev->lock, flags);
out:
	return sprintf(buf, "%s\n", state);
}

static DEVICE_ATTR(functions, S_IRUGO | S_IWUSR, functions_show,
						functions_store);
static DEVICE_ATTR(enable, S_IRUGO | S_IWUSR, enable_show, enable_store);
static DEVICE_ATTR(state, S_IRUGO, state_show, NULL);

#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
static ssize_t
bcdUSB_show(struct device *pdev, struct device_attribute *attr, char *buf)
{
	struct gadget_info *dev = dev_get_drvdata(pdev);

	return sprintf(buf, "%04x\n", dev->cdev.desc.bcdUSB);
	
}
static DEVICE_ATTR(bcdUSB, S_IRUGO, bcdUSB_show, NULL);
#endif

static struct device_attribute *android_usb_attributes[] = {
	&dev_attr_state,
	&dev_attr_enable,
	&dev_attr_functions,
#ifdef CONFIG_USB_ANDROID_SAMSUNG_COMPOSITE
	&dev_attr_bcdUSB,
#endif	
	NULL
};

static int android_device_create(struct gadget_info *gi)
{
	struct device *device;
	struct device_attribute **attrs;
	struct device_attribute *attr;

	INIT_WORK(&gi->work, android_work);
	device = device_create(android_class, NULL,
				MKDEV(0, 0), NULL, "android0");

	if (IS_ERR(device))
		return PTR_ERR(device);

	android_device = device;
	dev_set_drvdata(android_device, gi);

	attrs = android_usb_attributes;
	while ((attr = *attrs++)) {
		int err;

		err = device_create_file(android_device, attr);
		if (err) {
			device_destroy(android_device->class,
				       android_device->devt);
			return err;
		}
	}

	return 0;
}

static void android_device_destroy(void)
{
	struct device_attribute **attrs;
	struct device_attribute *attr;

	attrs = android_usb_attributes;
	while ((attr = *attrs++))
		device_remove_file(android_device, attr);
	device_destroy(android_device->class, android_device->devt);
}
#else
static inline int android_device_create(struct gadget_info *gi)
{
	return 0;
}

static inline void android_device_destroy(void)
{
}
#endif

static struct config_group *gadgets_make(
		struct config_group *group,
		const char *name)
{
	struct gadget_info *gi;

	gi = kzalloc(sizeof(*gi), GFP_KERNEL);
	if (!gi)
		return ERR_PTR(-ENOMEM);
	gi->group.default_groups = gi->default_groups;
	gi->group.default_groups[0] = &gi->functions_group;
	gi->group.default_groups[1] = &gi->configs_group;
	gi->group.default_groups[2] = &gi->strings_group;
	gi->group.default_groups[3] = &gi->os_desc_group;

	config_group_init_type_name(&gi->functions_group, "functions",
			&functions_type);
	config_group_init_type_name(&gi->configs_group, "configs",
			&config_desc_type);
	config_group_init_type_name(&gi->strings_group, "strings",
			&gadget_strings_strings_type);
	config_group_init_type_name(&gi->os_desc_group, "os_desc",
			&os_desc_type);

	gi->composite.bind = configfs_do_nothing;
	gi->composite.unbind = configfs_do_nothing;
	gi->composite.suspend = NULL;
	gi->composite.resume = NULL;
	gi->composite.max_speed = USB_SPEED_SUPER;

	spin_lock_init(&gi->spinlock);
	mutex_init(&gi->lock);
	INIT_LIST_HEAD(&gi->string_list);
	INIT_LIST_HEAD(&gi->available_func);
#ifdef CONFIG_USB_CONFIGFS_UEVENT
	INIT_LIST_HEAD(&gi->linked_func);
#endif

	composite_init_dev(&gi->cdev);
	gi->cdev.desc.bLength = USB_DT_DEVICE_SIZE;
	gi->cdev.desc.bDescriptorType = USB_DT_DEVICE;
	gi->cdev.desc.bcdDevice = cpu_to_le16(get_default_bcdDevice());

	gi->composite.gadget_driver = configfs_driver_template;

	gi->composite.gadget_driver.function = kstrdup(name, GFP_KERNEL);
	gi->composite.name = gi->composite.gadget_driver.function;

	if (!gi->composite.gadget_driver.function)
		goto err;

	if (android_device_create(gi) < 0) {
		kfree(gi->composite.gadget_driver.function);
		goto err;
	}
	config_group_init_type_name(&gi->group, name,
				&gadget_root_type);
	return &gi->group;

err:
	kfree(gi);
	return ERR_PTR(-ENOMEM);
}

static void gadgets_drop(struct config_group *group, struct config_item *item)
{
	config_item_put(item);
	android_device_destroy();
}

static struct configfs_group_operations gadgets_ops = {
	.make_group     = &gadgets_make,
	.drop_item      = &gadgets_drop,
};

static struct config_item_type gadgets_type = {
	.ct_group_ops   = &gadgets_ops,
	.ct_owner       = THIS_MODULE,
};

static struct configfs_subsystem gadget_subsys = {
	.su_group = {
		.cg_item = {
			.ci_namebuf = "usb_gadget",
			.ci_type = &gadgets_type,
		},
	},
	.su_mutex = __MUTEX_INITIALIZER(gadget_subsys.su_mutex),
};

void unregister_gadget_item(struct config_item *item)
{
	struct gadget_info *gi = to_gadget_info(item);

	mutex_lock(&gi->lock);
	unregister_gadget(gi);
	mutex_unlock(&gi->lock);
}
EXPORT_SYMBOL_GPL(unregister_gadget_item);

static int __init gadget_cfs_init(void)
{
	int ret;

	config_group_init(&gadget_subsys.su_group);

	ret = configfs_register_subsystem(&gadget_subsys);

#ifdef CONFIG_USB_CONFIGFS_UEVENT
	android_class = class_create(THIS_MODULE, "android_usb");
	if (IS_ERR(android_class))
		return PTR_ERR(android_class);
#endif

	return ret;
}
module_init(gadget_cfs_init);

static void __exit gadget_cfs_exit(void)
{
	configfs_unregister_subsystem(&gadget_subsys);
#ifdef CONFIG_USB_CONFIGFS_UEVENT
	if (!IS_ERR(android_class))
		class_destroy(android_class);
#endif

}
module_exit(gadget_cfs_exit);
