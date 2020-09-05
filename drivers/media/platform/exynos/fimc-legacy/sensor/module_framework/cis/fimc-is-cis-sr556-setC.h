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

#ifndef FIMC_IS_CIS_SR556_SET_C_H
#define FIMC_IS_CIS_SR556_SET_C_H

#include "fimc-is-cis.h"
#include "fimc-is-cis-sr556.h"

/* Reference : SR-556_Setting_V17.2_20180903.xlsx */

/* READ ME :
* There's a performance degradation issue due to camera radiation noise.
* With MIPI clock 429MHz setting radiation is more in Tablet(Landscape Device) radiation test. 
* So changing the MIPI clock to 419.25 as per HW team request.
* This file should be used only for Tablet's(Landscape Device).
*/

const u32 sensor_sr556_setfile_C_Global[] = {
	0x0e00, 0x0102, 0x02,
	0x0e02, 0x0102, 0x02,
	0x0e0c, 0x0100, 0x02,
	0x2000, 0x4031, 0x02,
	0x2002, 0x8400, 0x02,
	0x2004, 0x12b0, 0x02,
	0x2006, 0xe104, 0x02,
	0x2008, 0x12b0, 0x02,
	0x200a, 0xe12c, 0x02,
	0x200c, 0x12b0, 0x02,
	0x200e, 0xe142, 0x02,
	0x2010, 0x12b0, 0x02,
	0x2012, 0xe254, 0x02,
	0x2014, 0x12b0, 0x02,
	0x2016, 0xe150, 0x02,
	0x2018, 0x12b0, 0x02,
	0x201a, 0xed9e, 0x02,
	0x201c, 0x12b0, 0x02,
	0x201e, 0xe16c, 0x02,
	0x2020, 0x12b0, 0x02,
	0x2022, 0xe67e, 0x02,
	0x2024, 0x12b0, 0x02,
	0x2026, 0xe182, 0x02,
	0x2028, 0x12b0, 0x02,
	0x202a, 0xe198, 0x02,
	0x202c, 0x12b0, 0x02,
	0x202e, 0xe1ba, 0x02,
	0x2030, 0x12b0, 0x02,
	0x2032, 0xf422, 0x02,
	0x2034, 0x12b0, 0x02,
	0x2036, 0xe1c4, 0x02,
	0x2038, 0x12b0, 0x02,
	0x203a, 0xf3c2, 0x02,
	0x203c, 0x9392, 0x02,
	0x203e, 0x7114, 0x02,
	0x2040, 0x2003, 0x02,
	0x2042, 0x12b0, 0x02,
	0x2044, 0xe1fa, 0x02,
	0x2046, 0x3ffa, 0x02,
	0x2048, 0x0b00, 0x02,
	0x204a, 0x7302, 0x02,
	0x204c, 0x0036, 0x02,
	0x204e, 0x4392, 0x02,
	0x2050, 0x7902, 0x02,
	0x2052, 0x4292, 0x02,
	0x2054, 0x7100, 0x02,
	0x2056, 0x82be, 0x02,
	0x2058, 0x9382, 0x02,
	0x205a, 0x7114, 0x02,
	0x205c, 0x2403, 0x02,
	0x205e, 0x40b2, 0x02,
	0x2060, 0xd081, 0x02,
	0x2062, 0x0b88, 0x02,
	0x2064, 0x12b0, 0x02,
	0x2066, 0xe6d8, 0x02,
	0x2068, 0x12b0, 0x02,
	0x206a, 0xea1c, 0x02,
	0x206c, 0x12b0, 0x02,
	0x206e, 0xe1e4, 0x02,
	0x2070, 0x12b0, 0x02,
	0x2072, 0xe370, 0x02,
	0x2074, 0x930f, 0x02,
	0x2076, 0x27e2, 0x02,
	0x2078, 0x12b0, 0x02,
	0x207a, 0xf3da, 0x02,
	0x207c, 0x3fd5, 0x02,
	0x207e, 0x4030, 0x02,
	0x2080, 0xf750, 0x02,
	0x27fe, 0xe000, 0x02,
	0x3000, 0x70f8, 0x02,
	0x3002, 0x187f, 0x02,
	0x3004, 0x7070, 0x02,
	0x3006, 0x0114, 0x02,
	0x3008, 0x70b0, 0x02,
	0x300a, 0x1473, 0x02,
	0x300c, 0x0013, 0x02,
	0x300e, 0x140f, 0x02,
	0x3010, 0x0040, 0x02,
	0x3012, 0x100f, 0x02,
	0x3014, 0x70f8, 0x02,
	0x3016, 0x187f, 0x02,
	0x3018, 0x7070, 0x02,
	0x301a, 0x0114, 0x02,
	0x301c, 0x70b0, 0x02,
	0x301e, 0x1473, 0x02,
	0x3020, 0x0013, 0x02,
	0x3022, 0x140f, 0x02,
	0x3024, 0x0040, 0x02,
	0x3026, 0x000f, 0x02,
	0x0b00, 0x0000, 0x02,
	0x0b02, 0x0045, 0x02,
	0x0b04, 0xb405, 0x02,
	0x0b06, 0xc403, 0x02,
	0x0b08, 0x0081, 0x02,
	0x0b0a, 0x8252, 0x02,
	0x0b0c, 0xf814, 0x02,
	0x0b0e, 0xc618, 0x02,
	0x0b10, 0xa828, 0x02,
	0x0b12, 0x002c, 0x02,
	0x0b14, 0x4068, 0x02,
	0x0b16, 0x0000, 0x02,
	0x0f30, 0x6a25, 0x02,
	0x0f32, 0x7067, 0x02,
	0x0954, 0x0009, 0x02,
	0x0956, 0x0000, 0x02,
	0x095a, 0x5140, 0x02,
	0x0c00, 0x1110, 0x02,
	0x0c02, 0x0011, 0x02,
	0x0c04, 0x0000, 0x02,
	0x0c06, 0x0200, 0x02,
	0x0c10, 0x0040, 0x02,
	0x0c12, 0x0040, 0x02,
	0x0c14, 0x0040, 0x02,
	0x0c16, 0x0040, 0x02,
	0x0a10, 0x4000, 0x02,
	0x3068, 0xffff, 0x02,
	0x306a, 0xffff, 0x02,
	0x006c, 0x0300, 0x02,
	0x005e, 0x0200, 0x02,
	0x000e, 0x0100, 0x02,
	0x0e0a, 0x0001, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0022, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0202, 0x02,
	0x0012, 0x000e, 0x02,
	0x0018, 0x0a31, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0028, 0x02,
	0x002a, 0x002d, 0x02,
	0x0026, 0x0030, 0x02,
	0x002c, 0x07c7, 0x02,
	0x002e, 0x1111, 0x02,
	0x0030, 0x1111, 0x02,
	0x0032, 0x1111, 0x02,
	0x0006, 0x0823, 0x02,
	0x0116, 0x07b6, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0a20, 0x02,
	0x0a14, 0x0798, 0x02,
	0x003e, 0x0000, 0x02,
	0x0074, 0x080e, 0x02,
	0x0070, 0x0407, 0x02,
	0x0002, 0x0000, 0x02,
	0x0a02, 0x0000, 0x02,
	0x0a24, 0x0100, 0x02,
	0x0046, 0x0000, 0x02,
	0x0076, 0x0000, 0x02,
	0x0060, 0x0000, 0x02,
	0x0062, 0x0530, 0x02,
	0x0064, 0x0500, 0x02,
	0x0066, 0x0530, 0x02,
	0x0068, 0x0500, 0x02,
	0x0122, 0x0300, 0x02,
	0x015a, 0xff08, 0x02,
	0x0126, 0x00f9, 0x02,
	0x0804, 0x0200, 0x02,
	0x005c, 0x0100, 0x02,
	0x0a1a, 0x0800, 0x02,
};

/*
 * [Mode Information]
 *	0: 2592 x 1944 @30,     Full (4:3),  MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 836.7, MIPI Mclk(Mhz): 24.37
 *	1: 2560 x 1440 @30,     Full (16:9), MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 836.7, MIPI Mclk(Mhz): 24.37
 *	2: 1296 x 972  @30,     BIN2,        MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 418.35,  MIPI Mclk(Mhz): 24.37
 *	3: 1296 x 972  @15,     BIN2,        MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 418.35,  MIPI Mclk(Mhz): 24.37
 *	4: 1296 x 972  @7,      BIN2,        MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 418.35,  MIPI Mclk(Mhz): 24.37
 *	5: 648 x 484   @119.3,  BIN4,        MIPI lane: 2, MIPI data rate(Mbps/lane) Sensor: 209.18,  MIPI Mclk(Mhz): 24.37
 */


const u32 sensor_sr556_setfile_C_2592x1944_30fps[] = {
	0x0b0a, 0x8252, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7067, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0022, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0202, 0x02,
	0x0012, 0x000e, 0x02,
	0x0018, 0x0a31, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0028, 0x02,
	0x002a, 0x002d, 0x02,
	0x0026, 0x0030, 0x02,
	0x002c, 0x07c7, 0x02,
	0x002e, 0x1111, 0x02,
	0x0030, 0x1111, 0x02,
	0x0032, 0x1111, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x07c0, 0x02,
	0x0116, 0x07b6, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0a20, 0x02,
	0x0a14, 0x0798, 0x02,
	0x0074, 0x07be, 0x02,
	0x0070, 0x03df, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x014a, 0x02,
	0x090c, 0x0fdc, 0x02,
	0x090e, 0x002d, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc10a, 0x02,
	0x0916, 0x071f, 0x02,
	0x0918, 0x0408, 0x02,
	0x091a, 0x0c0d, 0x02,
	0x091c, 0x0f09, 0x02,
	0x091e, 0x0a00, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_2560x1440_30fps[] = {
	0x0b0a, 0x8252, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7067, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0022, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0202, 0x02,
	0x0012, 0x001e, 0x02,
	0x0018, 0x0a21, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0124, 0x02,
	0x002a, 0x0129, 0x02,
	0x0026, 0x012c, 0x02,
	0x002c, 0x06cb, 0x02,
	0x002e, 0x1111, 0x02,
	0x0030, 0x1111, 0x02,
	0x0032, 0x1111, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x07c0, 0x02,
	0x0116, 0x05be, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0a00, 0x02,
	0x0a14, 0x05a0, 0x02,
	0x0074, 0x07be, 0x02,
	0x0070, 0x03df, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x014a, 0x02,
	0x090c, 0x0fdc, 0x02,
	0x090e, 0x002d, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc10a, 0x02,
	0x0916, 0x071f, 0x02,
	0x0918, 0x0408, 0x02,
	0x091a, 0x0c0d, 0x02,
	0x091c, 0x0f09, 0x02,
	0x091e, 0x0a00, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_1296x972_30fps[] = {
	0x0b0a, 0x8259, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7167, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0122, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0404, 0x02,
	0x0012, 0x000c, 0x02,
	0x0018, 0x0a33, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0022, 0x02,
	0x002a, 0x002b, 0x02,
	0x0026, 0x0030, 0x02,
	0x002c, 0x07c7, 0x02,
	0x002e, 0x3311, 0x02,
	0x0030, 0x3311, 0x02,
	0x0032, 0x3311, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x07c0, 0x02,
	0x0116, 0x03ea, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0510, 0x02,
	0x0a14, 0x03cc, 0x02,
	0x0074, 0x07be, 0x02,
	0x0070, 0x03df, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x016a, 0x02,
	0x090c, 0x09c0, 0x02,
	0x090e, 0x0010, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc106, 0x02,
	0x0916, 0x040e, 0x02,
	0x0918, 0x0304, 0x02,
	0x091a, 0x0709, 0x02,
	0x091c, 0x0e06, 0x02,
	0x091e, 0x0300, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_1296x972_15fps[] = {
	0x0b0a, 0x8259, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7167, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0122, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0404, 0x02,
	0x0012, 0x000c, 0x02,
	0x0018, 0x0a33, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0022, 0x02,
	0x002a, 0x002b, 0x02,
	0x0026, 0x0030, 0x02,
	0x002c, 0x07c7, 0x02,
	0x002e, 0x3311, 0x02,
	0x0030, 0x3311, 0x02,
	0x0032, 0x3311, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x0f82, 0x02,
	0x0116, 0x03ea, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0510, 0x02,
	0x0a14, 0x03cc, 0x02,
	0x0074, 0x0f80, 0x02,
	0x0070, 0x07c0, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x016a, 0x02,
	0x090c, 0x09c0, 0x02,
	0x090e, 0x0010, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc106, 0x02,
	0x0916, 0x040e, 0x02,
	0x0918, 0x0304, 0x02,
	0x091a, 0x0709, 0x02,
	0x091c, 0x0e06, 0x02,
	0x091e, 0x0300, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_1296x972_7fps[] = {
	0x0b0a, 0x8259, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7167, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0122, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0404, 0x02,
	0x0012, 0x000c, 0x02,
	0x0018, 0x0a33, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0022, 0x02,
	0x002a, 0x002b, 0x02,
	0x0026, 0x0030, 0x02,
	0x002c, 0x07c7, 0x02,
	0x002e, 0x3311, 0x02,
	0x0030, 0x3311, 0x02,
	0x0032, 0x3311, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x213c, 0x02,
	0x0116, 0x03ea, 0x02,
	0x0a22, 0x0000, 0x02,
	0x0a12, 0x0510, 0x02,
	0x0a14, 0x03cc, 0x02,
	0x0074, 0x213a, 0x02,
	0x0070, 0x109d, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x016a, 0x02,
	0x090c, 0x09c0, 0x02,
	0x090e, 0x0010, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc106, 0x02,
	0x0916, 0x040e, 0x02,
	0x0918, 0x0304, 0x02,
	0x091a, 0x0709, 0x02,
	0x091c, 0x0e06, 0x02,
	0x091e, 0x0300, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_648x484_120fps[] = {
	0x0b0a, 0x8252, 0x02,
	0x0f30, 0x8135, 0x02,
	0x0f32, 0x7267, 0x02,
	0x004a, 0x0100, 0x02,
	0x004c, 0x0000, 0x02,
	0x004e, 0x0100, 0x02,
	0x000c, 0x0022, 0x02,
	0x0008, 0x0b00, 0x02,
	0x005a, 0x0208, 0x02,
	0x0012, 0x0008, 0x02,
	0x0018, 0x0a37, 0x02,
	0x0022, 0x0008, 0x02,
	0x0028, 0x0017, 0x02,
	0x0024, 0x0016, 0x02,
	0x002a, 0x001b, 0x02,
	0x0026, 0x0034, 0x02,
	0x002c, 0x07c3, 0x02,
	0x002e, 0x1111, 0x02,
	0x0030, 0x1111, 0x02,
	0x0032, 0x7711, 0x02,
	0x003c, 0x0101, 0x02,
	0x0006, 0x020c, 0x02,
	0x0116, 0x0202, 0x02,
	0x0a22, 0x0100, 0x02,
	0x0a12, 0x0288, 0x02,
	0x0a14, 0x01e4, 0x02,
	0x0074, 0x020a, 0x02,
	0x0070, 0x0105, 0x02,
	0x0804, 0x0200, 0x02,
	0x0a04, 0x016a, 0x02,
	0x090c, 0x0270, 0x02,
	0x090e, 0x0008, 0x02,
	0x0902, 0x4319, 0x02,
	0x0914, 0xc103, 0x02,
	0x0916, 0x0207, 0x02,
	0x0918, 0x0302, 0x02,
	0x091a, 0x0406, 0x02,
	0x091c, 0x0903, 0x02,
	0x091e, 0x0300, 0x02,
	0x0958, 0xaa80, 0x02,
};

const u32 sensor_sr556_setfile_C_Fsync_Normal[] = {
	0x0040, 0x0000, 0x02,
	0x0042, 0x0100, 0x02,
	0x003e, 0x0000, 0x02,
};

/* temporary VSYNC type
 * it need to check when use Master mode */
const u32 sensor_sr556_setfile_C_Fsync_Master[] = {
	0x0040, 0x0000, 0x02,
	0x0042, 0x0100, 0x02,
	0x003e, 0x0001, 0x02,
};

/* temporary Pos type
 * it need to check when use Slave mode */
const u32 sensor_sr556_setfile_C_Fsync_Slave[] = {
	0x0040, 0x0001, 0x02,
	0x0042, 0x0101, 0x02,
	0x003e, 0x0000, 0x02,
};

/*
  *    pixel rate calculation (Mpps)
  *    pll_voc_a = pll_info->ext_clk / pll_info->pre_pll_clk_div * pll_info->pll_multiplier;
  *    vt_pix_clk_hz = (pll_voc_a / pll_info->vt_pix_clk_div / pll_info->vt_sys_clk_div) * 2;
  *
  *    pre_pll_clk_div = 0x04
  *    pll_multiplier = 0x84
  *    vt_pix_clk_div = 0x01
  *    vt_sys_clk_div = 0x0a
  *    pll_voc_a = (26.00*1000*1000) / 0x04(4) * 0x84(132) = 858000000
  *    vt_pix_clk_hz = (858000000 / 1 / 10) * 2 = 171600000
  *    vt_pix_clk_div(0x01) and vt_sys_clk_div(0x0a) is the inversely calculated value
 */ 

const struct sensor_pll_info sensor_sr556_pllinfo_C_2592x1944_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div */
	0x0a, /* vt_sys_clk_div */
	0x04, /* pre_pll_clk_div */
	0x84, /* pll_multiplier */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div */

	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x07c0, /* frame_length_lines */
	0x0b00, /* line_length_pck */
};

const struct sensor_pll_info sensor_sr556_pllinfo_C_2560x1440_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div    */
	0x0a, /* vt_sys_clk_div    */
	0x04, /* pre_pll_clk_div   (0x0F32: PLL_prediv_ramp) */
	0x84, /* pll_multiplier    (0x0F33: PLL_mdiv_ramp) */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div	*/

	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x07c0, /* frame_length_lines	(0x0006) */
	0x0b00, /* line_length_pck		(0x0008) */
};

const struct sensor_pll_info sensor_sr556_pllinfo_C_1296x972_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div    */
	0x0a, /* vt_sys_clk_div    */
	0x04, /* pre_pll_clk_div   (0x0F32: PLL_prediv_ramp) */
	0x84, /* pll_multiplier    (0x0F33: PLL_mdiv_ramp) */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div	*/

	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x07c0, /* frame_length_lines	(0x0006) */
	0x0b00, /* line_length_pck		(0x0008) */
};

const struct sensor_pll_info sensor_sr556_pllinfo_C_1296x972_15fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div    */
	0x0a, /* vt_sys_clk_div    */
	0x04, /* pre_pll_clk_div   (0x0F32: PLL_prediv_ramp) */
	0x84, /* pll_multiplier    (0x0F33: PLL_mdiv_ramp) */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div	*/

	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x0f82, /* frame_length_lines	(0x0006) */
	0x0b00, /* line_length_pck		(0x0008) */
};

const struct sensor_pll_info sensor_sr556_pllinfo_C_1296x972_7fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div    */
	0x0a, /* vt_sys_clk_div    */
	0x04, /* pre_pll_clk_div   (0x0F32: PLL_prediv_ramp) */
	0x84, /* pll_multiplier    (0x0F33: PLL_mdiv_ramp) */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div	*/

	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x213c, /* frame_length_lines	(0x0006) */
	0x0b00, /* line_length_pck		(0x0008) */
};

const struct sensor_pll_info sensor_sr556_pllinfo_C_648x484_120fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div */
	0x0a, /* vt_sys_clk_div */
	0x04, /* pre_pll_clk_div */
	0x84, /* pll_multiplier */
	0x00, /* op_pix_clk_div */
	0x00, /* op_sys_clk_div */
	0x00, /* secnd_pre_pll_clk_div */
	0x00, /* secnd_pll_multiplier */
	0x020c, /* frame_length_lines */
	0x0b00, /* line_length_pck */
};

static const u32 *sensor_sr556_setfiles_C[] = {
	sensor_sr556_setfile_C_2592x1944_30fps,
	sensor_sr556_setfile_C_2560x1440_30fps,
	sensor_sr556_setfile_C_1296x972_30fps,
	sensor_sr556_setfile_C_1296x972_15fps,
	sensor_sr556_setfile_C_1296x972_7fps,
	sensor_sr556_setfile_C_648x484_120fps,
};

static const u32 sensor_sr556_setfile_C_sizes[] = {
	sizeof(sensor_sr556_setfile_C_2592x1944_30fps) / sizeof(sensor_sr556_setfile_C_2592x1944_30fps[0]),
	sizeof(sensor_sr556_setfile_C_2560x1440_30fps) / sizeof(sensor_sr556_setfile_C_2560x1440_30fps[0]),
	sizeof(sensor_sr556_setfile_C_1296x972_30fps) / sizeof(sensor_sr556_setfile_C_1296x972_30fps[0]),
	sizeof(sensor_sr556_setfile_C_1296x972_15fps) / sizeof(sensor_sr556_setfile_C_1296x972_15fps[0]),
	sizeof(sensor_sr556_setfile_C_1296x972_7fps) / sizeof(sensor_sr556_setfile_C_1296x972_7fps[0]),
	sizeof(sensor_sr556_setfile_C_648x484_120fps) / sizeof(sensor_sr556_setfile_C_648x484_120fps[0]),
};

static const struct sensor_pll_info *sensor_sr556_pllinfos_C[] = {
	&sensor_sr556_pllinfo_C_2592x1944_30fps,
	&sensor_sr556_pllinfo_C_2560x1440_30fps,
	&sensor_sr556_pllinfo_C_1296x972_30fps,
	&sensor_sr556_pllinfo_C_1296x972_15fps,
	&sensor_sr556_pllinfo_C_1296x972_7fps,
	&sensor_sr556_pllinfo_C_648x484_120fps,
};

#endif
