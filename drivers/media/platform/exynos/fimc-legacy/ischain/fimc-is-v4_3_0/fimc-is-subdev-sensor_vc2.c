/*
 * Samsung Exynos5 SoC series FIMC-IS driver
 *
 * exynos5 fimc-is video functions
 *
 * Copyright (c) 2011 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "fimc-is-device-ischain.h"
#include "fimc-is-device-sensor.h"
#include "fimc-is-subdev-ctrl.h"
#include "fimc-is-config.h"
#include "fimc-is-param.h"
#include "fimc-is-video.h"
#include "fimc-is-type.h"

static int fimc_is_sensor_vc2_cfg(struct fimc_is_subdev *leader,
	void *device_data,
	struct fimc_is_frame *frame,
	struct fimc_is_crop *incrop,
	struct fimc_is_crop *otcrop,
	u32 *lindex,
	u32 *hindex,
	u32 *indexes)
{
	int ret =0;

	return ret;
}

static int fimc_is_sensor_vc2_tag(struct fimc_is_subdev *subdev,
	void *device_data,
	struct fimc_is_frame *ldr_frame,
	struct camera2_node *node)
{
	int ret = 0;
	struct v4l2_subdev *subdev_csi;
	struct fimc_is_device_sensor *device;

	BUG_ON(!subdev);
	BUG_ON(!ldr_frame);

	device = (struct fimc_is_device_sensor *)device_data;
	subdev_csi = device->subdev_csi;

	if (!test_bit(FIMC_IS_SUBDEV_OPEN, &subdev->state)) {
		merr("[SUB%d] is not opened", device, subdev->vid);
		ret = -EINVAL;
		goto p_err;
	}

#ifdef DBG_STREAMING
	mdbgd_sensor("VC2 TAG(request %d)\n", device, node->request);
#endif
	if (node->request) {
		ret = fimc_is_sensor_buf_tag(device,
			subdev,
			subdev_csi,
			ldr_frame);
		if (ret) {
			mswarn("%d frame is drop", device, subdev, ldr_frame->fcount);
			node->request = 0;
		}
	}

p_err:
	return ret;
}


const struct fimc_is_subdev_ops fimc_is_subdev_ssvc2_ops = {
	.bypass			= NULL,
	.cfg			= fimc_is_sensor_vc2_cfg,
	.tag			= fimc_is_sensor_vc2_tag,
};
