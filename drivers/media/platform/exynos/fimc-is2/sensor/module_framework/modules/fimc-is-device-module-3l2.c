/*
 * Samsung Exynos5 SoC series Sensor driver
 *
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/i2c.h>
#include <linux/slab.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/delay.h>
#include <linux/version.h>
#include <linux/gpio.h>
#include <linux/clk.h>
#include <linux/regulator/consumer.h>
#include <linux/videodev2.h>
#include <linux/videodev2_exynos_camera.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/platform_device.h>
#include <linux/of_gpio.h>
#include <media/v4l2-ctrls.h>
#include <media/v4l2-device.h>
#include <media/v4l2-subdev.h>

#include <exynos-fimc-is-sensor.h>
#include "fimc-is-hw.h"
#include "fimc-is-core.h"
#include "fimc-is-device-sensor.h"
#include "fimc-is-device-sensor-peri.h"
#include "fimc-is-resourcemgr.h"
#include "fimc-is-dt.h"

#include "fimc-is-device-module-base.h"

/*
 * [ AP Information ]
 * LASSEN - MCLK 26MHz _ 16x10 Margin
 *
 * [Mode Information]
 *	0: 4144 x 3106 @30,  Full (4:3),    MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	1: 4144 x 2332 @30,  Full (16:9),   MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	2: 4144 x 2016 @30,  Full (18.5:9), MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	3: 2072 x 1552 @30,  BIN2 (4:3),    MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	4: 2072 x 1552 @15,  BIN2 (4:3),    MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	5: 2072 x 1162 @30,  BIN2 (16:9),   MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	6: 2072 x 1166 @60,  BIN2 (16:9),   MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	7: 2072 x 1008 @30,  BIN2 (18.5:9), MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	8: 1008 x 754  @120, BIN4 (4:3),    MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 *	9: 1008 x 568  @120, BIN4 (16:9),   MIPI lane: 4, MIPI data rate(Mbps/lane) Sensor: 1096.33, MIPI Clk(Mhz): 548.165
 */
static struct fimc_is_sensor_cfg config_module_3l2[] = {
	/* 4144x3106@30fps */
	FIMC_IS_SENSOR_CFG_EXT(4144, 3106, 30, 24, 0, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 4144x2332@30fps */
	FIMC_IS_SENSOR_CFG_EXT(4144, 2332, 30, 24, 1, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 4144x2016@30fps */
	FIMC_IS_SENSOR_CFG_EXT(4144, 2016, 30, 24, 2, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 2072x1552@30fps */
	FIMC_IS_SENSOR_CFG_EXT(2072, 1552, 30, 24, 3, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 2072x1552@15fps */
	FIMC_IS_SENSOR_CFG_EXT(2072, 1552, 15, 24, 4, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 2072x1162@30fps */
	FIMC_IS_SENSOR_CFG_EXT(2072, 1162, 30, 24, 5, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 2072x1166@60fps */
	FIMC_IS_SENSOR_CFG_EXT(2072, 1166, 60, 24, 6, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 2072x1008@30fps */
	FIMC_IS_SENSOR_CFG_EXT(2072, 1008, 30, 24, 7, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 1008x754@120fps */
	FIMC_IS_SENSOR_CFG_EXT(1008, 754, 120, 24, 8, CSI_DATA_LANES_4, 1096, 0, 0, 0),
	/* 1008x568@120fps */
	FIMC_IS_SENSOR_CFG_EXT(1008, 568, 120, 24, 9, CSI_DATA_LANES_4, 1096, 0, 0, 0),
};

static struct fimc_is_vci vci_module_3l2[] = {
	{
		.pixelformat = V4L2_PIX_FMT_SBGGR10,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_USER}, {3, 0}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR12,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_USER}, {3, 0}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR16,
		.config = {{0, HW_FORMAT_RAW10}, {1, HW_FORMAT_UNKNOWN}, {2, HW_FORMAT_USER}, {3, 0}}
	}
};

static const struct v4l2_subdev_core_ops core_ops = {
	.init = sensor_module_init,
	.g_ctrl = sensor_module_g_ctrl,
	.s_ctrl = sensor_module_s_ctrl,
	.g_ext_ctrls = sensor_module_g_ext_ctrls,
	.s_ext_ctrls = sensor_module_s_ext_ctrls,
	.ioctl = sensor_module_ioctl,
	.log_status = sensor_module_log_status,
};

static const struct v4l2_subdev_video_ops video_ops = {
	.s_stream = sensor_module_s_stream,
	.s_parm = sensor_module_s_param
};

static const struct v4l2_subdev_pad_ops pad_ops = {
	.set_fmt = sensor_module_s_format
};

static const struct v4l2_subdev_ops subdev_ops = {
	.core = &core_ops,
	.video = &video_ops,
	.pad = &pad_ops
};

static int sensor_module_3l2_power_setpin(struct device *dev,
	struct exynos_platform_fimc_is_module *pdata)
{
	struct device_node *dnode;
	int gpio_reset = 0;
	int gpio_none = 0;
	int gpio_camio_1p8_en = 0;
	int gpio_camaf_2p8_en = 0;
	int gpio_cam_core_en = 0;

	BUG_ON(!dev);

	dnode = dev->of_node;

	dev_info(dev, "%s E v4\n", __func__);

	gpio_reset = of_get_named_gpio(dnode, "gpio_reset", 0);
	if (gpio_is_valid(gpio_reset)) {
		gpio_request_one(gpio_reset, GPIOF_OUT_INIT_LOW, "CAM_GPIO_OUTPUT_LOW");
		gpio_free(gpio_reset);
	} else {
		dev_err(dev, "failed to get PIN_RESET\n");
		return -EINVAL;
	}

	gpio_cam_core_en = of_get_named_gpio(dnode, "gpio_cam_core_en", 0);
	if (gpio_is_valid(gpio_cam_core_en)) {
		gpio_request_one(gpio_cam_core_en, GPIOF_OUT_INIT_LOW, "CAM_CORE_INIT_LOW");
		gpio_free(gpio_cam_core_en);
	} else {
		dev_err(dev, "failed to get gpio_cam_core_en\n");
	}

	gpio_camio_1p8_en = of_get_named_gpio(dnode, "gpio_camio_1p8_en", 0);
	if (gpio_is_valid(gpio_camio_1p8_en)) {
		gpio_request_one(gpio_camio_1p8_en, GPIOF_OUT_INIT_LOW, "CAM_IO_1P8_INIT_LOW");
		gpio_free(gpio_camio_1p8_en);
	} else {
		dev_err(dev, "failed to get gpio_camio_1p8_en\n");
	}

	gpio_camaf_2p8_en = of_get_named_gpio(dnode, "gpio_camaf_2p8_en", 0);
	if (gpio_is_valid(gpio_camaf_2p8_en)) {
		gpio_request_one(gpio_camaf_2p8_en, GPIOF_OUT_INIT_LOW, "CAM_AF_2P8_INIT_LOW");
		gpio_free(gpio_camaf_2p8_en);
	} else {
		dev_err(dev, "failed to get gpio_camaf_2p8_en\n");
	}

	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF);

	SET_PIN_INIT(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_OFF);

	/* BACK CAEMRA - POWER ON */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "sen_rst low", PIN_OUTPUT, 0, 0);

	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_camio_1p8_en, "camio_1p8_en", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_cam_core_en, "cam_core_en", PIN_OUTPUT, 1, 1000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_camaf_2p8_en, "camaf_2p8_en", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "sen_rst high", PIN_OUTPUT, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "pin", PIN_FUNCTION, 2, 3000);

	/* BACK CAEMRA - POWER OFF */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_camaf_2p8_en, "camaf_2p8_en", PIN_OUTPUT, 0, 1000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "pin", PIN_FUNCTION, 1, 10);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_reset, "sen_rst low", PIN_OUTPUT, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_camio_1p8_en, "camio_1p8_en", PIN_OUTPUT, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_cam_core_en, "cam_core_en", PIN_OUTPUT, 0, 0);

	/* READ_ROM - POWER ON */
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_ON, gpio_camaf_2p8_en, "camaf_2p8_en", PIN_OUTPUT, 1, 2000);
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_ON, gpio_camio_1p8_en, "camio_1p8_en", PIN_OUTPUT, 1, 2000);

	/* READ_ROM - POWER OFF */
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_OFF, gpio_camaf_2p8_en, "camaf_2p8_en", PIN_OUTPUT, 0, 10);
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_OFF, gpio_camio_1p8_en, "camio_1p8_en", PIN_OUTPUT, 0, 0);

	dev_info(dev, "%s X v4\n", __func__);

	return 0;
}

int sensor_module_3l2_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct fimc_is_core *core;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor *device;
	struct sensor_open_extended *ext;
	struct exynos_platform_fimc_is_module *pdata;
	struct device *dev;

	BUG_ON(!fimc_is_dev);

	core = (struct fimc_is_core *)dev_get_drvdata(fimc_is_dev);
	if (!core) {
		probe_info("core device is not yet probed");
		return -EPROBE_DEFER;
	}

	dev = &pdev->dev;

	fimc_is_module_parse_dt(dev, sensor_module_3l2_power_setpin);

	pdata = dev_get_platdata(dev);
	device = &core->sensor[pdata->id];

	subdev_module = kzalloc(sizeof(struct v4l2_subdev), GFP_KERNEL);
	if (!subdev_module) {
		probe_err("subdev_module is NULL");
		ret = -ENOMEM;
		goto p_err;
	}

	probe_info("%s pdta->id(%d), module_enum id = %d \n", __func__, pdata->id, atomic_read(&device->module_count));
	module = &device->module_enum[atomic_read(&device->module_count)];
	atomic_inc(&device->module_count);
	clear_bit(FIMC_IS_MODULE_GPIO_ON, &module->state);
	module->pdata = pdata;
	module->dev = dev;
	module->sensor_id = SENSOR_NAME_S5K3L2;
	module->subdev = subdev_module;
	module->device = pdata->id;
	module->client = NULL;
	module->active_width = (4128 + 16);
	module->active_height = (3096 + 10);
	module->margin_left = 0;
	module->margin_right = 0;
	module->margin_top = 0;
	module->margin_bottom = 0;
	module->pixel_width = module->active_width;
	module->pixel_height = module->active_height;
	module->max_framerate = 120;
	module->position = pdata->position;
	module->mode = CSI_MODE_CH0_ONLY;
	module->lanes = CSI_DATA_LANES_4;
	module->bitwidth = 10;
	module->vcis = ARRAY_SIZE(vci_module_3l2);
	module->vci = vci_module_3l2;
	module->sensor_maker = "SLSI";
	module->sensor_name = "S5K3L2";
	module->setfile_name = "setfile_3l2.bin";
	module->cfgs = ARRAY_SIZE(config_module_3l2);
	module->cfg = config_module_3l2;
	module->ops = NULL;
	/* Sensor peri */
	module->private_data = kzalloc(sizeof(struct fimc_is_device_sensor_peri), GFP_KERNEL);
	if (!module->private_data) {
		probe_err("fimc_is_device_sensor_peri is NULL");
		ret = -ENOMEM;
		goto p_err;
	}
	fimc_is_sensor_peri_probe((struct fimc_is_device_sensor_peri*)module->private_data);
	PERI_SET_MODULE(module);

	ext = &module->ext;
	ext->mipi_lane_num = module->lanes;

	ext->sensor_con.product_name = module->sensor_id;
	ext->sensor_con.peri_type = SE_I2C;
	ext->sensor_con.peri_setting.i2c.channel = pdata->sensor_i2c_ch;
	ext->sensor_con.peri_setting.i2c.slave_address = pdata->sensor_i2c_addr;
	ext->sensor_con.peri_setting.i2c.speed = 400000;

	if (pdata->af_product_name !=  ACTUATOR_NAME_NOTHING) {
		ext->actuator_con.product_name = pdata->af_product_name;
		ext->actuator_con.peri_type = SE_I2C;
		ext->actuator_con.peri_setting.i2c.channel = pdata->af_i2c_ch;
		ext->actuator_con.peri_setting.i2c.slave_address = pdata->af_i2c_addr;
		ext->actuator_con.peri_setting.i2c.speed = 400000;
	}

	if (pdata->flash_product_name != FLADRV_NAME_NOTHING) {
		ext->flash_con.product_name = pdata->flash_product_name;
		ext->flash_con.peri_type = SE_GPIO;
		ext->flash_con.peri_setting.gpio.first_gpio_port_no = pdata->flash_first_gpio;
		ext->flash_con.peri_setting.gpio.second_gpio_port_no = pdata->flash_second_gpio;
	}

	ext->from_con.product_name = FROMDRV_NAME_NOTHING;

	if (pdata->preprocessor_product_name != PREPROCESSOR_NAME_NOTHING) {
		ext->preprocessor_con.product_name = pdata->preprocessor_product_name;
		ext->preprocessor_con.peri_info0.valid = true;
		ext->preprocessor_con.peri_info0.peri_type = SE_SPI;
		ext->preprocessor_con.peri_info0.peri_setting.spi.channel = pdata->preprocessor_spi_channel;
		ext->preprocessor_con.peri_info1.valid = true;
		ext->preprocessor_con.peri_info1.peri_type = SE_I2C;
		ext->preprocessor_con.peri_info1.peri_setting.i2c.channel = pdata->preprocessor_i2c_ch;
		ext->preprocessor_con.peri_info1.peri_setting.i2c.slave_address = pdata->preprocessor_i2c_addr;
		ext->preprocessor_con.peri_info1.peri_setting.i2c.speed = 400000;
		ext->preprocessor_con.peri_info2.valid = true;
		ext->preprocessor_con.peri_info2.peri_type = SE_DMA;
		ext->preprocessor_con.peri_info2.peri_setting.dma.channel = FLITE_ID_D;
	} else {
		ext->preprocessor_con.product_name = pdata->preprocessor_product_name;
	}

	if (pdata->ois_product_name != OIS_NAME_NOTHING) {
		ext->ois_con.product_name = pdata->ois_product_name;
		ext->ois_con.peri_type = SE_I2C;
		ext->ois_con.peri_setting.i2c.channel = pdata->ois_i2c_ch;
		ext->ois_con.peri_setting.i2c.slave_address = pdata->ois_i2c_addr;
		ext->ois_con.peri_setting.i2c.speed = 400000;
	} else {
		ext->ois_con.product_name = pdata->ois_product_name;
		ext->ois_con.peri_type = SE_NULL;
	}

	v4l2_subdev_init(subdev_module, &subdev_ops);

	v4l2_set_subdevdata(subdev_module, module);
	v4l2_set_subdev_hostdata(subdev_module, device);
	snprintf(subdev_module->name, V4L2_SUBDEV_NAME_SIZE, "sensor-subdev.%d", module->sensor_id);

	probe_info("%s done\n", __func__);

p_err:
	return ret;
}

static int sensor_module_3l2_remove(struct platform_device *pdev)
{
        int ret = 0;

        info("%s\n", __func__);

        return ret;
}

static const struct of_device_id exynos_fimc_is_sensor_module_3l2_match[] = {
	{
		.compatible = "samsung,sensor-module-3l2",
	},
	{},
};
MODULE_DEVICE_TABLE(of, exynos_fimc_is_sensor_module_3l2_match);

static struct platform_driver sensor_module_3l2_driver = {
	.probe  = sensor_module_3l2_probe,
	.remove = sensor_module_3l2_remove,
	.driver = {
		.name   = "FIMC-IS-SENSOR-MODULE-3L2",
		.owner  = THIS_MODULE,
		.of_match_table = exynos_fimc_is_sensor_module_3l2_match,
	}
};

module_platform_driver(sensor_module_3l2_driver);
