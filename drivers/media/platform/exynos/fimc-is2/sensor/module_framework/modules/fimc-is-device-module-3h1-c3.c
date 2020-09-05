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

#define S5K3H1_PDAF_MAXWIDTH	0 /* MAX witdh size */
#define S5K3H1_PDAF_MAXHEIGHT	0 /* MAX height size */
#define S5K3H1_PDAF_ELEMENT	0 /* V4L2_PIX_FMT_SBGGR16 */

#define S5K3H1_MIPI_MAXWIDTH	3264 /* MAX width size */
#define S5K3H1_MIPI_MAXHEIGHT	6 /* MAX height size */
#define S5K3H1_MIPI_ELEMENT	1 /* V4L2_PIX_FMT_SBGGR16 */

static struct fimc_is_sensor_cfg config_module_3h1_c3[] = {
	/* 3264X2448@30fps */
	FIMC_IS_SENSOR_CFG_EXT(3264, 2448, 30, 15, 0, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 3264, 6), 0),
	/* 2448X2448@30fps */
	FIMC_IS_SENSOR_CFG_EXT(2448, 2448, 30, 15, 1, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 2448, 8), 0),
	/* 3264X1836@30fps */
	FIMC_IS_SENSOR_CFG_EXT(3264, 1836, 30, 15, 2, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 3264, 6), 0),
	/* 1632X918@60fps */
	FIMC_IS_SENSOR_CFG_EXT(1632, 918, 60, 7, 3, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 1632, 4), 0),
	/* 1632X918@30fps */
	FIMC_IS_SENSOR_CFG_EXT(1632, 918, 30, 7, 4, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 1632, 4), 0),
	/* 1632X1224@30fps */
	FIMC_IS_SENSOR_CFG_EXT(1632, 1224, 30, 7, 5, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 1632, 4), 0),
	/* 1224X1224@30fps */
	FIMC_IS_SENSOR_CFG_EXT(1224, 1224, 30, 7, 6, CSI_DATA_LANES_2, 2145, 0, SET_VC(VC_MIPI_STAT, 1224, 4), 0),
	/* 800X600@30fps */
	FIMC_IS_SENSOR_CFG(800, 600, 120, 136, 7, CSI_DATA_LANES_2),
	/* 752X1328@15fps */
	FIMC_IS_SENSOR_CFG(752, 1328, 15, 15, 8, CSI_DATA_LANES_2),
	/* 376X664@110fps */
	FIMC_IS_SENSOR_CFG(376, 664, 110, 15, 9, CSI_DATA_LANES_2),
};

static struct fimc_is_vci vci_module_3h1_c3[] = {
	{
		.pixelformat = V4L2_PIX_FMT_SBGGR10,
		.config = {{0, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {1, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {0, HW_FORMAT_USER, VCI_DMA_INTERNAL},
			   {3, 0, VCI_DMA_NORMAL}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR12,
		.config = {{0, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {1, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {0, HW_FORMAT_USER, VCI_DMA_INTERNAL},
			   {3, 0, VCI_DMA_NORMAL}}
	}, {
		.pixelformat = V4L2_PIX_FMT_SBGGR16,
		.config = {{0, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {1, HW_FORMAT_RAW10, VCI_DMA_NORMAL},
			   {0, HW_FORMAT_USER, VCI_DMA_INTERNAL},
			   {3, 0, VCI_DMA_NORMAL}}
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
	//.s_mbus_fmt = sensor_module_s_format,
};

static const struct v4l2_subdev_pad_ops pad_ops = {
	.set_fmt = sensor_module_s_format
};
static const struct v4l2_subdev_ops subdev_ops = {
	.core = &core_ops,
	.video = &video_ops,
	.pad = &pad_ops,
};

static int sensor_module_3h1_c3_power_setpin(struct platform_device *pdev,
		struct exynos_platform_fimc_is_module *pdata)
{
	struct device *dev;
	struct device_node *dnode;
	int gpio_none = 0, gpio_reset = 0, gpio_prep_reset = 0;

	BUG_ON(!pdev);

	dev = &pdev->dev;
	dnode = dev->of_node;

	gpio_prep_reset = of_get_named_gpio(dnode, "gpio_prep_reset", 0);
	if (!gpio_is_valid(gpio_prep_reset)) {
		dev_err(dev, "%s: failed to get main comp reset gpio\n", __func__);
		return -EINVAL;
	} else {
		gpio_request_one(gpio_prep_reset, GPIOF_OUT_INIT_LOW, "CAM_GPIO_OUTPUT_LOW");
		gpio_free(gpio_prep_reset);
	}

	gpio_reset = of_get_named_gpio(dnode, "gpio_reset", 0);
	if (!gpio_is_valid(gpio_reset)) {
		dev_err(dev, "%s: failed to get PIN_RESET\n", __func__);
		return -EINVAL;
	} else {
		gpio_request_one(gpio_reset, GPIOF_OUT_INIT_LOW, "CAM_GPIO_OUTPUT_LOW");
		gpio_free(gpio_reset);
	}

	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF);
#ifdef CONFIG_PREPROCESSOR_STANDBY_USE
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF);
#ifdef CONFIG_SENSOR_RETENTION_USE
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON);
#endif
#endif
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_ON);
	SET_PIN_INIT(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_OFF);

	/********** FRONT CAMERA  - POWER ON **********/
	/* Companion I2C pull up source */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDIO_1.8V_CAM", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDIO_1.8V_COMP", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 1, 0);
	SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 1, 0, 1050000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 1, 1000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_reset, "rst_high", PIN_OUTPUT, 1, 0);
	/* Mclock enable */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_none, "pin", PIN_FUNCTION, 2, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_ON, gpio_prep_reset, "prep_rst high", PIN_OUTPUT, 1, 9000);

	/*********** FRONT CAMERA  - POWER OFF **********/
	/* Mclock disable */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "pin", PIN_FUNCTION, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "pin", PIN_FUNCTION, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "pin", PIN_FUNCTION, 0, 1);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_reset, "rst_low", PIN_OUTPUT, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_prep_reset, "prep_rst low", PIN_OUTPUT, 0, 10);
	/* Companion I2C pull up source */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDIO_1.8V_CAM", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDIO_1.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_OFF, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 0, 0);

#ifdef CONFIG_PREPROCESSOR_STANDBY_USE
#ifdef CAMERA_PARALLEL_RETENTION_SEQUENCE
	/********** STANDBY DISABLE **********/
	/* Sensor */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_SENSOR, gpio_none, "VDDIO_1.8V_CAM", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_SENSOR, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_SENSOR, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_SENSOR, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_SENSOR, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 1, 0);
	/* Companion */
	SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_PREPROCESSOR, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 1, 2200, 1050000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_PREPROCESSOR, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_PREPROCESSOR, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_PREPROCESSOR, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF_PREPROCESSOR, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 1, 100);
	/* MCLK & reset */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_reset, "rst_high", PIN_OUTPUT, 1, 40);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "pin", PIN_FUNCTION, 2, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_prep_reset, "prep_rst high", PIN_OUTPUT, 1, 2000);
#else /* !CAMERA_PARALLEL_RETENTION_SEQUENCE */
	/********** STANDBY DISABLE **********/
	/* Sensor */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDIO_1.8V_CAM", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 1, 0);
	/* Companion */
	SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 1, 2200, 1050000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 1, 100);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 1, 100);
	/* MCLK & reset */
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_reset, "rst_high", PIN_OUTPUT, 1, 40);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_none, "pin", PIN_FUNCTION, 2, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_OFF, gpio_prep_reset, "prep_rst high", PIN_OUTPUT, 1, 2000);
#endif /* CAMERA_PARALLEL_RETENTION_SEQUENCE */
	/********** STANDBY ENABLE **********/
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "pin", PIN_FUNCTION, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "pin", PIN_FUNCTION, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "pin", PIN_FUNCTION, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_reset, "rst_low", PIN_OUTPUT, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_prep_reset, "prep_rst low", PIN_OUTPUT, 0, 10);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 1, 0, 700000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDIO_1.8V_CAM", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_STANDBY_ON, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 0, 0);
#endif /* CONFIG_PREPROCESSOR_STANDBY_USE */

#ifdef CONFIG_SENSOR_RETENTION_USE
	/********** SENSOR RETENTION ENABLE **********/
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "pin", PIN_FUNCTION, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "pin", PIN_FUNCTION, 1, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "pin", PIN_FUNCTION, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_reset, "rst_low", PIN_OUTPUT, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_prep_reset, "prep_rst low", PIN_OUTPUT, 0, 10);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDD_NORET_0.9V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDD_CORE_0.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDD_CORE_1.0V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDA_1.8V_COMP", PIN_REGULATOR, 0, 0);
	SET_PIN_VOLTAGE(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDD_RET_1.0V_COMP", PIN_REGULATOR, 1, 0, 700000);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDD_VTCAM_AF_2.8V", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDD_1.2V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDA_2.9V_VT", PIN_REGULATOR, 0, 0);
	SET_PIN(pdata, SENSOR_SCENARIO_NORMAL, GPIO_SCENARIO_SENSOR_RETENTION_ON, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 0, 0);
#endif

	/******************** READ_ROM - POWER ON ********************/
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_ON, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 1, 0);

	/******************** READ_ROM - POWER OFF ********************/
	SET_PIN(pdata, SENSOR_SCENARIO_READ_ROM, GPIO_SCENARIO_OFF, gpio_none, "VDDIO_1.8V_VT", PIN_REGULATOR, 0, 0);

	dev_info(dev, "%s X v4\n", __func__);

	return 0;
}

int sensor_module_3h1_c3_probe(struct platform_device *pdev)
{
	int ret = 0;
	struct fimc_is_core *core;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_module_enum *module;
	struct fimc_is_device_sensor *device;
	struct sensor_open_extended *ext;
	struct exynos_platform_fimc_is_module *pdata;
	struct device *dev;
	int ch, vc_idx;
	struct pinctrl_state *s;

	BUG_ON(!fimc_is_dev);

	core = (struct fimc_is_core *)dev_get_drvdata(fimc_is_dev);
	if (!core) {
		probe_info("core device is not yet probed");
		return -EPROBE_DEFER;
	}

	dev = &pdev->dev;

	fimc_is_sensor_module_parse_dt(pdev, sensor_module_3h1_c3_power_setpin);

	pdata = dev_get_platdata(dev);
	device = &core->sensor[pdata->id];

	subdev_module = kzalloc(sizeof(struct v4l2_subdev), GFP_KERNEL);
	if (!subdev_module) {
		probe_err("subdev_module is NULL");
		ret = -ENOMEM;
		goto p_err;
	}

	probe_info("%s pdta->id(%d), module_enum id = %d \n", __func__, pdata->id,
				atomic_read(&device->module_count));
	module = &device->module_enum[atomic_read(&device->module_count)];
	atomic_inc(&device->module_count);
	clear_bit(FIMC_IS_MODULE_GPIO_ON, &module->state);
	module->pdata = pdata;
	module->dev = dev;
	module->sensor_id = SENSOR_NAME_S5K3H1;
	module->subdev = subdev_module;
	module->device = pdata->id;
	module->client = NULL;
	module->active_width = 3280;
	module->active_height = 2464;
	module->margin_left = 0;
	module->margin_right = 0;
	module->margin_top = 0;
	module->margin_bottom = 0;
	module->pixel_width = module->active_width + 0;
	module->pixel_height = module->active_height + 0;
	module->max_framerate = 120;
	module->position = pdata->position;
	module->mode = CSI_MODE_DT_ONLY;
	module->lanes = CSI_DATA_LANES_2;
	module->bitwidth = 10;
	module->vcis = ARRAY_SIZE(vci_module_3h1_c3);
	module->vci = vci_module_3h1_c3;
	module->sensor_maker = "SLSI";
	module->sensor_name = "S5K3H1";
	module->setfile_name = "setfile_3h1.bin";
	module->cfgs = ARRAY_SIZE(config_module_3h1_c3);
	module->cfg = config_module_3h1_c3;
	module->ops = NULL;

	for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++)
		module->internal_vc[ch] = pdata->internal_vc[ch];

	for (vc_idx = 0; vc_idx < 2; vc_idx++) {
		switch (vc_idx) {
		case VC_BUF_DATA_TYPE_SENSOR_STAT1:
			module->vc_max_size[vc_idx].width = S5K3H1_PDAF_MAXWIDTH;
			module->vc_max_size[vc_idx].height = S5K3H1_PDAF_MAXHEIGHT;
			module->vc_max_size[vc_idx].element_size = S5K3H1_PDAF_ELEMENT;
			break;
		case VC_BUF_DATA_TYPE_GENERAL_STAT1:
			module->vc_max_size[vc_idx].width = S5K3H1_MIPI_MAXWIDTH;
			module->vc_max_size[vc_idx].height = S5K3H1_MIPI_MAXHEIGHT;
			module->vc_max_size[vc_idx].element_size = S5K3H1_MIPI_ELEMENT;
			break;
		}
	}

	/* Sensor peri */
	module->private_data = kzalloc(sizeof(struct fimc_is_device_sensor_peri), GFP_KERNEL);
	if (!module->private_data) {
		probe_err("fimc_is_device_sensor_peri is NULL");
		ret = -ENOMEM;
		//kfree(subdev_module);
		goto p_err;
	}
	fimc_is_sensor_peri_probe((struct fimc_is_device_sensor_peri*)module->private_data);
	PERI_SET_MODULE(module);

	ext = &module->ext;
	ext->mipi_lane_num = module->lanes;
	ext->I2CSclk = 0;

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

	s = pinctrl_lookup_state(pdata->pinctrl, "release");

	if (pinctrl_select_state(pdata->pinctrl, s) < 0) {
		probe_err("pinctrl_select_state is fail\n");
		goto p_err;
	}

	probe_info("%s done\n", __func__);

p_err:
	return ret;
}

static int sensor_module_3h1_c3_remove(struct platform_device *pdev)
{
	int ret = 0;

	info("%s\n", __func__);

	return ret;
}

static const struct of_device_id exynos_fimc_is_sensor_module_3h1_c3_match[] = {
	{
		.compatible = "samsung,sensor-module-3h1-c3",
	},
	{},
};
MODULE_DEVICE_TABLE(of, exynos_fimc_is_sensor_module_3h1_c3_match);

static struct platform_driver sensor_module_3h1_c3_driver = {
	.probe  = sensor_module_3h1_c3_probe,
	.remove = sensor_module_3h1_c3_remove,
	.driver = {
		.name   = "FIMC-IS-SENSOR-MODULE-3H1-C3",
		.owner  = THIS_MODULE,
		.of_match_table = exynos_fimc_is_sensor_module_3h1_c3_match,
	}
};

module_platform_driver(sensor_module_3h1_c3_driver);
