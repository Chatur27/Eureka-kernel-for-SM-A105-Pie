/*
 * Samsung EXYNOS FIMC-IS (Imaging Subsystem) driver
 *
 * Copyright (C) 2014 Samsung Electronics Co., Ltd.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>

#include <linux/videodev2.h>
#include <linux/videodev2_exynos_camera.h>

#include "fimc-is-device-ischain.h"
#include "fimc-is-control-sensor.h"
#include "fimc-is-device-sensor-peri.h"
#include "fimc-is-interface-sensor.h"

/* helper functions */
struct fimc_is_module_enum *get_subdev_module_enum(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_module_enum *module = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

p_err:
	return module;
}

static struct fimc_is_device_sensor *get_device_sensor(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_module_enum *module;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_device_sensor *device;

	if (unlikely(!itf)) {
		err("%s, NULL sensor interface", __func__);
		return NULL;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, failed to get sensor_peri's module", __func__);
		return NULL;
	}

	subdev_module = module->subdev;
	if (!subdev_module) {
		err("%s, module's subdev was not probed", __func__);
		return NULL;
	}

	device = v4l2_get_subdev_hostdata(subdev_module);

	return device;
}

struct fimc_is_device_csi *get_subdev_csi(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_module_enum *module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct v4l2_subdev *subdev_module;
	struct fimc_is_device_csi *csi = NULL;
	struct fimc_is_device_sensor *device;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		csi = NULL;
		goto p_err;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	subdev_module = module->subdev;
	if (!subdev_module) {
		err("module is not probed");
		subdev_module = NULL;
		goto p_err;
	}

	device = v4l2_get_subdev_hostdata(subdev_module);

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(device->subdev_csi);
	if (unlikely(!csi)) {
		err("%s, csi in is NULL", __func__);
		csi = NULL;
		goto p_err;
	}

p_err:
	return csi;
}

struct fimc_is_actuator *get_subdev_actuator(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_actuator *actuator = NULL;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		actuator = NULL;
		goto p_err;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	if (unlikely(!sensor_peri->subdev_actuator)) {
		err("%s, subdev module in is NULL", __func__);
		actuator = NULL;
		goto p_err;
	}

	actuator = (struct fimc_is_actuator *)v4l2_get_subdevdata(sensor_peri->subdev_actuator);
	if (unlikely(!actuator)) {
		err("%s, module in is NULL", __func__);
		actuator = NULL;
		goto p_err;
	}

p_err:
	return actuator;
}

int sensor_get_ctrl(struct fimc_is_sensor_interface *itf,
			u32 ctrl_id, u32 *val)
{
	int ret = 0;
	struct fimc_is_device_sensor *device = NULL;
	struct fimc_is_module_enum *module = NULL;
	struct v4l2_subdev *subdev_module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct v4l2_control ctrl;

	if (unlikely(!itf)) {
		err("%s, interface in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);
	BUG_ON(!val);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	subdev_module = module->subdev;
	if (!subdev_module) {
		err("module is not probed");
		subdev_module = NULL;
		goto p_err;
	}

	device = v4l2_get_subdev_hostdata(subdev_module);
	if (unlikely(!device)) {
		err("%s, device in is NULL", __func__);
		ret = -EINVAL;
		goto p_err;
	}

	ctrl.id = ctrl_id;
	ctrl.value = -1;
	ret = fimc_is_sensor_g_ctrl(device, &ctrl);
	*val = (u32)ctrl.value;
	if (ret < 0) {
		err("err!!! ret(%d), return_value(%d)", ret, *val);
		ret = -EINVAL;
		goto p_err;
	}

p_err:
	return ret;
}

/* if target param has only one value(such as CIS_SMIA mode or flash_intensity),
   then set value to long_val, will be ignored short_val */
int set_interface_param(struct fimc_is_sensor_interface *itf,
			enum itf_cis_interface mode,
			enum itf_param_type target,
			u32 index,
			u32 long_val,
			u32 short_val)
{
	int ret = 0;
	u32 val[MAX_EXPOSURE_GAIN_PER_FRAME] = {0};

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	if (mode == ITF_CIS_SMIA) {
		val[EXPOSURE_GAIN_INDEX] = long_val;
		val[LONG_EXPOSURE_GAIN_INDEX] = 0;
		val[SHORT_EXPOSURE_GAIN_INDEX] = 0;
	} else if (mode == ITF_CIS_SMIA_WDR) {
		val[EXPOSURE_GAIN_INDEX] = 0;
		val[LONG_EXPOSURE_GAIN_INDEX] = long_val;;
		val[SHORT_EXPOSURE_GAIN_INDEX] = short_val;
	} else {
		pr_err("[%s] invalid mode (%d)\n", __func__, mode);
		ret = -EINVAL;
		goto p_err;
	}

	if (index >= NUM_FRAMES) {
		pr_err("[%s] invalid frame index (%d)\n", __func__, index);
		ret = -EINVAL;
		goto p_err;
	}

	switch (target) {
	case ITF_CIS_PARAM_TOTAL_GAIN:
		itf->total_gain[EXPOSURE_GAIN_INDEX][index] = val[EXPOSURE_GAIN_INDEX];
		itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][index] = val[LONG_EXPOSURE_GAIN_INDEX];
		itf->total_gain[SHORT_EXPOSURE_GAIN_INDEX][index] = val[SHORT_EXPOSURE_GAIN_INDEX];
		dbg_sensor(1, "%s: total gain[%d] %d %d %d\n", __func__, index,
				val[EXPOSURE_GAIN_INDEX],
				val[LONG_EXPOSURE_GAIN_INDEX],
				val[SHORT_EXPOSURE_GAIN_INDEX]);
		dbg_sensor(1, "%s: total gain [0]:%d [1]:%d [2]:%d\n", __func__,
				itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][0],
				itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][1],
				itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][2]);
		break;
	case ITF_CIS_PARAM_ANALOG_GAIN:
		itf->analog_gain[EXPOSURE_GAIN_INDEX][index] = val[EXPOSURE_GAIN_INDEX];
		itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][index] = val[LONG_EXPOSURE_GAIN_INDEX];
		itf->analog_gain[SHORT_EXPOSURE_GAIN_INDEX][index] = val[SHORT_EXPOSURE_GAIN_INDEX];
		dbg_sensor(1, "%s: again[%d] %d %d %d\n", __func__, index,
				val[EXPOSURE_GAIN_INDEX],
				val[LONG_EXPOSURE_GAIN_INDEX],
				val[SHORT_EXPOSURE_GAIN_INDEX]);
		dbg_sensor(1, "%s: again [0]:%d [1]:%d [2]:%d\n", __func__,
				itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][0],
				itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][1],
				itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][2]);
		break;
	case ITF_CIS_PARAM_DIGITAL_GAIN:
		itf->digital_gain[EXPOSURE_GAIN_INDEX][index] = val[EXPOSURE_GAIN_INDEX];
		itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][index] = val[LONG_EXPOSURE_GAIN_INDEX];
		itf->digital_gain[SHORT_EXPOSURE_GAIN_INDEX][index] = val[SHORT_EXPOSURE_GAIN_INDEX];
		dbg_sensor(1, "%s: dgain[%d] %d %d %d\n", __func__, index,
				val[EXPOSURE_GAIN_INDEX],
				val[LONG_EXPOSURE_GAIN_INDEX],
				val[SHORT_EXPOSURE_GAIN_INDEX]);
		dbg_sensor(1, "%s: dgain [0]:%d [1]:%d [2]:%d\n", __func__,
				itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][0],
				itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][1],
				itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][2]);
		break;
	case ITF_CIS_PARAM_EXPOSURE:
		itf->exposure[EXPOSURE_GAIN_INDEX][index] = val[EXPOSURE_GAIN_INDEX];
		itf->exposure[LONG_EXPOSURE_GAIN_INDEX][index] = val[LONG_EXPOSURE_GAIN_INDEX];
		itf->exposure[SHORT_EXPOSURE_GAIN_INDEX][index] = val[SHORT_EXPOSURE_GAIN_INDEX];
		dbg_sensor(1, "%s: expo[%d] %d %d %d\n", __func__, index,
				val[EXPOSURE_GAIN_INDEX],
				val[LONG_EXPOSURE_GAIN_INDEX],
				val[SHORT_EXPOSURE_GAIN_INDEX]);
		dbg_sensor(1, "%s: expo [0]:%d [1]:%d [2]:%d\n", __func__,
				itf->exposure[LONG_EXPOSURE_GAIN_INDEX][0],
				itf->exposure[LONG_EXPOSURE_GAIN_INDEX][1],
				itf->exposure[LONG_EXPOSURE_GAIN_INDEX][2]);
		break;
	case ITF_CIS_PARAM_FLASH_INTENSITY:
		itf->flash_intensity[index] = long_val;
		break;
	default:
		pr_err("[%s] invalid CIS_SMIA mode (%d)\n", __func__, mode);
		ret = -EINVAL;
		goto p_err;
		break;
	}

p_err:
	return ret;
}

int get_interface_param(struct fimc_is_sensor_interface *itf,
			enum itf_cis_interface mode,
			enum itf_param_type target,
			u32 index,
			u32 *long_val,
			u32 *short_val)
{
	int ret = 0;
	u32 val[MAX_EXPOSURE_GAIN_PER_FRAME] = {0};

	BUG_ON(!itf);
	BUG_ON(!long_val);
	BUG_ON(!short_val);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	if (index >= NUM_FRAMES) {
		pr_err("[%s] invalid frame index (%d)\n", __func__, index);
		ret = -EINVAL;
		goto p_err;
	}

	switch (target) {
	case ITF_CIS_PARAM_TOTAL_GAIN:
		val[EXPOSURE_GAIN_INDEX] = itf->total_gain[EXPOSURE_GAIN_INDEX][index];
		val[LONG_EXPOSURE_GAIN_INDEX] = itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][index];
		val[SHORT_EXPOSURE_GAIN_INDEX] = itf->total_gain[SHORT_EXPOSURE_GAIN_INDEX][index];
		dbg_sensor(2, "%s: total gain[%d] %d %d %d\n", __func__, index,
			val[EXPOSURE_GAIN_INDEX],
			val[LONG_EXPOSURE_GAIN_INDEX],
			val[SHORT_EXPOSURE_GAIN_INDEX]);
		break;
	case ITF_CIS_PARAM_ANALOG_GAIN:
		val[EXPOSURE_GAIN_INDEX] = itf->analog_gain[EXPOSURE_GAIN_INDEX][index];
		val[LONG_EXPOSURE_GAIN_INDEX] = itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][index];
		val[SHORT_EXPOSURE_GAIN_INDEX] = itf->analog_gain[SHORT_EXPOSURE_GAIN_INDEX][index];
		dbg_sensor(2, "%s: again[%d] %d %d %d\n", __func__, index,
			val[EXPOSURE_GAIN_INDEX],
			val[LONG_EXPOSURE_GAIN_INDEX],
			val[SHORT_EXPOSURE_GAIN_INDEX]);
		break;
	case ITF_CIS_PARAM_DIGITAL_GAIN:
		val[EXPOSURE_GAIN_INDEX] = itf->digital_gain[EXPOSURE_GAIN_INDEX][index];
		val[LONG_EXPOSURE_GAIN_INDEX] = itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][index];
		val[SHORT_EXPOSURE_GAIN_INDEX] = itf->digital_gain[SHORT_EXPOSURE_GAIN_INDEX][index];
		dbg_sensor(2, "%s: dgain[%d] %d %d %d\n", __func__, index,
			val[EXPOSURE_GAIN_INDEX],
			val[LONG_EXPOSURE_GAIN_INDEX],
			val[SHORT_EXPOSURE_GAIN_INDEX]);
		break;
	case ITF_CIS_PARAM_EXPOSURE:
		val[EXPOSURE_GAIN_INDEX] = itf->exposure[EXPOSURE_GAIN_INDEX][index];
		val[LONG_EXPOSURE_GAIN_INDEX] = itf->exposure[LONG_EXPOSURE_GAIN_INDEX][index];
		val[SHORT_EXPOSURE_GAIN_INDEX] = itf->exposure[SHORT_EXPOSURE_GAIN_INDEX][index];
		dbg_sensor(2, "%s: exposure[%d] %d %d %d\n", __func__, index,
			val[EXPOSURE_GAIN_INDEX],
			val[LONG_EXPOSURE_GAIN_INDEX],
			val[SHORT_EXPOSURE_GAIN_INDEX]);
		break;
	case ITF_CIS_PARAM_FLASH_INTENSITY:
		val[EXPOSURE_GAIN_INDEX] = itf->flash_intensity[index];
		val[LONG_EXPOSURE_GAIN_INDEX] = itf->flash_intensity[index];
		break;
	default:
		pr_err("[%s] invalid CIS_SMIA mode (%d)\n", __func__, mode);
		ret = -EINVAL;
		goto p_err;
		break;
	}

	if (mode == ITF_CIS_SMIA) {
		*long_val = val[EXPOSURE_GAIN_INDEX];
	} else if (mode == ITF_CIS_SMIA_WDR) {
		*long_val = val[LONG_EXPOSURE_GAIN_INDEX];
		*short_val = val[SHORT_EXPOSURE_GAIN_INDEX];
	} else {
		pr_err("[%s] invalid mode (%d)\n", __func__, mode);
		ret = -EINVAL;
		goto p_err;
	}

p_err:
	return ret;
}

u32 get_vsync_count(struct fimc_is_sensor_interface *itf);

u32 get_frame_count(struct fimc_is_sensor_interface *itf)
{
	u32 frame_count = 0;
#if !defined(CONFIG_USE_SENSOR_GROUP)
	struct fimc_is_device_sensor *device = NULL;
	struct fimc_is_module_enum *module = NULL;
	struct v4l2_subdev *subdev_module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
#endif

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

#if !defined(CONFIG_USE_SENSOR_GROUP)
	if (itf->otf_flag_3aa == true) {
		frame_count = get_vsync_count(itf);
	} else {
		/* Get 3AA active count */
		sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
		BUG_ON(!sensor_peri);

		module = sensor_peri->module;
		if (unlikely(!module)) {
			err("%s, module in is NULL", __func__);
			module = NULL;
			return 0;
		}

		subdev_module = module->subdev;
		if (!subdev_module) {
			err("module is not probed");
			subdev_module = NULL;
			return 0;
		}

		device = v4l2_get_subdev_hostdata(subdev_module);
		if (unlikely(!device)) {
			err("%s, device in is NULL", __func__);
			return 0;
		}

		frame_count = device->ischain->group_3aa.fcount;
	}
#else
	frame_count = get_vsync_count(itf);
#endif

	/* Frame count have to start at 1 */
	if (frame_count == 0)
		frame_count = 1;

	return frame_count;
}

struct fimc_is_sensor_ctl *get_sensor_ctl_from_module(struct fimc_is_sensor_interface *itf,
							u32 frame_count)
{
	u32 index = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	index = frame_count % CAM2P0_UCTL_LIST_SIZE;

	return &sensor_peri->cis.sensor_ctls[index];
}

camera2_sensor_uctl_t *get_sensor_uctl_from_module(struct fimc_is_sensor_interface *itf,
							u32 frame_count)
{
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_ctl = get_sensor_ctl_from_module(itf, frame_count);

	/* TODO: will be moved to report_sensor_done() */
	sensor_ctl->sensor_frame_number = frame_count;

	return &sensor_ctl->cur_cam20_sensor_udctrl;
}

void set_sensor_uctl_valid(struct fimc_is_sensor_interface *itf,
						u32 frame_count)
{
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_ctl = get_sensor_ctl_from_module(itf, frame_count);

	sensor_ctl->is_valid_sensor_udctrl = true;
}

int get_num_of_frame_per_one_3aa(struct fimc_is_sensor_interface *itf,
				u32 *num_of_frame);
int set_exposure(struct fimc_is_sensor_interface *itf,
		enum itf_cis_interface mode,
		u32 long_exp,
		u32 short_exp)
{
	int ret = 0;
	u32 frame_count = 0;
	camera2_sensor_uctl_t *sensor_uctl;
	u32 i = 0;
	u32 num_of_frame = 1;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	frame_count = get_frame_count(itf);
	ret = get_num_of_frame_per_one_3aa(itf, &num_of_frame);

	for (i = 0; i < num_of_frame; i++) {
		sensor_uctl = get_sensor_uctl_from_module(itf, frame_count + i);
		BUG_ON(!sensor_uctl);

		/* set exposure */
		sensor_uctl->exposureTime = fimc_is_sensor_convert_us_to_ns(long_exp);
		if (mode == ITF_CIS_SMIA_WDR) {
		sensor_uctl->longExposureTime = fimc_is_sensor_convert_us_to_ns(long_exp);
		sensor_uctl->shortExposureTime = fimc_is_sensor_convert_us_to_ns(short_exp);
		}
		set_sensor_uctl_valid(itf, frame_count);
	}

	return ret;
}

int set_gain_permile(struct fimc_is_sensor_interface *itf,
		enum itf_cis_interface mode,
		u32 long_total_gain, u32 short_total_gain,
		u32 long_analog_gain, u32 short_analog_gain,
		u32 long_digital_gain, u32 short_digital_gain)
{
	int ret = 0;
	u32 frame_count = 0;
	camera2_sensor_uctl_t *sensor_uctl;
	u32 i = 0;
	u32 num_of_frame = 1;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	frame_count = get_frame_count(itf);

	ret = get_num_of_frame_per_one_3aa(itf, &num_of_frame);

	for (i = 0; i < num_of_frame; i++) {
		sensor_uctl = get_sensor_uctl_from_module(itf, frame_count + i);
		BUG_ON(!sensor_uctl);

		/* set exposure */
		if (mode == ITF_CIS_SMIA) {
			sensor_uctl->sensitivity = DIV_ROUND_UP(long_total_gain, 10);

			sensor_uctl->analogGain = long_analog_gain;
			sensor_uctl->digitalGain = long_digital_gain;

			set_sensor_uctl_valid(itf, frame_count);
		} else if (mode == ITF_CIS_SMIA_WDR) {
			sensor_uctl->sensitivity = DIV_ROUND_UP(long_total_gain, 10);

			/* Caution: short values are setted at analog/digital gain */
			sensor_uctl->analogGain = short_analog_gain;
			sensor_uctl->digitalGain = short_digital_gain;
			sensor_uctl->longAnalogGain = long_analog_gain;
			sensor_uctl->shortAnalogGain = short_analog_gain;
			sensor_uctl->longDigitalGain = long_digital_gain;
			sensor_uctl->shortDigitalGain = short_digital_gain;

			set_sensor_uctl_valid(itf, frame_count);
		} else {
			pr_err("invalid cis interface mode (%d)\n", mode);
			ret = -EINVAL;
		}
	}

	return ret;
}

/* new APIs */
int request_reset_interface(struct fimc_is_sensor_interface *itf,
				u32 exposure,
				u32 total_gain,
				u32 analog_gain,
				u32 digital_gain)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "[%s] exposure(%d), total_gain(%d), a-gain(%d), d-gain(%d)\n", __func__,
			exposure, total_gain, analog_gain, digital_gain);

	itf->vsync_flag = false;
	end_index = itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA;

	for (i = 0; i <= end_index; i++) {
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_TOTAL_GAIN, i, total_gain, total_gain);
		if (ret < 0)
			pr_err("[%s] set_interface_param TOTAL_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, i, analog_gain, analog_gain);
		if (ret < 0)
			pr_err("[%s] set_interface_param ANALOG_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN, i, digital_gain, digital_gain);
		if (ret < 0)
			pr_err("[%s] set_interface_param DIGITAL_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE, i, exposure, exposure);
		if (ret < 0)
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_FLASH_INTENSITY, i, 0, 0);
		if (ret < 0)
			pr_err("[%s] set_interface_param FLASH_INTENSITY fail(%d)\n", __func__, ret);
	}

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			exposure,
			exposure,
			total_gain, total_gain,
			analog_gain, analog_gain,
			digital_gain, digital_gain);

	memset(sensor_peri->cis.cis_data->auto_exposure, 0, sizeof(sensor_peri->cis.cis_data->auto_exposure));

	return ret;
}

int get_calibrated_size(struct fimc_is_sensor_interface *itf,
			u32 *width,
			u32 *height)
{
	int ret = 0;
	struct fimc_is_module_enum *module = NULL;

	BUG_ON(!itf);
	BUG_ON(!width);
	BUG_ON(!height);

	module = get_subdev_module_enum(itf);
	BUG_ON(!module);

	*width = module->pixel_width;
	*height = module->pixel_height;

	pr_debug("%s, width(%d), height(%d)\n", __func__, *width, *height);

	return ret;
}

int get_bayer_order(struct fimc_is_sensor_interface *itf,
			u32 *bayer_order)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!bayer_order);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	*bayer_order = sensor_peri->cis.bayer_order;

	return ret;
}

u32 get_min_exposure_time(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 exposure = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MIN_EXPOSURE_TIME, &exposure);
	if (ret < 0 || exposure == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, exposure);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) min exp(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), exposure);

p_err:
	return exposure;
}

u32 get_max_exposure_time(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 exposure = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MAX_EXPOSURE_TIME, &exposure);
	if (ret < 0 || exposure == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, exposure);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) max exp(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), exposure);

p_err:
	return exposure;
}

u32 get_min_analog_gain(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 again = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MIN_ANALOG_GAIN, &again);
	if (ret < 0 || again == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, again);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) min analog gain(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), again);

p_err:
	return again;
}

u32 get_max_analog_gain(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 again = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MAX_ANALOG_GAIN, &again);
	if (ret < 0 || again == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, again);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) max analog gain(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), again);

p_err:
	return again;
}

u32 get_min_digital_gain(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 dgain = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MIN_DIGITAL_GAIN, &dgain);
	if (ret < 0 || dgain == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, dgain);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) min digital gain(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), dgain);

p_err:
	return dgain;
}

u32 get_max_digital_gain(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 dgain = 0;

	BUG_ON(!itf);

	ret = sensor_get_ctrl(itf, V4L2_CID_SENSOR_GET_MAX_DIGITAL_GAIN, &dgain);
	if (ret < 0 || dgain == 0) {
		err("err!!! ret(%d), return_value(%d)", ret, dgain);
		goto p_err;
	}

	dbg_sensor(2, "%s:(%d:%d) max digital gain(%d)\n", __func__, get_vsync_count(itf), get_frame_count(itf), dgain);

p_err:
	return dgain;
}


u32 get_vsync_count(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_csi *csi;

	BUG_ON(!itf);

	csi = get_subdev_csi(itf);
	BUG_ON(!csi);

	return atomic_read(&csi->fcount);
}

u32 get_vblank_count(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_csi *csi;

	BUG_ON(!itf);

	csi = get_subdev_csi(itf);
	BUG_ON(!csi);

	return atomic_read(&csi->vblank_count);
}

bool is_vvalid_period(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_csi *csi;

	BUG_ON(!itf);

	csi = get_subdev_csi(itf);
	BUG_ON(!csi);

	return atomic_read(&csi->vvalid) <= 0 ? false : true;
}

int request_exposure(struct fimc_is_sensor_interface *itf,
			u32 long_exposure,
			u32 short_exposure)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;

#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
#endif

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "[%s](%d:%d) long_exposure(%d), short_exposure(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf), long_exposure, short_exposure);

	end_index = (itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA);

	i = (itf->vsync_flag == false ? 0 : end_index);
	for (; i <= end_index; i++) {
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE, i, long_exposure, short_exposure);
		if (ret < 0) {
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
			goto p_err;
		}
	}
	itf->vsync_flag = true;

	/* set exposure */
	if (itf->otf_flag_3aa == true) {
		ret = set_exposure(itf, itf->cis_mode, long_exposure, short_exposure);
		if (ret < 0) {
			pr_err("[%s] set_exposure fail(%d)\n", __func__, ret);
			goto p_err;
		}
	}

#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
	/* store exposure for use initial AE */
	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	if (!sensor_peri) {
		err("[%s] sensor_peri is NULL", __func__);
		return -EINVAL;
	}

	if (sensor_peri->cis.use_initial_ae) {
		sensor_peri->cis.last_ae_setting.long_exposure = long_exposure;
		sensor_peri->cis.last_ae_setting.exposure = short_exposure;
	}
#endif

p_err:
	return ret;
}

int adjust_exposure(struct fimc_is_sensor_interface *itf,
			u32 long_exposure,
			u32 short_exposure,
			u32 *available_long_exposure,
			u32 *available_short_exposure,
			fimc_is_sensor_adjust_direction adjust_direction)
{
	/* NOT IMPLEMENTED YET */
	int ret = -1;

	dbg_sensor(1, "[%s] NOT IMPLEMENTED YET\n", __func__);

	return ret;
}

int get_next_frame_timing(struct fimc_is_sensor_interface *itf,
			u32 *long_exposure,
			u32 *short_exposure,
			u32 *frame_period,
			u64 *line_period)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!long_exposure);
	BUG_ON(!short_exposure);
	BUG_ON(!frame_period);
	BUG_ON(!line_period);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE, NEXT_FRAME, long_exposure, short_exposure);
	if (ret < 0)
		pr_err("[%s] get_interface_param EXPOSURE fail(%d)\n", __func__, ret);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	*frame_period = sensor_peri->cis.cis_data->frame_time;
	*line_period = sensor_peri->cis.cis_data->line_readOut_time;

	dbg_sensor(2, "%s:(%d:%d) exp(%d, %d), frame_period %d, line_period %lld\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_exposure, *short_exposure, *frame_period, *line_period);

	return ret;
}

int get_frame_timing(struct fimc_is_sensor_interface *itf,
			u32 *long_exposure,
			u32 *short_exposure,
			u32 *frame_period,
			u64 *line_period)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!long_exposure);
	BUG_ON(!short_exposure);
	BUG_ON(!frame_period);
	BUG_ON(!line_period);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE, CURRENT_FRAME, long_exposure, short_exposure);
	if (ret < 0)
		pr_err("[%s] get_interface_param EXPOSURE fail(%d)\n", __func__, ret);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	*frame_period = sensor_peri->cis.cis_data->frame_time;
	*line_period = sensor_peri->cis.cis_data->line_readOut_time;

	dbg_sensor(2, "%s:(%d:%d) exp(%d, %d), frame_period %d, line_period %lld\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_exposure, *short_exposure, *frame_period, *line_period);

	return ret;
}

int request_analog_gain(struct fimc_is_sensor_interface *itf,
			u32 long_analog_gain,
			u32 short_analog_gain)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "[%s](%d:%d) long_analog_gain(%d), short_analog_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf), long_analog_gain, short_analog_gain);

	end_index = (itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA);

	i = (itf->vsync_flag == false ? 0 : end_index);
	for (; i <= end_index; i++) {
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, i, long_analog_gain, short_analog_gain);
		if (ret < 0) {
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
			goto p_err;
		}
	}

p_err:
	return ret;
}

int request_gain(struct fimc_is_sensor_interface *itf,
		u32 long_total_gain,
		u32 long_analog_gain,
		u32 long_digital_gain,
		u32 short_total_gain,
		u32 short_analog_gain,
		u32 short_digital_gain)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;

#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
#endif

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "[%s](%d:%d) long_total_gain(%d), short_total_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf), long_total_gain, short_total_gain);
	dbg_sensor(1, "[%s](%d:%d) long_analog_gain(%d), short_analog_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf), long_analog_gain, short_analog_gain);
	dbg_sensor(1, "[%s](%d:%d) long_digital_gain(%d), short_digital_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf), long_digital_gain, short_digital_gain);

	end_index = (itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA);

	i = (itf->vsync_flag == false ? 0 : end_index);
	for (; i <= end_index; i++) {
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_TOTAL_GAIN, i, long_total_gain, short_total_gain);
		if (ret < 0) {
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
			goto p_err;
		}
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, i, long_analog_gain, short_analog_gain);
		if (ret < 0) {
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
			goto p_err;
		}
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN, i, long_digital_gain, short_digital_gain);
		if (ret < 0) {
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
			goto p_err;
		}
	}

	/* set gain permile */
	if (itf->otf_flag_3aa == true) {
		ret = set_gain_permile(itf, itf->cis_mode,
				long_total_gain, short_total_gain,
				long_analog_gain, short_analog_gain,
				long_digital_gain, short_digital_gain);
		if (ret < 0) {
			pr_err("[%s] set_gain_permile fail(%d)\n", __func__, ret);
			goto p_err;
		}
	}

#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
	/* store gain for use initial AE */
	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	if (!sensor_peri) {
		err("[%s] sensor_peri is NULL", __func__);
		return -EINVAL;
	}

	if (sensor_peri->cis.use_initial_ae) {
		sensor_peri->cis.last_ae_setting.long_analog_gain = long_analog_gain;
		sensor_peri->cis.last_ae_setting.long_digital_gain = long_digital_gain;
		sensor_peri->cis.last_ae_setting.analog_gain = short_analog_gain;
		sensor_peri->cis.last_ae_setting.digital_gain = short_digital_gain;
	}
#endif

p_err:
	return ret;
}

int request_sensitivity(struct fimc_is_sensor_interface *itf,
		u32 sensitivity)
{
	int ret = 0;
	u32 frame_count = 0;
	camera2_sensor_uctl_t *sensor_uctl = NULL;
	u32 i = 0;
	u32 num_of_frame = 1;
	u32 index = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_sensor_ctl *module_ctl = NULL;
	struct camera2_sensor_ctl *sensor_ctl = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	frame_count = get_frame_count(itf);

	ret = get_num_of_frame_per_one_3aa(itf, &num_of_frame);

	/* set sensitivity  */
	if (itf->otf_flag_3aa == true) {
		for (i = 0; i < num_of_frame; i++) {
			sensor_uctl = get_sensor_uctl_from_module(itf, frame_count + i);
			BUG_ON(!sensor_uctl);

			sensor_uctl->sensitivity = sensitivity;
		}
	}

	/* set previous values */
	index = (frame_count - 1) % CAM2P0_UCTL_LIST_SIZE;
	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	module_ctl = &sensor_peri->cis.sensor_ctls[index];
	sensor_ctl = &module_ctl->cur_cam20_sensor_ctrl;

	index = (frame_count + 1) % EXPECT_DM_NUM;
	if(sensor_ctl->sensitivity != 0 && module_ctl->valid_sensor_ctrl == true) {
		if(sensor_ctl->sensitivity != sensor_peri->cis.expecting_sensor_dm[index].sensitivity) {
			sensor_peri->cis.expecting_sensor_dm[index].sensitivity = sensor_ctl->sensitivity;
		}
	}

	if(sensor_ctl->exposureTime != 0 && module_ctl->valid_sensor_ctrl == true) {
		if(sensor_ctl->exposureTime != sensor_peri->cis.expecting_sensor_dm[index].exposureTime) {
			sensor_peri->cis.expecting_sensor_dm[index].exposureTime = sensor_ctl->exposureTime;
		}
	}

	if (!IS_ERR_OR_NULL(sensor_uctl))
		dbg_sensor(1, "[%s]: #=%d, sensitivity=%d, preCtrl=%d\n",
				__func__, frame_count, sensor_uctl->sensitivity, sensor_ctl->sensitivity);

	return ret;
}


int adjust_analog_gain(struct fimc_is_sensor_interface *itf,
			u32 desired_long_analog_gain,
			u32 desired_short_analog_gain,
			u32 *actual_long_gain,
			u32 *actual_short_gain,
			fimc_is_sensor_adjust_direction adjust_direction)
{
	/* NOT IMPLEMENTED YET */
	int ret = -1;

	dbg_sensor(1, "[%s] NOT IMPLEMENTED YET\n", __func__);

	return ret;
}

int get_next_analog_gain(struct fimc_is_sensor_interface *itf,
			u32 *long_analog_gain,
			u32 *short_analog_gain)
{
	int ret = 0;

	BUG_ON(!itf);
	BUG_ON(!long_analog_gain);
	BUG_ON(!short_analog_gain);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, NEXT_FRAME, long_analog_gain, short_analog_gain);
	if (ret < 0)
		pr_err("[%s] get_interface_param ANALOG_GAIN fail(%d)\n", __func__, ret);

	dbg_sensor(2, "[%s](%d:%d) long_analog_gain(%d), short_analog_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_analog_gain, *short_analog_gain);

	return ret;
}

int get_analog_gain(struct fimc_is_sensor_interface *itf,
			u32 *long_analog_gain,
			u32 *short_analog_gain)
{
	int ret = 0;

	BUG_ON(!itf);
	BUG_ON(!long_analog_gain);
	BUG_ON(!short_analog_gain);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, CURRENT_FRAME, long_analog_gain, short_analog_gain);
	if (ret < 0)
		pr_err("[%s] get_interface_param ANALOG_GAIN fail(%d)\n", __func__, ret);

	dbg_sensor(2, "[%s](%d:%d) long_analog_gain(%d), short_analog_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_analog_gain, *short_analog_gain);

	return ret;
}

int get_next_digital_gain(struct fimc_is_sensor_interface *itf,
				u32 *long_digital_gain,
				u32 *short_digital_gain)
{
	int ret = 0;

	BUG_ON(!itf);
	BUG_ON(!long_digital_gain);
	BUG_ON(!short_digital_gain);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN, NEXT_FRAME, long_digital_gain, short_digital_gain);
	if (ret < 0)
		pr_err("[%s] get_interface_param DIGITAL_GAIN fail(%d)\n", __func__, ret);

	dbg_sensor(2, "[%s](%d:%d) long_digital_gain(%d), short_digital_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_digital_gain, *short_digital_gain);

	return ret;
}

int get_digital_gain(struct fimc_is_sensor_interface *itf,
			u32 *long_digital_gain,
			u32 *short_digital_gain)
{
	int ret = 0;

	BUG_ON(!itf);
	BUG_ON(!long_digital_gain);
	BUG_ON(!short_digital_gain);

	ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN, CURRENT_FRAME, long_digital_gain, short_digital_gain);
	if (ret < 0)
		pr_err("[%s] get_interface_param DIGITAL_GAIN fail(%d)\n", __func__, ret);

	dbg_sensor(2, "[%s](%d:%d) long_digital_gain(%d), short_digital_gain(%d)\n", __func__,
		get_vsync_count(itf), get_frame_count(itf),
		*long_digital_gain, *short_digital_gain);

	return ret;
}

bool is_actuator_available(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	return test_bit(FIMC_IS_SENSOR_ACTUATOR_AVAILABLE, &sensor_peri->peri_state);
}

bool is_flash_available(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	return test_bit(FIMC_IS_SENSOR_FLASH_AVAILABLE, &sensor_peri->peri_state);
}

bool is_companion_available(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	return test_bit(FIMC_IS_SENSOR_PREPROCESSOR_AVAILABLE, &sensor_peri->peri_state);
}

bool is_ois_available(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	return test_bit(FIMC_IS_SENSOR_OIS_AVAILABLE, &sensor_peri->peri_state);
}

int get_sensor_frame_timing(struct fimc_is_sensor_interface *itf,
			u32 *pclk,
			u32 *line_length_pck,
			u32 *frame_length_lines,
			u32 *max_margin_cit)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri;

	BUG_ON(!itf);
	BUG_ON(!pclk);
	BUG_ON(!line_length_pck);
	BUG_ON(!frame_length_lines);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	*pclk = sensor_peri->cis.cis_data->pclk;
	*line_length_pck = sensor_peri->cis.cis_data->line_length_pck;
	*frame_length_lines = sensor_peri->cis.cis_data->frame_length_lines;
	*max_margin_cit = sensor_peri->cis.cis_data->cur_coarse_integration_time_step;

	dbg_sensor(2, "[%s](%d:%d) pclk(%d), line_length_pck(%d), frame_length_lines(%d), max_margin_cit(%d)\n",
		__func__, get_vsync_count(itf), get_frame_count(itf),
		*pclk, *line_length_pck, *frame_length_lines, *max_margin_cit);

	return ret;
}

#ifdef USE_MS_PDAF_INTERFACE
int get_sensor_cur_size(struct fimc_is_sensor_interface *itf,
			u32 *cur_pos_x,
			u32 *cur_pos_y,
			u32 *cur_width,
			u32 *cur_height)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!cur_width);
	BUG_ON(!cur_height);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	dbg_sensor(2, "[%s]MS_PDAF cur_x [%d] cur_y [%d]\n", __func__, sensor_peri->cis.cis_data->cur_pos_x , sensor_peri->cis.cis_data->cur_pos_y);

	*cur_pos_x = sensor_peri->cis.cis_data->cur_pos_x;
	*cur_pos_y = sensor_peri->cis.cis_data->cur_pos_y;
	*cur_width = sensor_peri->cis.cis_data->cur_width;
	*cur_height = sensor_peri->cis.cis_data->cur_height;

	return ret;
}
#else /* USE_MS_PDAF_INTERFACE */
int get_sensor_cur_size(struct fimc_is_sensor_interface *itf,
			u32 *cur_width,
			u32 *cur_height)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!cur_width);
	BUG_ON(!cur_height);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	*cur_width = sensor_peri->cis.cis_data->cur_width;
	*cur_height = sensor_peri->cis.cis_data->cur_height;

	return ret;
}
#endif /* USE_MS_PDAF_INTERFACE */

int get_sensor_max_fps(struct fimc_is_sensor_interface *itf,
			u32 *max_fps)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!max_fps);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	*max_fps = sensor_peri->cis.cis_data->max_fps;

	return ret;
}

int get_sensor_cur_fps(struct fimc_is_sensor_interface *itf,
			u32 *cur_fps)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!cur_fps);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	if (sensor_peri->cis.cis_data->cur_frame_us_time != 0) {
		*cur_fps = (u32)((1 * 1000 * 1000) / sensor_peri->cis.cis_data->cur_frame_us_time);
	} else {
		pr_err("[%s] cur_frame_us_time is ZERO\n", __func__);
		ret = -1;
	}

	return ret;
}

int get_hdr_ratio_ctl_by_again(struct fimc_is_sensor_interface *itf,
			u32 *ctrl_by_again)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!ctrl_by_again);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	*ctrl_by_again = sensor_peri->cis.hdr_ctrl_by_again;

	return ret;
}

int get_sensor_use_dgain(struct fimc_is_sensor_interface *itf,
			u32 *use_dgain)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!use_dgain);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	*use_dgain = sensor_peri->cis.use_dgain;

	return ret;
}

int set_alg_reset_flag(struct fimc_is_sensor_interface *itf,
			bool executed)
{
	int ret = 0;
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_ctl = get_sensor_ctl_from_module(itf, get_frame_count(itf));

	if (sensor_ctl == NULL) {
		err("[%s]: get_sensor_ctl_from_module fail!!\n", __func__);
		return -1;
	}

	sensor_ctl->alg_reset_flag = executed;

	return ret;
}

/*
 * TODO: need to implement getting C2, C3 stat data
 * This sensor interface returns done status of getting sensor stat
 */
int get_sensor_hdr_stat(struct fimc_is_sensor_interface *itf,
		enum itf_cis_hdr_stat_status *status)
{
	int ret = 0;

	info("%s", __func__);

	return ret;
}

/*
 * TODO: For example, 3AA thumbnail result shuld be applied to sensor in case of IMX230
 */
int set_3a_alg_res_to_sens(struct fimc_is_sensor_interface *itf,
		struct fimc_is_3a_res_to_sensor *sensor_setting)
{
	int ret = 0;

	info("%s", __func__);

	return ret;
}

int get_sensor_fnum(struct fimc_is_sensor_interface *itf,
			u32 *fnum)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!fnum);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	*fnum = sensor_peri->cis.aperture_num;

	return ret;
}

int set_initial_exposure_of_setfile(struct fimc_is_sensor_interface *itf,
				u32 expo)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	dbg_sensor(1, "[%s] init expo (%d)\n", __func__, expo);

	sensor_peri->cis.cis_data->low_expo_start = expo;

	return ret;
}

int set_video_mode_of_setfile(struct fimc_is_sensor_interface *itf,
				bool video_mode)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	dbg_sensor(1, "[%s] video mode (%d)\n", __func__, video_mode);

	sensor_peri->cis.cis_data->video_mode = video_mode;

	return ret;
}

int get_num_of_frame_per_one_3aa(struct fimc_is_sensor_interface *itf,
				u32 *num_of_frame)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	u32 max_fps = 0;

	BUG_ON(!itf);
	BUG_ON(!num_of_frame);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	max_fps = sensor_peri->cis.cis_data->max_fps;

	/* TODO: SDK should know how many frames are needed to execute one 3a. */
	*num_of_frame = NUM_OF_FRAME_30FPS;

	if (sensor_peri->cis.cis_data->video_mode == true) {
		if (max_fps >= 300) {
			*num_of_frame = 10; /* TODO */
		} else if (max_fps >= 240) {
			*num_of_frame = NUM_OF_FRAME_240FPS;
		} else if (max_fps >= 120) {
			*num_of_frame = NUM_OF_FRAME_120FPS;
		} else if (max_fps >= 60) {
			*num_of_frame = NUM_OF_FRAME_60FPS;
		}
	}

	return 0;
}

int get_offset_from_cur_result(struct fimc_is_sensor_interface *itf,
				u32 *offset)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	u32 max_fps = 0;

	BUG_ON(!itf);
	BUG_ON(!offset);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);
	BUG_ON(!sensor_peri->cis.cis_data);

	max_fps = sensor_peri->cis.cis_data->max_fps;

	/* TODO: SensorSdk delivers the result of the 3AA by taking into account this parameter. */
	*offset = 0;

	if (sensor_peri->cis.cis_data->video_mode == true) {
		if (max_fps >= 300) {
			*offset = 1;
		} else if (max_fps >= 240) {
			*offset = 1;
		} else if (max_fps >= 120) {
			*offset = 1;
		} else if (max_fps >= 60) {
			*offset = 0;
		}
	}

	return 0;
}

int set_cur_uctl_list(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (fimc_is_vender_wdr_mode_on(sensor_peri->cis.cis_data)) {
		fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			sensor_peri->cis.cur_sensor_uctrl.longExposureTime,
			sensor_peri->cis.cur_sensor_uctrl.shortExposureTime,
			sensor_peri->cis.cur_sensor_uctrl.sensitivity,
			0,
			sensor_peri->cis.cur_sensor_uctrl.longAnalogGain,
			sensor_peri->cis.cur_sensor_uctrl.shortAnalogGain,
			sensor_peri->cis.cur_sensor_uctrl.longDigitalGain,
			sensor_peri->cis.cur_sensor_uctrl.shortDigitalGain);
	} else {
		fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			sensor_peri->cis.cur_sensor_uctrl.exposureTime,
			0,
			sensor_peri->cis.cur_sensor_uctrl.sensitivity,
			0,
			sensor_peri->cis.cur_sensor_uctrl.analogGain,
			0,
			sensor_peri->cis.cur_sensor_uctrl.digitalGain,
			0);
	}

	return 0;
}

int apply_sensor_setting(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor *device = NULL;
	struct fimc_is_module_enum *module = NULL;
	struct v4l2_subdev *subdev_module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	subdev_module = module->subdev;
	if (!subdev_module) {
		err("module is not probed");
		subdev_module = NULL;
		goto p_err;
	}

	device = v4l2_get_subdev_hostdata(subdev_module);
	BUG_ON(!device);

	/* sensor control */
	fimc_is_sensor_ctl_frame_evt(device);

p_err:
	return 0;
}

int request_reset_expo_gain(struct fimc_is_sensor_interface *itf,
				u32 long_expo,
				u32 long_tgain,
				u32 long_again,
				u32 long_dgain,
				u32 short_expo,
				u32 short_tgain,
				u32 short_again,
				u32 short_dgain)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "[%s] long exposure(%d), total_gain(%d), a-gain(%d), d-gain(%d)\n", __func__,
			long_expo, long_tgain, long_again, long_dgain);
	dbg_sensor(1, "[%s] short exposure(%d), total_gain(%d), a-gain(%d), d-gain(%d)\n", __func__,
			short_expo, short_tgain, short_again, short_dgain);

	end_index = itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA;

	for (i = 0; i <= end_index; i++) {
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_TOTAL_GAIN, i, long_tgain, short_tgain);
		if (ret < 0)
			pr_err("[%s] set_interface_param TOTAL_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN, i, long_again, short_again);
		if (ret < 0)
			pr_err("[%s] set_interface_param ANALOG_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN, i, long_dgain, short_dgain);
		if (ret < 0)
			pr_err("[%s] set_interface_param DIGITAL_GAIN fail(%d)\n", __func__, ret);
		ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE, i, long_expo, short_expo);
		if (ret < 0)
			pr_err("[%s] set_interface_param EXPOSURE fail(%d)\n", __func__, ret);
	}

	itf->vsync_flag = true;

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	fimc_is_sensor_set_cis_uctrl_list(sensor_peri,
			long_expo,
			short_expo,
			long_tgain, short_tgain,
			long_again, short_again,
			long_dgain, short_dgain);

	memset(sensor_peri->cis.cis_data->auto_exposure, 0, sizeof(sensor_peri->cis.cis_data->auto_exposure));

	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].exposure = long_expo;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].analog_gain = long_again;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].digital_gain = long_dgain;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].long_exposure = long_expo;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].long_analog_gain = long_again;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].long_digital_gain = long_dgain;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].short_exposure = short_expo;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].short_analog_gain = short_again;
	sensor_peri->cis.cis_data->auto_exposure[CURRENT_FRAME].short_digital_gain = short_dgain;

	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].exposure = long_expo;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].analog_gain = long_again;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].digital_gain = long_dgain;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].long_exposure = long_expo;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].long_analog_gain = long_again;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].long_digital_gain = long_dgain;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].short_exposure = short_expo;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].short_analog_gain = short_again;
	sensor_peri->cis.cis_data->auto_exposure[NEXT_FRAME].short_digital_gain = short_dgain;

	return ret;
}

int set_sensor_info_mode_change(struct fimc_is_sensor_interface *itf,
		u32 long_expo,
		u32 long_again,
		u32 long_dgain,
		u32 expo,
		u32 again,
		u32 dgain)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);

	sensor_peri->cis.mode_chg_expo = expo;
	sensor_peri->cis.mode_chg_again = again;
	sensor_peri->cis.mode_chg_dgain = dgain;
	sensor_peri->cis.mode_chg_long_expo = long_expo;
	sensor_peri->cis.mode_chg_long_again = long_again;
	sensor_peri->cis.mode_chg_long_dgain = long_dgain;

	dbg_sensor(1, "[%s] mode_chg_expo(%d), again(%d), dgain(%d)\n", __func__,
			sensor_peri->cis.mode_chg_expo,
			sensor_peri->cis.mode_chg_again,
			sensor_peri->cis.mode_chg_dgain);

	return ret;
}

int update_sensor_dynamic_meta(struct fimc_is_sensor_interface *itf,
		u32 frame_count,
		camera2_ctl_t *ctrl,
		camera2_dm_t *dm,
		camera2_udm_t *udm)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	u32 index = 0;

	BUG_ON(!itf);
	BUG_ON(!ctrl);
	BUG_ON(!dm);
	BUG_ON(!udm);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	index = frame_count % EXPECT_DM_NUM;
	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);

	dm->sensor.exposureTime = sensor_peri->cis.expecting_sensor_dm[index].exposureTime;
	dm->sensor.frameDuration = sensor_peri->cis.cis_data->cur_frame_us_time * 1000;
	dm->sensor.sensitivity = sensor_peri->cis.expecting_sensor_dm[index].sensitivity;
	dm->sensor.rollingShutterSkew = sensor_peri->cis.cis_data->rolling_shutter_skew;

	udm->sensor.analogGain = sensor_peri->cis.expecting_sensor_udm[index].analogGain;
	udm->sensor.digitalGain = sensor_peri->cis.expecting_sensor_udm[index].digitalGain;
	udm->sensor.longExposureTime = sensor_peri->cis.expecting_sensor_udm[index].longExposureTime;
	udm->sensor.shortExposureTime = sensor_peri->cis.expecting_sensor_udm[index].shortExposureTime;
	udm->sensor.longAnalogGain = sensor_peri->cis.expecting_sensor_udm[index].longAnalogGain;
	udm->sensor.shortAnalogGain = sensor_peri->cis.expecting_sensor_udm[index].shortAnalogGain;
	udm->sensor.longDigitalGain = sensor_peri->cis.expecting_sensor_udm[index].longDigitalGain;
	udm->sensor.shortDigitalGain = sensor_peri->cis.expecting_sensor_udm[index].shortDigitalGain;

	dbg_sensor(1, "[%s]: expo(%lld), duration(%lld), sensitivity(%d), rollingShutterSkew(%lld)\n",
			__func__, dm->sensor.exposureTime,
			dm->sensor.frameDuration,
			dm->sensor.sensitivity,
			dm->sensor.rollingShutterSkew);
	dbg_sensor(1, "[%s]: udm expo[%lld, %lld], dgain[%d, %d] again[%d, %d]\n",
			__func__,
			udm->sensor.longExposureTime, udm->sensor.shortExposureTime,
			udm->sensor.longDigitalGain, udm->sensor.shortDigitalGain,
			udm->sensor.longAnalogGain, udm->sensor.shortAnalogGain);

	return ret;
}

int copy_sensor_ctl(struct fimc_is_sensor_interface *itf,
			u32 frame_count,
			camera2_shot_t *shot)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	u32 index = 0;
	struct fimc_is_sensor_ctl *sensor_ctl;
	cis_shared_data *cis_data = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	index = frame_count % EXPECT_DM_NUM;
	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	sensor_ctl = &sensor_peri->cis.sensor_ctls[index];
	cis_data = sensor_peri->cis.cis_data;

	BUG_ON(!cis_data);

	sensor_ctl->ctl_frame_number = 0;
	sensor_ctl->valid_sensor_ctrl = false;
	sensor_ctl->is_sensor_request = false;

	if (shot != NULL) {
#ifdef CONFIG_COMPANION_USE
		cis_data->companion_data.caf_mode = shot->uctl.companionUd.caf_mode;
		cis_data->companion_data.disparity_mode = shot->uctl.companionUd.disparity_mode;
#endif
		cis_data->companion_data.paf_mode = shot->uctl.companionUd.paf_mode;
		cis_data->companion_data.wdr_mode = shot->uctl.companionUd.wdr_mode;
		cis_data->companion_data.masterCam = shot->uctl.masterCam;

		sensor_ctl->ctl_frame_number = shot->dm.request.frameCount;
		sensor_ctl->cur_cam20_sensor_ctrl = shot->ctl.sensor;
		if (sensor_peri->subdev_ois) {
			sensor_peri->ois->ois_mode = shot->ctl.lens.opticalStabilizationMode;
			sensor_peri->ois->coef = (u8)shot->uctl.lensUd.oisCoefVal;
		}

#ifndef USE_WDR_INTERFACE
		if (shot->uctl.companionUd.wdr_mode == COMPANION_WDR_ON ||
				shot->uctl.companionUd.wdr_mode == COMPANION_WDR_AUTO ||
				shot->uctl.companionUd.wdr_mode == COMPANION_WDR_AUTO_LIKE)
			itf->cis_mode = ITF_CIS_SMIA_WDR;
		else
#endif
			itf->cis_mode = ITF_CIS_SMIA;

		/* set frame rate : Limit of max frame duration
		 * Frame duration is set by
		 * 1. Manual sensor control
		 *	 - For HAL3.2
		 *	 - AE_MODE is OFF and ctl.sensor.frameDuration is not 0.
		 * 2. Target FPS Range
		 *	 - For backward compatibility
		 *	 - In case of AE_MODE is not OFF and aeTargetFpsRange[0] is not 0,
		 *	   frame durtaion is 1000000us / aeTargetFpsRage[0]
		 */
		if (shot->ctl.aa.aeMode == AA_AEMODE_OFF) {
			sensor_ctl->valid_sensor_ctrl = true;
			sensor_ctl->is_sensor_request = true;
		} else if (shot->ctl.aa.aeTargetFpsRange[1] != 0) {
			u32 duration_us = 1000000 / shot->ctl.aa.aeTargetFpsRange[1];
			sensor_ctl->cur_cam20_sensor_udctrl.frameDuration = fimc_is_sensor_convert_us_to_ns(duration_us);

			/* qbuf min, max fps value */
			sensor_peri->cis.min_fps = shot->ctl.aa.aeTargetFpsRange[0];
			sensor_peri->cis.max_fps = shot->ctl.aa.aeTargetFpsRange[1];
		}
	}

	return ret;
}

int get_module_id(struct fimc_is_sensor_interface *itf, u32 *module_id)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_module_enum *module = NULL;

	BUG_ON(!itf);
	BUG_ON(!module_id);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	*module_id = module->sensor_id;

p_err:
	return 0;

}

int get_module_position(struct fimc_is_sensor_interface *itf,
				enum exynos_sensor_position *module_position)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_module_enum *module = NULL;

	BUG_ON(!itf);
	BUG_ON(!module_position);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, module in is NULL", __func__);
		module = NULL;
		goto p_err;
	}

	*module_position = module->position;

p_err:
	return 0;

}

int set_sensor_3a_mode(struct fimc_is_sensor_interface *itf,
				u32 mode)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	if (mode > 1) {
		err("ERR[%s] invalid mode(%d)\n", __func__, mode);
		return -1;
	}

	/* 0: OTF, 1: M2M */
	itf->otf_flag_3aa = mode == 0 ? true : false;

	if (itf->otf_flag_3aa == false) {
		ret = fimc_is_sensor_init_sensor_thread(sensor_peri);
		if (ret) {
			err("fimc_is_sensor_init_sensor_thread is fail(%d)", ret);
			return ret;
		}
	}

	return 0;
}

#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
int get_initial_exposure_gain_of_sensor(struct fimc_is_sensor_interface *itf,
	u32 *long_expo,
	u32 *long_again,
	u32 *long_dgain,
	u32 *short_expo,
	u32 *short_again,
	u32 *short_dgain)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_device_sensor *device = NULL;

	if (!itf) {
		err("[%s] fimc_is_sensor_interface is NULL", __func__);
		return -EINVAL;
	}

	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	if (!sensor_peri) {
		err("[%s] sensor_peri is NULL", __func__);
		return -EINVAL;
	}

	device = get_device_sensor(itf);
	if (!device) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	if (sensor_peri->cis.use_initial_ae && device->cfg->framerate < 60) {
		*long_expo = sensor_peri->cis.init_ae_setting.long_exposure;
		*long_again = sensor_peri->cis.init_ae_setting.long_analog_gain;
		*long_dgain = sensor_peri->cis.init_ae_setting.long_digital_gain;
		*short_expo = sensor_peri->cis.init_ae_setting.exposure;
		*short_again = sensor_peri->cis.init_ae_setting.analog_gain;
		*short_dgain = sensor_peri->cis.init_ae_setting.digital_gain;
	} else {
		*long_expo = 0;
		*long_again = 0;
		*long_dgain = 0;
		*short_expo = 0;
		*short_again = 0;
		*short_dgain = 0;
		dbg_sensor(1, "%s: called at not enabled last_ae, use default low exposure setting", __func__);
	}

	dbg_sensor(1, "%s: sensorid(%d),long(%d-%d-%d), shot(%d-%d-%d)\n", __func__,
		sensor_peri->module->sensor_id,
		*long_expo, *long_again, *long_dgain, *short_expo, *short_again, *short_dgain);

	return 0;
}
#endif

u32 set_adjust_sync(struct fimc_is_sensor_interface *itf, u32 setsync)
{
	u32 ret = 0;
	struct fimc_is_device_sensor *device = NULL;
	struct fimc_is_module_enum *module = NULL;
	struct v4l2_subdev *subdev_module = NULL;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	module = sensor_peri->module;
	subdev_module = module->subdev;
	device = v4l2_get_subdev_hostdata(subdev_module);

	/* sensor control */
	fimc_is_sensor_ctl_adjust_sync(device, setsync);

	return ret;
}

/* In order to change a current CIS mode when an user select the WDR (long and short exposure) mode or the normal AE mo */
int change_cis_mode(struct fimc_is_sensor_interface *itf,
		enum itf_cis_interface cis_mode)
{
	int ret = 0;

#if 0
	/* Change get cis_mode to copy sensor_ctl */
	info("cis mode : %d -> %d", itf->cis_mode, cis_mode);
	itf->cis_mode = cis_mode;
#endif

	return ret;
}

int start_of_frame(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "%s: !!!!!!!!!!!!!!!!!!!!!!!\n", __func__);
	end_index = itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA;

	for (i = 0; i < end_index; i++) {
		if (itf->cis_mode == ITF_CIS_SMIA) {
			itf->total_gain[EXPOSURE_GAIN_INDEX][i] = itf->total_gain[EXPOSURE_GAIN_INDEX][i + 1];
			itf->analog_gain[EXPOSURE_GAIN_INDEX][i] = itf->analog_gain[EXPOSURE_GAIN_INDEX][i + 1];
			itf->digital_gain[EXPOSURE_GAIN_INDEX][i] = itf->digital_gain[EXPOSURE_GAIN_INDEX][i + 1];
			itf->exposure[EXPOSURE_GAIN_INDEX][i] = itf->exposure[EXPOSURE_GAIN_INDEX][i + 1];
		} else if (itf->cis_mode == ITF_CIS_SMIA_WDR){
			itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][i] = itf->total_gain[LONG_EXPOSURE_GAIN_INDEX][i + 1];
			itf->total_gain[SHORT_EXPOSURE_GAIN_INDEX][i] = itf->total_gain[SHORT_EXPOSURE_GAIN_INDEX][i + 1];
			itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][i] = itf->analog_gain[LONG_EXPOSURE_GAIN_INDEX][i + 1];
			itf->analog_gain[SHORT_EXPOSURE_GAIN_INDEX][i] = itf->analog_gain[SHORT_EXPOSURE_GAIN_INDEX][i + 1];
			itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][i] = itf->digital_gain[LONG_EXPOSURE_GAIN_INDEX][i + 1];
			itf->digital_gain[SHORT_EXPOSURE_GAIN_INDEX][i] = itf->digital_gain[SHORT_EXPOSURE_GAIN_INDEX][i + 1];
			itf->exposure[LONG_EXPOSURE_GAIN_INDEX][i] = itf->exposure[LONG_EXPOSURE_GAIN_INDEX][i + 1];
			itf->exposure[SHORT_EXPOSURE_GAIN_INDEX][i] = itf->exposure[SHORT_EXPOSURE_GAIN_INDEX][i + 1];
		} else {
			pr_err("[%s] in valid cis_mode (%d)\n", __func__, itf->cis_mode);
			ret = -EINVAL;
			goto p_err;
		}

		itf->flash_intensity[i] = itf->flash_intensity[i + 1];
		itf->flash_mode[i] = itf->flash_mode[i + 1];
		itf->flash_firing_duration[i] = itf->flash_firing_duration[i + 1];
	}

	itf->flash_mode[i] = CAM2_FLASH_MODE_OFF;
	itf->flash_intensity[end_index] = 0;
	itf->flash_firing_duration[i] = 0;

	/* Flash setting */
	ret =  set_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_FLASH_INTENSITY, end_index, 0, 0);
	if (ret < 0)
		pr_err("[%s] set_interface_param FLASH_INTENSITY fail(%d)\n", __func__, ret);
	/* TODO */
	/*
	if (itf->flash_itf_ops) {
		(*itf->flash_itf_ops)->on_start_of_frame(itf->flash_itf_ops);
		(*itf->flash_itf_ops)->set_next_flash(itf->flash_itf_ops, itf->flash_intensity[NEXT_FRAME]);
	}
	*/

p_err:
	return ret;
}

int end_of_frame(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;
	u32 end_index = 0;
	u32 long_total_gain = 0;
	u32 short_total_gain = 0;
	u32 long_analog_gain = 0;
	u32 short_analog_gain = 0;
	u32 long_digital_gain = 0;
	u32 short_digital_gain = 0;
	u32 long_exposure = 0;
	u32 short_exposure = 0;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	dbg_sensor(1, "%s: !!!!!!!!!!!!!!!!!!!!!!!\n", __func__);
	end_index = itf->otf_flag_3aa == true ? NEXT_NEXT_FRAME_OTF : NEXT_NEXT_FRAME_DMA;

	if (itf->vsync_flag == true) {
		/* TODO: sensor timing test */

		if (itf->otf_flag_3aa == false) {
			/* set gain */
			ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_TOTAL_GAIN,
						end_index, &long_total_gain, &short_total_gain);
			if (ret < 0)
				pr_err("[%s] get TOTAL_GAIN fail(%d)\n", __func__, ret);
			ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_ANALOG_GAIN,
						end_index, &long_analog_gain, &short_analog_gain);
			if (ret < 0)
				pr_err("[%s] get ANALOG_GAIN fail(%d)\n", __func__, ret);
			ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_DIGITAL_GAIN,
						end_index, &long_digital_gain, &short_digital_gain);
			if (ret < 0)
				pr_err("[%s] get DIGITAL_GAIN fail(%d)\n", __func__, ret);

			ret = set_gain_permile(itf, itf->cis_mode,
						long_total_gain, short_total_gain,
						long_analog_gain, short_analog_gain,
						long_digital_gain, short_digital_gain);
			if (ret < 0) {
				pr_err("[%s] set_gain_permile fail(%d)\n", __func__, ret);
				goto p_err;
			}

			/* set exposure */
			ret =  get_interface_param(itf, itf->cis_mode, ITF_CIS_PARAM_EXPOSURE,
						end_index, &long_exposure, &short_exposure);
			if (ret < 0)
				pr_err("[%s] get EXPOSURE fail(%d)\n", __func__, ret);

			ret = set_exposure(itf, itf->cis_mode, long_exposure, short_exposure);
			if (ret < 0) {
				pr_err("[%s] set_exposure fail(%d)\n", __func__, ret);
				goto p_err;
			}
		}
	}

	/* TODO */
	/*
	if (itf->flash_itf_ops) {
		(*itf->flash_itf_ops)->on_end_of_frame(itf->flash_itf_ops);
	}
	*/

p_err:
	return ret;
}

int apply_frame_settings(struct fimc_is_sensor_interface *itf)
{
	/* NOT IMPLEMENTED YET */
	int ret = -1;

	err("[%s] NOT IMPLEMENTED YET\n", __func__);

	return ret;
}

/* end of new APIs */

/* Flash interface */
int set_flash(struct fimc_is_sensor_interface *itf,
		u32 frame_count, u32 flash_mode, u32 intensity, u32 time)
{
	int ret = 0;
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;
	camera2_flash_uctl_t *flash_uctl = NULL;
	enum flash_mode mode = CAM2_FLASH_MODE_OFF;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_ctl = get_sensor_ctl_from_module(itf, frame_count);
	flash_uctl = &sensor_ctl->cur_cam20_flash_udctrl;

	sensor_ctl->flash_frame_number = frame_count;

	if (intensity == 0) {
		mode = CAM2_FLASH_MODE_OFF;
	} else {
		switch (flash_mode) {
		case CAM2_FLASH_MODE_OFF:
		case CAM2_FLASH_MODE_SINGLE:
		case CAM2_FLASH_MODE_TORCH:
			mode = flash_mode;
			break;
		default:
			err("[%s] unknown scene_mode(%d)\n", __func__, flash_mode);
			break;
		}
	}

	flash_uctl->flashMode = mode;
	flash_uctl->firingPower = intensity;
	flash_uctl->firingTime = time;

	dbg_flash("[%s] frame count %d,  mode %d, intensity %d, firing time %lld\n", __func__,
			frame_count,
			flash_uctl->flashMode,
			flash_uctl->firingPower,
			flash_uctl->firingTime);

	sensor_ctl->valid_flash_udctrl = true;

	return ret;
}

int request_flash(struct fimc_is_sensor_interface *itf,
				u32 mode,
				bool on,
				u32 intensity,
				u32 time)
{
	int ret = 0;
	u32 i = 0;
	u32 end_index = 0;
	u32 vsync_cnt = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	vsync_cnt = get_vsync_count(itf);

	dbg_flash("[%s](%d) request_flash, mode(%d), on(%d), intensity(%d), time(%d)\n",
			__func__, vsync_cnt, mode, on, intensity, time);

	ret = get_num_of_frame_per_one_3aa(itf, &end_index);
	if (ret < 0) {
		pr_err("[%s] get_num_of_frame_per_one_3aa fail(%d)\n", __func__, ret);
		goto p_err;
	}

	for (i = 0; i < end_index; i++) {
		if (mode == CAM2_FLASH_MODE_TORCH && on == false) {
			dbg_flash("[%s](%d) pre-flash off, mode(%d), on(%d), intensity(%d), time(%d)\n",
				__func__, vsync_cnt, mode, on, intensity, time);
			/* pre-flash off */
			sensor_peri->flash->flash_ae.pre_fls_ae_reset = true;
			sensor_peri->flash->flash_ae.frm_num_pre_fls = vsync_cnt + 1;
		} else if (mode == CAM2_FLASH_MODE_SINGLE && on == true) {
			dbg_flash("[%s](%d) main on-off, mode(%d), on(%d), intensity(%d), time(%d)\n",
				__func__, vsync_cnt, mode, on, intensity, time);

			sensor_peri->flash->flash_data.mode = mode;
			sensor_peri->flash->flash_data.intensity = intensity;
			sensor_peri->flash->flash_data.firing_time_us = time;
			/* main-flash on off*/
			sensor_peri->flash->flash_ae.main_fls_ae_reset = true;
			sensor_peri->flash->flash_ae.frm_num_main_fls[0] = vsync_cnt + 1;
			sensor_peri->flash->flash_ae.frm_num_main_fls[1] = vsync_cnt + 2;
		} else {
			/* pre-flash on & flash off */
			ret = set_flash(itf, vsync_cnt + i, mode, intensity, time);
			if (ret < 0) {
				pr_err("[%s] set_flash fail(%d)\n", __func__, ret);
				goto p_err;
			}
		}
	}

p_err:
	return ret;
}

int request_flash_expo_gain(struct fimc_is_sensor_interface *itf,
			struct fimc_is_flash_expo_gain *flash_ae)
{
	int ret = 0;
	int i = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	for (i = 0; i < 2; i++) {
		sensor_peri->flash->flash_ae.expo[i] = flash_ae->expo[i];
		sensor_peri->flash->flash_ae.tgain[i] = flash_ae->tgain[i];
		sensor_peri->flash->flash_ae.again[i] = flash_ae->again[i];
		sensor_peri->flash->flash_ae.dgain[i] = flash_ae->dgain[i];
		sensor_peri->flash->flash_ae.long_expo[i] = flash_ae->long_expo[i];
		sensor_peri->flash->flash_ae.long_tgain[i] = flash_ae->long_tgain[i];
		sensor_peri->flash->flash_ae.long_again[i] = flash_ae->long_again[i];
		sensor_peri->flash->flash_ae.long_dgain[i] = flash_ae->long_dgain[i];
		sensor_peri->flash->flash_ae.short_expo[i] = flash_ae->short_expo[i];
		sensor_peri->flash->flash_ae.short_tgain[i] = flash_ae->short_tgain[i];
		sensor_peri->flash->flash_ae.short_again[i] = flash_ae->short_again[i];
		sensor_peri->flash->flash_ae.short_dgain[i] = flash_ae->short_dgain[i];
		dbg_flash("[%s] expo(%d, %d, %d), again(%d, %d, %d), dgain(%d, %d, %d)\n",
			__func__,
			sensor_peri->flash->flash_ae.expo[i],
			sensor_peri->flash->flash_ae.long_expo[i],
			sensor_peri->flash->flash_ae.short_expo[i],
			sensor_peri->flash->flash_ae.again[i],
			sensor_peri->flash->flash_ae.long_again[i],
			sensor_peri->flash->flash_ae.short_again[i],
			sensor_peri->flash->flash_ae.dgain[i],
			sensor_peri->flash->flash_ae.long_dgain[i],
			sensor_peri->flash->flash_ae.short_dgain[i]);
	}

	return ret;
}

int update_flash_dynamic_meta(struct fimc_is_sensor_interface *itf,
		u32 frame_count,
		camera2_ctl_t *ctrl,
		camera2_dm_t *dm,
		camera2_udm_t *udm)
{
	int ret = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;

	BUG_ON(!itf);
	BUG_ON(!ctrl);
	BUG_ON(!dm);
	BUG_ON(!udm);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);

	if (sensor_peri->flash) {
		dm->flash.flashMode = sensor_peri->flash->flash_data.mode;
		dm->flash.firingPower = sensor_peri->flash->flash_data.intensity;
		dm->flash.firingTime = sensor_peri->flash->flash_data.firing_time_us;
		if (sensor_peri->flash->flash_data.flash_fired)
			dm->flash.flashState = FLASH_STATE_FIRED;
		else
			dm->flash.flashState = FLASH_STATE_READY;

		dbg_flash("[%s]: mode(%d), power(%d), time(%lld), state(%d)\n",
				__func__, dm->flash.flashMode,
				dm->flash.firingPower,
				dm->flash.firingTime,
				dm->flash.flashState);
	}

	return ret;
}

static struct fimc_is_framemgr *get_csi_vc_framemgr(struct fimc_is_device_csi *csi, u32 ch)
{
	struct fimc_is_subdev *fimc_is_subdev_vc;
	struct fimc_is_framemgr *framemgr = NULL;

	if (ch >= CSI_VIRTUAL_CH_MAX) {
		err("VC(%d of %d) is out-of-range", ch, CSI_VIRTUAL_CH_MAX);
		return NULL;
	}

#if !defined(CONFIG_USE_SENSOR_GROUP)
	if (ch == CSI_VIRTUAL_CH_0) {
		framemgr = csi->framemgr;
	} else {
#endif
		fimc_is_subdev_vc = csi->dma_subdev[ch];
		if (!fimc_is_subdev_vc ||
				!test_bit(FIMC_IS_SUBDEV_START, &fimc_is_subdev_vc->state)) {
			err("[%d] vc(%d) subdev is not started", csi->instance, ch);
			return NULL;
		}

		framemgr = GET_SUBDEV_FRAMEMGR(fimc_is_subdev_vc);
#if !defined(CONFIG_USE_SENSOR_GROUP)
	}
#endif
	return framemgr;
}

#ifdef USE_MS_PDAF_INTERFACE
int get_vc_dma_buf(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 frame_count,
		u32 *buf_index,
		u64 *buf_addr)
{
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_framemgr *framemgr;
	struct fimc_is_frame *frame;
	struct fimc_is_subdev *subdev;
	unsigned long flags;
	int ret = -1;
	int ch;

	BUG_ON(!buf_addr);
	BUG_ON(!buf_index);

	*buf_addr = 0;
	*buf_index = 0;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);

	/* HACK: return 0 when don't update internal vc frame */
	if (csi->internal_update != true)
		return 0;

	switch (data_type) {
	case VC_BUF_DATA_TYPE_SENSOR_STAT1:
	case VC_BUF_DATA_TYPE_SENSOR_STAT2:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAILPDAF)
				break;
		}
		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT1:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_PRIVATE)
				break;
		}
		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT2:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPISTAT)
				break;
		}
		break;
	default:
		err("%s, invalid data type(%d)", __func__, data_type);
		return -EINVAL;
	}

	if (ch == CSI_VIRTUAL_CH_MAX) {
		err("requested stat. type(%d) is not supported with current config",
								data_type);
		return -EINVAL;
	}

	framemgr = get_csi_vc_framemgr(csi, ch);

	if (!framemgr) {
		err("failed to get framemgr");
		return -ENXIO;
	}

	subdev = csi->dma_subdev[ch];
	BUG_ON(!subdev);

	framemgr_e_barrier_irqs(framemgr, FMGR_IDX_30, flags);
	if (!framemgr->frames) {
		merr("framemgr was already closed", sensor);
		ret = -EINVAL;
		goto err_get_framemgr;
	}

	frame = find_frame(framemgr, FS_FREE, frame_fcount, (void *)(ulong)frame_count);
	if (frame) {
		/* cache invalidate */
		CALL_BUFOP(subdev->pb_subdev[frame->index], sync_for_cpu,
			subdev->pb_subdev[frame->index],
			0,
			subdev->pb_subdev[frame->index]->size,
			DMA_FROM_DEVICE);

		*buf_addr = frame->kvaddr_buffer[0];
		*buf_index = frame->index;

		trans_frame(framemgr, frame, FS_PROCESS);
	} else {
		err("failed to get a frame: fcount: %d", frame_count);
		ret = -EINVAL;
		goto err_invalid_frame;
	}
	csi->internal_update = false;

	framemgr_x_barrier_irqr(framemgr, FMGR_IDX_30, flags);

	dbg_sensor(2, "[%s]: ch: %d, index: %d, framecount: %d, addr: 0x%llx\n",
			__func__, ch, *buf_index, frame_count, *buf_addr);

	return 0;

err_invalid_frame:
err_get_framemgr:
	framemgr_x_barrier_irqr(framemgr, FMGR_IDX_30, flags);

	return ret;
}

int put_vc_dma_buf(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 index)
{
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_framemgr *framemgr;
	struct fimc_is_frame *frame;
	unsigned long flags;
	int ch;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);
	switch (data_type) {
	case VC_BUF_DATA_TYPE_SENSOR_STAT1:
	case VC_BUF_DATA_TYPE_SENSOR_STAT2:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAILPDAF)
				break;
		}
		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT1:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_PRIVATE)
				break;
		}
		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT2:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPISTAT)
				break;
		}
		break;
	default:
		err("%s, invalid data type(%d)", __func__, data_type);
		return -EINVAL;
	}

	if (ch == CSI_VIRTUAL_CH_MAX) {
		err("requested stat. type(%d) is not supported with current config",
								data_type);
		return -EINVAL;
	}

	framemgr = get_csi_vc_framemgr(csi, ch);

	if (!framemgr) {
		err("failed to get framemgr");
		return -ENXIO;
	}

	if (index >= framemgr->num_frames) {
		err("index(%d of %d) is out-of-range", index, framemgr->num_frames);
		return -ENOENT;
	}

	framemgr_e_barrier_irqs(framemgr, FMGR_IDX_31, flags);
	if (!framemgr->frames) {
		framemgr_x_barrier_irqr(framemgr, FMGR_IDX_31, flags);
		merr("framemgr was already closed", sensor);
		return -EINVAL;
	}

	frame = &framemgr->frames[index];
	trans_frame(framemgr, frame, FS_FREE);

	framemgr_x_barrier_irqr(framemgr, FMGR_IDX_31, flags);

	dbg_sensor(1, "[%s]: ch: %d, index: %d\n", __func__, ch, index);

	return 0;
}


int get_vc_dma_buf_info(struct fimc_is_sensor_interface *itf,
			enum itf_vc_buf_data_type data_type,
			struct vc_buf_info_t *buf_info)
{
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_sensor_vc_max_size *vc_max_size;
	struct fimc_is_module_enum *module;
	struct fimc_is_subdev *subdev;
	int ch;

	memset(buf_info, 0, sizeof(struct vc_buf_info_t));
	buf_info->stat_type = VC_STAT_TYPE_INVALID;
	buf_info->sensor_mode = VC_SENSOR_MODE_INVALID;
	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	module = sensor_peri->module;
	if (unlikely(!module)) {
		err("%s, failed to get sensor_peri's module", __func__);
		return -EINVAL;
	}

	vc_max_size = &module->vc_max_size[data_type];

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);
	switch (data_type) {
	case VC_BUF_DATA_TYPE_SENSOR_STAT1:
		if (sensor_peri->cis.use_pdaf) {
			buf_info->sensor_mode = VC_SENSOR_MODE_MSPD_GLOBAL_NORMAL;
		}
		// Intentionally have left to flow without break statement
	case VC_BUF_DATA_TYPE_SENSOR_STAT2:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAILPDAF)
				break;
		}

		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT1:
		for (ch = CSI_VIRTUAL_CH_1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_PRIVATE)
				break;
		}
		break;
	case VC_BUF_DATA_TYPE_GENERAL_STAT2:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPISTAT)
				break;
		}
		break;
	default:
		err("%s, invalid data type(%d)", __func__, data_type);
		return -EINVAL;
	}

	if (ch == CSI_VIRTUAL_CH_MAX) {
		err("requested stat. type(%d) is not supported with current config",
								data_type);
		return -EINVAL;
	}

	subdev = csi->dma_subdev[ch];
	if (!subdev) {
		err("failed to get subdev device");
		return -ENODEV;
	}

	buf_info->stat_type = vc_max_size->stat_type;
	buf_info->width = subdev->output.width;
	buf_info->height = subdev->output.height;
	buf_info->element_size = vc_max_size->element_size;

	dbg_sensor(2, "TEMP_PDAF: VC buf (req_type(%d), stat_type(%d), mode(%d), width(%d), height(%d), element(%d byte))\n",
		data_type, buf_info->stat_type, buf_info->sensor_mode, buf_info->width, buf_info->height, buf_info->element_size);

	return 0;
}

int get_vc_dma_buf_max_size(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 *width,
		u32 *height,
		u32 *element_size)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_sensor_vc_max_size *vc_max_size;
	struct fimc_is_module_enum *module;
	int buf_max_size;
	int ret = 0;

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	module = sensor_peri->module;
	if (unlikely(!module)) {
			err("%s, failed to get sensor_peri's module", __func__);
			ret = -EINVAL;
			goto p_err;
	}

	vc_max_size = &module->vc_max_size[data_type];

	*width = vc_max_size->width;
	*height = vc_max_size->height;
	*element_size = vc_max_size->element_size;

	buf_max_size = vc_max_size->width * vc_max_size->height * vc_max_size->element_size;

	info("VC max buf (type(%d), width(%d), height(%d),element(%d byte))\n",
		data_type, *width, *height, *element_size);

	return buf_max_size;
p_err:
	return ret;
}

int register_vc_dma_notifier(struct fimc_is_sensor_interface *itf,
		enum itf_vc_stat_type type,
		vc_dma_notifier_t notifier,
		void *data)
{
	return 0;
}

int unregister_vc_dma_notifier(struct fimc_is_sensor_interface *itf,
		enum itf_vc_stat_type type,
		vc_dma_notifier_t notifier)
{
	return 0;
}

int csi_reserved_0(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int csi_reserved_1(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

#else /* USE_MS_PDAF_INTERFACE */

int get_vc_dma_buf(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 *buf_index,
		u64 *buf_addr,
		u32 *frame_count)
{
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_framemgr *framemgr;
	struct fimc_is_frame *frame;
	struct fimc_is_subdev *subdev;
	struct v4l2_control ctrl;
	unsigned long flags;
	int ret = -1;
	int ch;
	u32 cur_frameptr, frameptr;

	*frame_count = 0;
	*buf_addr = 0;
	*buf_index = 0;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);

	/* HACK: return 0 when don't update internal vc frame */
	if (csi->internal_update != true)
		return 0;

	switch (data_type) {
	case VC_BUF_DATA_TYPE_PDAF:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAIL_MODE_PDAF)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("tail mode vc not exist");
			return -EINVAL;
		}
		break;
	case VC_BUF_DATA_TYPE_MIPI_STAT:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPI_STAT)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("mipi stat vc not exist");
			return -EINVAL;
		}
		break;
	default:
		err("%s, invalid data type(%d)", __func__, data_type);
		return -EINVAL;
	}

	framemgr = get_csi_vc_framemgr(csi, ch);

	if (!framemgr) {
		err("failed to get framemgr");
		return -ENXIO;
	}

	subdev = csi->dma_subdev[ch];
	BUG_ON(!subdev);

	framemgr_e_barrier_irqs(framemgr, FMGR_IDX_30, flags);
	if (!framemgr->frames) {
		framemgr_x_barrier_irqr(framemgr, FMGR_IDX_30, flags);
		merr("framemgr was already closed", sensor);
		return -EINVAL;
	}

	/* get frame current frameptr - 1 */
	ctrl.id = V4L2_CID_IS_G_VC1_FRAMEPTR + (ch - 1);
	ret = v4l2_subdev_call(sensor->subdev_csi, core, g_ctrl, &ctrl);
	if (ret) {
		err("csi_g_ctrl fail");
		framemgr_x_barrier_irqr(framemgr, FMGR_IDX_30, flags);
		return -EINVAL;
	}
	cur_frameptr = ctrl.value;
	/* set return buffer offset */
	if (subdev->vc_buffer_offset)
		frameptr = CSI_GET_PREV_FRAMEPTR(cur_frameptr, framemgr->num_frames, subdev->vc_buffer_offset);
	else
		frameptr = ctrl.value;

	frame = &framemgr->frames[frameptr];
	if (frame) {
		/* cache invalidate */
		CALL_BUFOP(subdev->pb_subdev[frame->index], sync_for_cpu,
			subdev->pb_subdev[frame->index],
			0,
			subdev->output.width * subdev->output.height * 2,
			DMA_FROM_DEVICE);
		if (frame->state == FS_FREE) {
			*frame_count = frame->fcount;
			*buf_addr = frame->kvaddr_buffer[0];
			*buf_index = frame->index;

			trans_frame(framemgr, frame, FS_PROCESS);

			csi->internal_update = false;

			ret = 0;
		}
	}

	framemgr_x_barrier_irqr(framemgr, FMGR_IDX_30, flags);

	dbg_sensor(2, "[%s]: ch: %d, index: %d, framecount: %d, addr: 0x%llx\n",
			__func__, ch, *buf_index, *frame_count, *buf_addr);

	return ret;
}

int put_vc_dma_buf(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 index)
{
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_framemgr *framemgr;
	struct fimc_is_frame *frame;
	struct fimc_is_subdev *subdev;
	unsigned long flags;
	int ret = 0;
	int ch;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -ENODEV;
	}

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);
	switch (data_type) {
	case VC_BUF_DATA_TYPE_PDAF:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAIL_MODE_PDAF)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("tail mode vc not exist");
			return -EINVAL;
		}
		break;
	case VC_BUF_DATA_TYPE_MIPI_STAT:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPI_STAT)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("tail mode vc not exist");
			return -EINVAL;
		}
		break;
	default:
		err("%s, invalid data type(%d)", __func__, data_type);
		return -EINVAL;
	}

	framemgr = get_csi_vc_framemgr(csi, ch);

	if (!framemgr) {
		err("failed to get framemgr");
		return -ENXIO;
	}

	if (index >= framemgr->num_frames) {
		err("index(%d of %d) is out-of-range", index, framemgr->num_frames);
		return -ENOENT;
	}

	subdev = csi->dma_subdev[ch];
	BUG_ON(!subdev);

	framemgr_e_barrier_irqs(framemgr, FMGR_IDX_31, flags);
	if (!framemgr->frames) {
		framemgr_x_barrier_irqr(framemgr, FMGR_IDX_31, flags);
		merr("framemgr was already closed", sensor);
		return -EINVAL;
	}

	frame = &framemgr->frames[index];
	trans_frame(framemgr, frame, FS_FREE);

	framemgr_x_barrier_irqr(framemgr, FMGR_IDX_31, flags);

	dbg_sensor(1, "[%s]: ch: %d, index: %d\n", __func__, ch, index);

	return ret;
}

int get_vc_dma_buf_size(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 *width,
		u32 *height,
		u32 *element_size)
{
	int ret = 0;
	int ch;
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_device_csi *csi;
	struct fimc_is_subdev *subdev;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("failed to get sensor device");
		ret = -ENODEV;
		goto p_err;
	}

	csi = (struct fimc_is_device_csi *)v4l2_get_subdevdata(sensor->subdev_csi);
	if (!csi) {
		err("failed to get csi device");
		ret = -ENODEV;
		goto p_err;
	}

	switch (data_type) {
	case VC_BUF_DATA_TYPE_PDAF:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_TAIL_MODE_PDAF)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("VC_BUF_DATA_TYPE_PDAF, vc not exist");
			ret = -EINVAL;
			goto p_err;
		}
		break;
	case VC_BUF_DATA_TYPE_MIPI_STAT:
		for (ch = 1; ch < CSI_VIRTUAL_CH_MAX; ch++) {
			if (csi->internal_vc[ch] == VC_MIPI_STAT)
				break;
		}

		if (ch == CSI_VIRTUAL_CH_MAX) {
			err("VC_BUF_DATA_TYPE_MIPI_STAT, vc not exist");
			ret = -EINVAL;
			goto p_err;
		}
		break;
	default:
		err("invalid data type(%d)", data_type);
		ret = -EINVAL;
		goto p_err;
	}

	subdev = csi->dma_subdev[ch];
	if (!subdev) {
		err("failed to get subdev device");
		ret = -ENODEV;
		goto p_err;
	}

	*width = subdev->output.width;
	*height = subdev->output.height;

	switch (subdev->pixelformat) {
	case V4L2_PIX_FMT_SBGGR16:
		*element_size = 2; /* byte */
		break;
	default:
		err("unknown pixelformat(%c%c%c%c)\n",
			(char)((subdev->pixelformat >> 0) & 0xFF),
			(char)((subdev->pixelformat >> 8) & 0xFF),
			(char)((subdev->pixelformat >> 16) & 0xFF),
			(char)((subdev->pixelformat >> 24) & 0xFF));
		ret = -EINVAL;
		goto p_err;
	}

	info("VC buf (type(%d), width(%d), height(%d), element(%d byte))\n",
		data_type, *width, *height, *element_size);

p_err:
	return ret;
}

int get_vc_dma_buf_max_size(struct fimc_is_sensor_interface *itf,
		enum itf_vc_buf_data_type data_type,
		u32 *width,
		u32 *height,
		u32 *element_size)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_sensor_vc_max_size *vc_max_size;
	struct fimc_is_module_enum *module;
	int buf_max_size;
	int ret = 0;

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	module = sensor_peri->module;
	if (unlikely(!module)) {
			err("%s, failed to get sensor_peri's module", __func__);
			ret = -EINVAL;
			goto p_err;
	}

	vc_max_size = &module->vc_max_size[data_type];

	*width = vc_max_size->width;
	*height = vc_max_size->height;
	*element_size = vc_max_size->element_size;

	buf_max_size = vc_max_size->width * vc_max_size->height * vc_max_size->element_size;

	info("VC max buf (type(%d), width(%d), height(%d),element(%d byte))\n",
		data_type, *width, *height, *element_size);

	return buf_max_size;
p_err:
	return ret;
}

int csi_reserved_0(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int csi_reserved_1(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int csi_reserved_2(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int csi_reserved_3(struct fimc_is_sensor_interface *itf)
{
	return 0;
}
#endif /* USE_MS_PDAF_INTERFACE */

int set_long_term_expo_mode(struct fimc_is_sensor_interface *itf,
		struct fimc_is_long_term_expo_mode *long_term_expo_mode)
{
	int ret = 0;
	int i = 0;
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);
	BUG_ON(!sensor_peri);

	sensor_peri->cis.long_term_mode.sen_strm_off_on_enable = long_term_expo_mode->sen_strm_off_on_enable;
	sensor_peri->cis.long_term_mode.frm_num_strm_off_on_interval = long_term_expo_mode->frm_num_strm_off_on_interval;

	if (sensor_peri->cis.long_term_mode.sen_strm_off_on_enable) {
		for (i = 0; i < 2; i++) {
			sensor_peri->cis.long_term_mode.expo[i] = long_term_expo_mode->expo[i];
			sensor_peri->cis.long_term_mode.tgain[i] = long_term_expo_mode->tgain[i];
			sensor_peri->cis.long_term_mode.again[i] = long_term_expo_mode->again[i];
			sensor_peri->cis.long_term_mode.dgain[i] = long_term_expo_mode->dgain[i];
			sensor_peri->cis.long_term_mode.frm_num_strm_off_on[i] = long_term_expo_mode->frm_num_strm_off_on[i];
		}

		sensor_peri->cis.long_term_mode.frame_interval = long_term_expo_mode->frm_num_strm_off_on_interval;
		sensor_peri->cis.long_term_mode.lemode_set.lemode = sensor_peri->cis.long_term_mode.sen_strm_off_on_enable;
	}

	dbg_sensor(1, "[%s]: expo[0](%d), expo[1](%d), again[0](%d), again[1](%d), "
		KERN_CONT "dgain[0](%d), again[1](%d), interval(%d)\n", __func__,
		long_term_expo_mode->expo[0], long_term_expo_mode->expo[1],
		long_term_expo_mode->again[0], long_term_expo_mode->again[1],
		long_term_expo_mode->dgain[0], long_term_expo_mode->dgain[1],
		long_term_expo_mode->frm_num_strm_off_on_interval);

	return ret;
}

int get_sensor_state(struct fimc_is_sensor_interface *itf)
{
	struct fimc_is_device_sensor *sensor;

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -1;
	}

	dbg("%s: sstream(%d)\n", sensor->instance, __func__, sensor->sstream);
	return sensor->sstream;
}

int dual_reserved_0(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int dual_reserved_1(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int dual_reserved_2(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int dual_reserved_3(struct fimc_is_sensor_interface *itf)
{
	return 0;
}

int request_wb_gain(struct fimc_is_sensor_interface *itf,
		u32 gr_gain, u32 r_gain, u32 b_gain, u32 gb_gain)
{
	struct fimc_is_device_sensor_peri *sensor_peri = NULL;
	struct fimc_is_device_sensor *sensor;
	struct fimc_is_sensor_ctl *sensor_ctl = NULL;
	int i;
	u32 frame_count = 0, num_of_frame = 1;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -1;
	}

	if (!test_bit(FIMC_IS_SENSOR_FRONT_START, &sensor->state)) {
		sensor_peri->cis.mode_chg_wb_gains.gr = gr_gain;
		sensor_peri->cis.mode_chg_wb_gains.r = r_gain;
		sensor_peri->cis.mode_chg_wb_gains.b = b_gain;
		sensor_peri->cis.mode_chg_wb_gains.gb = gb_gain;
	}

	frame_count = get_frame_count(itf);
	get_num_of_frame_per_one_3aa(itf, &num_of_frame);

	for (i = 0; i < num_of_frame; i++) {
		sensor_ctl = get_sensor_ctl_from_module(itf, frame_count + i);
		BUG_ON(!sensor_ctl);

		sensor_ctl->wb_gains.gr = gr_gain;
		sensor_ctl->wb_gains.r = r_gain;
		sensor_ctl->wb_gains.b = b_gain;
		sensor_ctl->wb_gains.gb = gb_gain;

		if (i == 0)
			sensor_ctl->update_wb_gains = true;
	}

	dbg_sensor(1, "[%s] stream %s, wb gains(gr:%d, r:%d, b:%d, gb:%d)\n",
		__func__,
		test_bit(FIMC_IS_SENSOR_FRONT_START, &sensor->state) ? "on" : "off",
		gr_gain, r_gain, b_gain, gb_gain);

	return 0;
}

int set_sensor_info_mfhdr_mode_change(struct fimc_is_sensor_interface *itf,
		u32 count, u32 *long_expo, u32 *long_again, u32 *long_dgain,
		u32 *expo, u32 *again, u32 *dgain)
{
	struct fimc_is_device_sensor_peri *sensor_peri;
	struct fimc_is_device_sensor *sensor;
	camera2_sensor_uctl_t *sensor_uctl;
	struct fimc_is_sensor_ctl *sensor_ctl;
	int idx;

	BUG_ON(!itf);
	BUG_ON(itf->magic != SENSOR_INTERFACE_MAGIC);

	sensor_peri = container_of(itf, struct fimc_is_device_sensor_peri, sensor_interface);

	sensor = get_device_sensor(itf);
	if (!sensor) {
		err("%s, failed to get sensor device", __func__);
		return -1;
	}

	if (test_bit(FIMC_IS_SENSOR_FRONT_START, &sensor->state))
		warn("[%s] called during stream on", __func__);

	if (count < 1) {
		err("[%s] wrong request count(%d)", __func__, count);
		return -1;
	}

	/* set index 0 values to mode_chg_xxx variables
	 * for applying values before stream on
	 */
	sensor_peri->cis.mode_chg_expo = expo[0];
	sensor_peri->cis.mode_chg_again = again[0];
	sensor_peri->cis.mode_chg_dgain = dgain[0];
	sensor_peri->cis.mode_chg_long_expo = long_expo[0];
	sensor_peri->cis.mode_chg_long_again = long_again[0];
	sensor_peri->cis.mode_chg_long_dgain = long_dgain[0];

	/* set index 0 ~ cnt-1 values to sensor uctl variables
	 * for applying values during streaming.
	 * (index 0 is set for code clean(not updated during streaming)
	 * set "use_sensor_work" to true for call sensor_work_thread
	 * to apply sensor settings
	 */
	sensor_peri->use_sensor_work = true;
	for (idx = 0; idx < count; idx++) {
		sensor_ctl = get_sensor_ctl_from_module(itf, get_frame_count(itf) + idx);
		sensor_ctl->force_update = true;

		sensor_uctl = get_sensor_uctl_from_module(itf, get_frame_count(itf) + idx);
		BUG_ON(!sensor_uctl);

		sensor_uctl->exposureTime = fimc_is_sensor_convert_us_to_ns(long_expo[idx]);
		sensor_uctl->longExposureTime = fimc_is_sensor_convert_us_to_ns(long_expo[idx]);
		sensor_uctl->shortExposureTime = fimc_is_sensor_convert_us_to_ns(expo[idx]);

		sensor_uctl->sensitivity = DIV_ROUND_UP((long_again[idx] + long_dgain[idx]), 10);
		sensor_uctl->analogGain = again[idx];
		sensor_uctl->digitalGain = dgain[idx];
		sensor_uctl->longAnalogGain = long_again[idx];
		sensor_uctl->shortAnalogGain = again[idx];
		sensor_uctl->longDigitalGain = long_dgain[idx];
		sensor_uctl->shortDigitalGain = dgain[idx];

		set_sensor_uctl_valid(itf, idx);

		dbg_sensor(1, "[%s][I:%d,F:%d]: exp(%d), again(%d), dgain(%d), "
			KERN_CONT "long_exp(%d), long_again(%d), long_dgain(%d)\n",
			__func__, idx, get_frame_count(itf) + idx,
			expo[idx], again[idx], dgain[idx],
			long_expo[idx], long_again[idx], long_dgain[idx]);
	}

	return 0;
}

#ifdef USE_MS_PDAF_INTERFACE
int set_paf_param(struct fimc_is_sensor_interface *itf,
		struct paf_setting_t *regs, u32 regs_size)
{
	return -EINVAL;
}

int get_paf_ready(struct fimc_is_sensor_interface *itf, u32 *ready)
{
	return -EINVAL;
}

int paf_reserved(struct fimc_is_sensor_interface *itf)
{
	return -EINVAL;
}
#endif /* USE_MS_PDAF_INTERFACE */

int init_sensor_interface(struct fimc_is_sensor_interface *itf)
{
	int ret = 0;

	itf->magic = SENSOR_INTERFACE_MAGIC;
	itf->vsync_flag = false;

	/* Default scenario is OTF */
	itf->otf_flag_3aa = true;
	/* TODO: check cis mode */
	itf->cis_mode = ITF_CIS_SMIA_WDR;
	/* OTF default is 3 frame delay */
	itf->diff_bet_sen_isp = itf->otf_flag_3aa ? DIFF_OTF_DELAY : DIFF_M2M_DELAY;

	/* struct fimc_is_cis_interface_ops */
	itf->cis_itf_ops.request_reset_interface = request_reset_interface;
	itf->cis_itf_ops.get_calibrated_size = get_calibrated_size;
	itf->cis_itf_ops.get_bayer_order = get_bayer_order;
	itf->cis_itf_ops.get_min_exposure_time = get_min_exposure_time;
	itf->cis_itf_ops.get_max_exposure_time = get_max_exposure_time;
	itf->cis_itf_ops.get_min_analog_gain = get_min_analog_gain;
	itf->cis_itf_ops.get_max_analog_gain = get_max_analog_gain;
	itf->cis_itf_ops.get_min_digital_gain = get_min_digital_gain;
	itf->cis_itf_ops.get_max_digital_gain = get_max_digital_gain;

	itf->cis_itf_ops.get_vsync_count = get_vsync_count;
	itf->cis_itf_ops.get_vblank_count = get_vblank_count;
	itf->cis_itf_ops.is_vvalid_period = is_vvalid_period;

	itf->cis_itf_ops.request_exposure = request_exposure;
	itf->cis_itf_ops.adjust_exposure = adjust_exposure;

	itf->cis_itf_ops.get_next_frame_timing = get_next_frame_timing;
	itf->cis_itf_ops.get_frame_timing = get_frame_timing;

	itf->cis_itf_ops.request_analog_gain = request_analog_gain;
	itf->cis_itf_ops.request_gain = request_gain;

	itf->cis_itf_ops.adjust_analog_gain = adjust_analog_gain;
	itf->cis_itf_ops.get_next_analog_gain = get_next_analog_gain;
	itf->cis_itf_ops.get_analog_gain = get_analog_gain;

	itf->cis_itf_ops.get_next_digital_gain = get_next_digital_gain;
	itf->cis_itf_ops.get_digital_gain = get_digital_gain;

	itf->cis_itf_ops.is_actuator_available = is_actuator_available;
	itf->cis_itf_ops.is_flash_available = is_flash_available;
	itf->cis_itf_ops.is_companion_available = is_companion_available;
	itf->cis_itf_ops.is_ois_available = is_ois_available;

	itf->cis_itf_ops.get_sensor_frame_timing = get_sensor_frame_timing;
	itf->cis_itf_ops.get_sensor_cur_size = get_sensor_cur_size;
	itf->cis_itf_ops.get_sensor_max_fps = get_sensor_max_fps;
	itf->cis_itf_ops.get_sensor_cur_fps = get_sensor_cur_fps;
	itf->cis_itf_ops.get_hdr_ratio_ctl_by_again = get_hdr_ratio_ctl_by_again;
	itf->cis_itf_ops.get_sensor_use_dgain = get_sensor_use_dgain;
	itf->cis_itf_ops.get_sensor_fnum = get_sensor_fnum;
	itf->cis_itf_ops.set_alg_reset_flag = set_alg_reset_flag;
	itf->cis_ext_itf_ops.get_sensor_hdr_stat = get_sensor_hdr_stat;
	itf->cis_ext_itf_ops.set_3a_alg_res_to_sens = set_3a_alg_res_to_sens;

	itf->cis_itf_ops.set_initial_exposure_of_setfile = set_initial_exposure_of_setfile;
	itf->cis_itf_ops.set_video_mode_of_setfile = set_video_mode_of_setfile;

	itf->cis_itf_ops.get_num_of_frame_per_one_3aa = get_num_of_frame_per_one_3aa;
	itf->cis_itf_ops.get_offset_from_cur_result = get_offset_from_cur_result;

	itf->cis_itf_ops.set_cur_uctl_list = set_cur_uctl_list;

	/* TODO: What is diff with apply_frame_settings at event_ops */
	itf->cis_itf_ops.apply_sensor_setting = apply_sensor_setting;

	/* reset exposure and gain for flash */
	itf->cis_itf_ops.request_reset_expo_gain = request_reset_expo_gain;

	itf->cis_itf_ops.set_sensor_info_mode_change = set_sensor_info_mode_change;
	itf->cis_itf_ops.update_sensor_dynamic_meta = update_sensor_dynamic_meta;
	itf->cis_itf_ops.copy_sensor_ctl = copy_sensor_ctl;
	itf->cis_itf_ops.get_module_id = get_module_id;
	itf->cis_itf_ops.get_module_position = get_module_position;
	itf->cis_itf_ops.set_sensor_3a_mode = set_sensor_3a_mode;
#ifdef USE_FACE_UNLOCK_AE_AWB_INIT
	itf->cis_itf_ops.get_initial_exposure_gain_of_sensor = get_initial_exposure_gain_of_sensor;
#endif
	itf->cis_ext_itf_ops.change_cis_mode = change_cis_mode;

	/* struct fimc_is_cis_event_ops */
	itf->cis_evt_ops.start_of_frame = start_of_frame;
	itf->cis_evt_ops.end_of_frame = end_of_frame;
	itf->cis_evt_ops.apply_frame_settings = apply_frame_settings;

	/* Actuator interface */
	itf->actuator_itf.soft_landing_table.enable = false;
	itf->actuator_itf.position_table.enable = false;
	itf->actuator_itf.initialized = false;

	itf->actuator_itf_ops.set_actuator_position_table = set_actuator_position_table;
	itf->actuator_itf_ops.set_soft_landing_config = set_soft_landing_config;
	itf->actuator_itf_ops.set_position = set_position;
	itf->actuator_itf_ops.get_cur_frame_position = get_cur_frame_position;
	itf->actuator_itf_ops.get_applied_actual_position = get_applied_actual_position;
	itf->actuator_itf_ops.get_prev_frame_position = get_prev_frame_position;
	itf->actuator_itf_ops.set_af_window_position = set_af_window_position; /* AF window value for M2M AF */
	itf->actuator_itf_ops.get_status = get_status;

	/* Flash interface */
	itf->flash_itf_ops.request_flash = request_flash;
	itf->flash_itf_ops.request_flash_expo_gain = request_flash_expo_gain;
	itf->flash_itf_ops.update_flash_dynamic_meta = update_flash_dynamic_meta;

#ifdef USE_MS_PDAF_INTERFACE
	/* paf interface */
	itf->paf_itf_ops.set_paf_param = set_paf_param;
	itf->paf_itf_ops.get_paf_ready = get_paf_ready;
	itf->paf_itf_ops.reserved[0] = paf_reserved;
	itf->paf_itf_ops.reserved[1] = paf_reserved;
	itf->paf_itf_ops.reserved[2] = paf_reserved;
	itf->paf_itf_ops.reserved[3] = paf_reserved;
	itf->paf_itf_ops.reserved[4] = paf_reserved;
	itf->paf_itf_ops.reserved[5] = paf_reserved;

	/* MIPI-CSI interface */
	itf->csi_itf_ops.get_vc_dma_buf = get_vc_dma_buf;
	itf->csi_itf_ops.put_vc_dma_buf = put_vc_dma_buf;
	itf->csi_itf_ops.get_vc_dma_buf_info = get_vc_dma_buf_info;
	itf->csi_itf_ops.get_vc_dma_buf_max_size = get_vc_dma_buf_max_size;
	itf->csi_itf_ops.register_vc_dma_notifier = register_vc_dma_notifier;
	itf->csi_itf_ops.unregister_vc_dma_notifier = unregister_vc_dma_notifier;
	itf->csi_itf_ops.reserved[0] = csi_reserved_0;
	itf->csi_itf_ops.reserved[1] = csi_reserved_1;
#else /* USE_MS_PDAF_INTERFACE */
	/* MIPI-CSI interface */
	itf->csi_itf_ops.get_vc_dma_buf = get_vc_dma_buf;
	itf->csi_itf_ops.put_vc_dma_buf = put_vc_dma_buf;
	itf->csi_itf_ops.get_vc_dma_buf_size = get_vc_dma_buf_size;
	itf->csi_itf_ops.get_vc_dma_buf_max_size = get_vc_dma_buf_max_size;
	itf->csi_itf_ops.reserved[0] = csi_reserved_0;
	itf->csi_itf_ops.reserved[1] = csi_reserved_1;
	itf->csi_itf_ops.reserved[2] = csi_reserved_2;
	itf->csi_itf_ops.reserved[3] = csi_reserved_3;
#endif /* USE_MS_PDAF_INTERFACE */

	/* CIS ext2 interface */
	/* Long Term Exposure mode(LTE mode) interface */
	itf->cis_ext2_itf_ops.set_long_term_expo_mode = set_long_term_expo_mode;
	itf->cis_ext_itf_ops.set_adjust_sync = set_adjust_sync;
	itf->cis_ext_itf_ops.request_sensitivity = request_sensitivity;

	/* Sensor dual sceanrio interface */
	itf->dual_itf_ops.get_sensor_state = get_sensor_state;
	itf->dual_itf_ops.reserved[0] = dual_reserved_0;
	itf->dual_itf_ops.reserved[1] = dual_reserved_1;
	itf->dual_itf_ops.reserved[2] = dual_reserved_2;
	itf->dual_itf_ops.reserved[3] = dual_reserved_3;

	itf->cis_ext2_itf_ops.request_wb_gain = request_wb_gain;
	itf->cis_ext2_itf_ops.set_sensor_info_mfhdr_mode_change = set_sensor_info_mfhdr_mode_change;

	return ret;
}

