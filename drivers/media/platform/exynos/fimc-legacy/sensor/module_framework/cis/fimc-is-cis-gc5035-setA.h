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

#ifndef FIMC_IS_CIS_GC5035_SET_A_H
#define FIMC_IS_CIS_GC5035_SET_A_H

#include "fimc-is-cis.h"
#include "fimc-is-cis-gc5035.h"

// Reference Version : GC5035 Setting Beta V0.40.xlsx

const u32 sensor_gc5035_setfile_A_Global[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xc1, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x44, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x0c, 0x01,
	0x11, 0x02, 0x01,
	0x17, 0x80, 0x01,
	0x19, 0x05, 0x01,
	0xfe, 0x02, 0x01,
	0x30, 0x03, 0x01,
	0x31, 0x03, 0x01,
	0xfe, 0x00, 0x01,
	0xd9, 0xc0, 0x01,
	0x1b, 0x20, 0x01,
	0x21, 0x48, 0x01,
	0x28, 0x22, 0x01,
	0x29, 0x58, 0x01,
	0x4b, 0x10, 0x01,
	0x4e, 0x1a, 0x01,
	0x50, 0x11, 0x01,
	0x52, 0x33, 0x01,
	0x53, 0x44, 0x01,
	0x55, 0x10, 0x01,
	0x5b, 0x11, 0x01,
	0xc5, 0x02, 0x01,
	0x8c, 0x1a, 0x01,
	0xfe, 0x02, 0x01,
	0x33, 0x05, 0x01,
	0x32, 0x38, 0x01,
	0xfe, 0x00, 0x01,
	0x16, 0x0c, 0x01,
	0x1a, 0x1a, 0x01,
	0x20, 0x10, 0x01,
	0x46, 0xe3, 0x01,
	0x4a, 0x04, 0x01,
	0x54, 0x02, 0x01,
	0x62, 0x00, 0x01,
	0x72, 0xcf, 0x01,
	0x73, 0xc9, 0x01,
	0x7a, 0x05, 0x01,
	0x7d, 0xcc, 0x01,
	0x90, 0x00, 0x01,
	0xce, 0x98, 0x01,
	0xd2, 0x40, 0x01,
	0xe6, 0xe0, 0x01,
	0xfe, 0x02, 0x01,
	0x12, 0x01, 0x01,
	0x13, 0x01, 0x01,
	0x14, 0x01, 0x01,
	0x15, 0x02, 0x01,
	0x22, 0x7c, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xb0, 0x6e, 0x01,
	0xb1, 0x01, 0x01,
	0xb2, 0x00, 0x01,
	0xb3, 0x00, 0x01,
	0xb4, 0x00, 0x01,
	0xb6, 0x00, 0x01,
	0xfe, 0x01, 0x01,
	0x53, 0x00, 0x01,
	0x89, 0x03, 0x01,
	0x60, 0x40, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x00, 0x01,
};

const u32 sensor_gc5035_setfile_A_2576x1932_30fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x07, 0x01,
	0x42, 0xf4, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x00, 0x01,
	0x92, 0x0e, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0x10, 0x01,
	0x95, 0x07, 0x01,
	0x96, 0x8c, 0x01,
	0x97, 0x0a, 0x01,
	0x98, 0x10, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_2560x1440_30fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x07, 0x01,
	0x42, 0xf4, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x01, 0x01,
	0x92, 0x04, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0x18, 0x01,
	0x95, 0x05, 0x01,
	0x96, 0xa0, 0x01,
	0x97, 0x0a, 0x01,
	0x98, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x2, 0x57, 0x01,
	0x3, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_2224x1080_30fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x07, 0x01,
	0x42, 0xf4, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x01, 0x01,
	0x92, 0xb8, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0xc0, 0x01,
	0x95, 0x04, 0x01,
	0x96, 0x38, 0x01,
	0x97, 0x08, 0x01,
	0x98, 0xb0, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_1920x1920_30fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x07, 0x01,
	0x42, 0xf4, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x00, 0x01,
	0x92, 0x14, 0x01,
	0x93, 0x01, 0x01,
	0x94, 0x58, 0x01,
	0x95, 0x07, 0x01,
	0x96, 0x80, 0x01,
	0x97, 0x7, 0x01,
	0x98, 0x80, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};


const u32 sensor_gc5035_setfile_A_2576x1932_24fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x09, 0x01,
	0x42, 0xe8, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x00, 0x01,
	0x92, 0x0e, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0x10, 0x01,
	0x95, 0x07, 0x01,
	0x96, 0x8c, 0x01,
	0x97, 0x0a, 0x01,
	0x98, 0x10, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_2560x1440_24fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x09, 0x01,
	0x42, 0xe8, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x01, 0x01,
	0x92, 0x04, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0x18, 0x01,
	0x95, 0x05, 0x01,
	0x96, 0xa0, 0x01,
	0x97, 0x0a, 0x01,
	0x98, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};


const u32 sensor_gc5035_setfile_A_2224x1080_24fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x09, 0x01,
	0x42, 0xe8, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x01, 0x01,
	0x92, 0xb8, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0xc0, 0x01,
	0x95, 0x04, 0x01,
	0x96, 0x38, 0x01,
	0x97, 0x08, 0x01,
	0x98, 0xb0, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_1920x1920_24fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x09, 0x01,
	0x42, 0xe8, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x00, 0x01,
	0x92, 0x14, 0x01,
	0x93, 0x01, 0x01,
	0x94, 0x58, 0x01,
	0x95, 0x07, 0x01,
	0x96, 0x80, 0x01,
	0x97, 0x7, 0x01,
	0x98, 0x80, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_640x480_120fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x20, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0x87, 0x01,
	0xf7, 0x11, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x18, 0x01,
	0x09, 0x01, 0x01,
	0x0a, 0xf4, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x03, 0x01,
	0x0e, 0xc8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x15, 0x01,
	0x92, 0x3a, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0x45, 0x01,
	0x96, 0x35, 0x01,
	0xd5, 0xf0, 0x01,
	0x97, 0x20, 0x01,
	0x1f, 0x19, 0x01,
	0xd0, 0xb3, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x03, 0x01,
	0x42, 0xf8, 0x01,
	0x03, 0x02, 0x01,
	0x04, 0x64, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x00, 0x01,
	0x4a, 0x01, 0x01,
	0x4b, 0xf8, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x06, 0x01,
	0x44, 0x02, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x00, 0x01,
	0x92, 0x02, 0x01,
	0x93, 0x01, 0x01,
	0x94, 0x4c, 0x01,
	0x95, 0x01, 0x01,
	0x96, 0xe0, 0x01,
	0x97, 0x02, 0x01,
	0x98, 0x80, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x58, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x03, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x06, 0x01,
	0x29, 0x03, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x06, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};
const u32 sensor_gc5035_setfile_A_2576x1188_30fps[] = {
	0xfc, 0x01, 0x01,
	0xf4, 0x40, 0x01,
	0xf5, 0xe9, 0x01,
	0xf6, 0x14, 0x01,
	0xf8, 0x45, 0x01,
	0xf9, 0x82, 0x01,
	0xfa, 0x00, 0x01,
	0xfc, 0x81, 0x01,
	0xfe, 0x00, 0x01,
	0x36, 0x01, 0x01,
	0xd3, 0x87, 0x01,
	0x36, 0x00, 0x01,
	0x33, 0x00, 0x01,
	0xfe, 0x03, 0x01,
	0x01, 0xe7, 0x01,
	0xf7, 0x01, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8f, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xee, 0x30, 0x01,
	0x87, 0x18, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x90, 0x01,
	0xfe, 0x00, 0x01,
	0x05, 0x02, 0x01,
	0x06, 0xde, 0x01,
	0x9d, 0x34, 0x01,
	0x09, 0x00, 0x01,
	0x0a, 0x04, 0x01,
	0x0b, 0x00, 0x01,
	0x0c, 0x03, 0x01,
	0x0d, 0x07, 0x01,
	0x0e, 0xa8, 0x01,
	0x0f, 0x0a, 0x01,
	0x10, 0x30, 0x01,
	0x91, 0x80, 0x01,
	0x92, 0x28, 0x01,
	0x93, 0x20, 0x01,
	0x95, 0xa0, 0x01,
	0x96, 0xe0, 0x01,
	0xd5, 0xfc, 0x01,
	0x97, 0x28, 0x01,
	0x1f, 0x11, 0x01,
	0xd0, 0xb2, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x88, 0x01,
	0xfe, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0xfc, 0x8e, 0x01,
	0xfe, 0x00, 0x01,
	0x41, 0x07, 0x01,
	0x42, 0xf4, 0x01,
	0x03, 0x07, 0x01,
	0x04, 0x2c, 0x01,
	0xfe, 0x01, 0x01,
	0x42, 0x21, 0x01,
	0x49, 0x03, 0x01,
	0x4a, 0xff, 0x01,
	0x4b, 0xc0, 0x01,
	0x55, 0x00, 0x01,
	0x41, 0x28, 0x01,
	0x4c, 0x00, 0x01,
	0x4d, 0x00, 0x01,
	0x4e, 0x3c, 0x01,
	0x44, 0x08, 0x01,
	0x48, 0x02, 0x01,
	0x91, 0x01, 0x01,
	0x92, 0x82, 0x01,
	0x93, 0x00, 0x01,
	0x94, 0x10, 0x01,
	0x95, 0x04, 0x01,
	0x96, 0xa4, 0x01,
	0x97, 0x0a, 0x01,
	0x98, 0x10, 0x01,
	0xfe, 0x03, 0x01,
	0x02, 0x57, 0x01,
	0x03, 0xb7, 0x01,
	0x15, 0x14, 0x01,
	0x18, 0x0f, 0x01,
	0x21, 0x22, 0x01,
	0x22, 0x07, 0x01,
	0x23, 0x48, 0x01,
	0x24, 0x12, 0x01,
	0x25, 0x28, 0x01,
	0x26, 0x09, 0x01,
	0x29, 0x06, 0x01,
	0x2a, 0x58, 0x01,
	0x2b, 0x09, 0x01,
	0xfe, 0x01, 0x01,
	0x8c, 0x10, 0x01,
	0xfe, 0x00, 0x01,
	0x3e, 0x91, 0x01,
};

const u32 sensor_gc5035_setfile_A_Fsync_Slave[] = {
	0x91, 0x00, 0x01,
	I2C_MODE_DELAY, 50000, 0x00, /* Delay change to 50000us */
	0x00, 0x09, 0x01,
	0x82, 0x0a, 0x01,
	0x84, 0x80, 0x01,
};

const u32 sensor_gc5035_setfile_A_Fsync_Master[] = {
	0x91, 0x00, 0x01,
	I2C_MODE_DELAY, 50000, 0x00, /* Delay change to 50000us */
	0x00, 0x09, 0x01,
	0x82, 0x01, 0x01,
	0x84, 0x80, 0x01,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2576x1932_30fps = {
	EXT_CLK_Mhz * 1000 * 1000,  /* ext_clk */
	884 * 1000 * 1000,          /* mipi_datarate = OPSYCK */
	176800000,                  /* pclk = VTPXCK*/
	1996,                       /* frame_length_lines */
	2936,                       /* line_length_pck */
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2560x1440_30fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	1996,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2224x1080_30fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	1996,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_1920x1920_30fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	1996,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2576x1932_24fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	2492,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2560x1440_24fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	2492,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2224x1080_24fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	2492,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_1920x1920_24fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	2492,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_640x480_120fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	442 * 1000 * 1000, 
	176800000,
	502,
	2936,
};

const struct sensor_pll_info_compact sensor_gc5035_pllinfo_A_2576x1188_30fps = {
	EXT_CLK_Mhz * 1000 * 1000,   /* ext_clk */
	884 * 1000 * 1000, 
	176800000,
	2492,
	2936,
};

static const u32 *sensor_gc5035_setfiles_A[] = {
	sensor_gc5035_setfile_A_2576x1932_30fps,
	sensor_gc5035_setfile_A_2560x1440_30fps,
	sensor_gc5035_setfile_A_2224x1080_30fps,
	sensor_gc5035_setfile_A_1920x1920_30fps,
	sensor_gc5035_setfile_A_2576x1188_30fps,
	sensor_gc5035_setfile_A_2576x1932_24fps,
	sensor_gc5035_setfile_A_2560x1440_24fps,
	sensor_gc5035_setfile_A_2224x1080_24fps,
	sensor_gc5035_setfile_A_1920x1920_24fps,
	sensor_gc5035_setfile_A_640x480_120fps,
};

static const u32 sensor_gc5035_setfile_A_sizes[] = {
	sizeof(sensor_gc5035_setfile_A_2576x1932_30fps) / sizeof(sensor_gc5035_setfile_A_2576x1932_30fps[0]),
	sizeof(sensor_gc5035_setfile_A_2560x1440_30fps) / sizeof(sensor_gc5035_setfile_A_2560x1440_30fps[0]),
	sizeof(sensor_gc5035_setfile_A_2224x1080_30fps) / sizeof(sensor_gc5035_setfile_A_2224x1080_30fps[0]),
	sizeof(sensor_gc5035_setfile_A_1920x1920_30fps) / sizeof(sensor_gc5035_setfile_A_1920x1920_30fps[0]),
	sizeof(sensor_gc5035_setfile_A_2576x1188_30fps) / sizeof(sensor_gc5035_setfile_A_2576x1188_30fps[0]),
	sizeof(sensor_gc5035_setfile_A_2576x1932_24fps) / sizeof(sensor_gc5035_setfile_A_2576x1932_24fps[0]),
	sizeof(sensor_gc5035_setfile_A_2560x1440_24fps) / sizeof(sensor_gc5035_setfile_A_2560x1440_24fps[0]),
	sizeof(sensor_gc5035_setfile_A_2224x1080_24fps) / sizeof(sensor_gc5035_setfile_A_2224x1080_24fps[0]),
	sizeof(sensor_gc5035_setfile_A_1920x1920_24fps) / sizeof(sensor_gc5035_setfile_A_1920x1920_24fps[0]),
	sizeof(sensor_gc5035_setfile_A_640x480_120fps) / sizeof(sensor_gc5035_setfile_A_640x480_120fps[0]),
};

static const struct sensor_pll_info_compact *sensor_gc5035_pllinfos_A[] = {
	&sensor_gc5035_pllinfo_A_2576x1932_30fps,
	&sensor_gc5035_pllinfo_A_2560x1440_30fps,
	&sensor_gc5035_pllinfo_A_2224x1080_30fps,
	&sensor_gc5035_pllinfo_A_1920x1920_30fps,
	&sensor_gc5035_pllinfo_A_2576x1188_30fps,
	&sensor_gc5035_pllinfo_A_2576x1932_24fps,
	&sensor_gc5035_pllinfo_A_2560x1440_24fps,
	&sensor_gc5035_pllinfo_A_2224x1080_24fps,
	&sensor_gc5035_pllinfo_A_1920x1920_24fps,
	&sensor_gc5035_pllinfo_A_640x480_120fps,
};

#endif
