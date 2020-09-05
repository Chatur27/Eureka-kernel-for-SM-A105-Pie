/*
 * Samsung Exynos5 SoC series Sensor driver
 *
 *
 * Copyright (c) 2016 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef FIMC_IS_CIS_IMX219_SET_A_H
#define FIMC_IS_CIS_IMX219_SET_A_H

#include "fimc-is-cis.h"
#include "fimc-is-cis-imx219.h"

/* 16x10 margin, EXTCLK 26Mhz */
const u32 sensor_imx219_setfile_A_3280x2458_30fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x0A, 0x01,
	0x0161, 0x7B, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x00, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xCF, 0x01,
	0x0168, 0x00, 0x01,
	0x0169, 0x04, 0x01,
	0x016A, 0x09, 0x01,
	0x016B, 0x9D, 0x01,
	0x016C, 0x0C, 0x01,
	0x016D, 0xD0, 0x01,
	0x016E, 0x09, 0x01,
	0x016F, 0x9A, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x00, 0x01,
	0x0175, 0x00, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x50, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x53, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const u32 sensor_imx219_setfile_A_3280x1846_30fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x0A, 0x01,
	0x0161, 0x79, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x00, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xCF, 0x01,
	0x0168, 0x01, 0x01,
	0x0169, 0x36, 0x01,
	0x016A, 0x08, 0x01,
	0x016B, 0x6B, 0x01,
	0x016C, 0x0C, 0x01,
	0x016D, 0xD0, 0x01,
	0x016E, 0x07, 0x01,
	0x016F, 0x36, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x00, 0x01,
	0x0175, 0x00, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x50, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x53, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const u32 sensor_imx219_setfile_A_1640x924_60fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x05, 0x01,
	0x0161, 0x3C, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x00, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xCF, 0x01,
	0x0168, 0x01, 0x01,
	0x0169, 0x34, 0x01,
	0x016A, 0x08, 0x01,
	0x016B, 0x6B, 0x01,
	0x016C, 0x06, 0x01,
	0x016D, 0x68, 0x01,
	0x016E, 0x03, 0x01,
	0x016F, 0x9C, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x01, 0x01,
	0x0175, 0x01, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x50, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x53, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const u32 sensor_imx219_setfile_A_1640x1232_60fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x04, 0x01,
	0x0161, 0x0F, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x00, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xCF, 0x01,
	0x0168, 0x01, 0x01,
	0x0169, 0x34, 0x01,
	0x016A, 0x08, 0x01,
	0x016B, 0x6B, 0x01,
	0x016C, 0x06, 0x01,
	0x016D, 0x68, 0x01,
	0x016E, 0x03, 0x01,
	0x016F, 0x9C, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x01, 0x01,
	0x0175, 0x01, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x3E, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x3E, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const u32 sensor_imx219_setfile_A_816x604_120fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x02, 0x01,
	0x0161, 0x9E, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x08, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xC7, 0x01,
	0x0168, 0x00, 0x01,
	0x0169, 0x18, 0x01,
	0x016A, 0x09, 0x01,
	0x016B, 0x87, 0x01,
	0x016C, 0x03, 0x01,
	0x016D, 0x30, 0x01,
	0x016E, 0x02, 0x01,
	0x016F, 0x5C, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x02, 0x01,
	0x0175, 0x02, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x50, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x53, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const u32 sensor_imx219_setfile_A_816x460_120fps[] = {
	0x30EB, 0x05, 0x01,
	0x30EB, 0x0C, 0x01,
	0x300A, 0xFF, 0x01,
	0x300B, 0xFF, 0x01,
	0x30EB, 0x05, 0x01,
	0x30EB, 0x09, 0x01,

	0x0114, 0x03, 0x01,
	0x0128, 0x00, 0x01,
	0x012A, 0x1A, 0x01,
	0x012B, 0x00, 0x01,
	0x0160, 0x02, 0x01,
	0x0161, 0x9E, 0x01,
	0x0162, 0x0D, 0x01,
	0x0163, 0x78, 0x01,
	0x0164, 0x00, 0x01,
	0x0165, 0x08, 0x01,
	0x0166, 0x0C, 0x01,
	0x0167, 0xC7, 0x01,
	0x0168, 0x01, 0x01,
	0x0169, 0x38, 0x01,
	0x016A, 0x08, 0x01,
	0x016B, 0x67, 0x01,
	0x016C, 0x03, 0x01,
	0x016D, 0x30, 0x01,
	0x016E, 0x01, 0x01,
	0x016F, 0xCC, 0x01,
	0x0170, 0x01, 0x01,
	0x0171, 0x01, 0x01,
	0x0174, 0x02, 0x01,
	0x0175, 0x02, 0x01,
	0x018C, 0x0A, 0x01,
	0x018D, 0x0A, 0x01,
	0x0301, 0x05, 0x01,
	0x0303, 0x01, 0x01,
	0x0304, 0x03, 0x01,
	0x0305, 0x03, 0x01,
	0x0306, 0x00, 0x01,
	0x0307, 0x50, 0x01,
	0x0309, 0x0A, 0x01,
	0x030B, 0x01, 0x01,
	0x030C, 0x00, 0x01,
	0x030D, 0x53, 0x01,
	0x4767, 0x0F, 0x01,
	0x4750, 0x14, 0x01,
	0x47B4, 0x14, 0x01,
};

const struct sensor_pll_info sensor_imx219_pllinfo_A_3280x2458_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x50, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x6C, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x0A7B, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */

};

const struct sensor_pll_info sensor_imx219_pllinfo_A_3280x1846_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x50, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x56, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x0A79, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */
};

const struct sensor_pll_info sensor_imx219_pllinfo_A_1640x924_60fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x50, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x4E, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x053C, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */
};

const struct sensor_pll_info sensor_imx219_pllinfo_A_1640x1232_60fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x3E, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x36, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x040F, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */
};

const struct sensor_pll_info sensor_imx219_pllinfo_A_816x604_120fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x50, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x30, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x029E, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */
};

const struct sensor_pll_info sensor_imx219_pllinfo_A_816x460_120fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x05, /* vt_pix_clk_div	(0x0301) */
	0x01, /* vt_sys_clk_div	(0x0303) */
	0x03, /* pre_pll_clk_div	(0x0304) */
	0x50, /* pll_multiplier	(H:0x306 L:0x0307) */
	0x0A, /* op_pix_clk_div	(0x0309) */
	0x01, /* op_sys_clk_div	(0x030B) */

	0x03, /* secnd_pre_pll_clk_div	(0x0305) */
	0x30, /* secnd_pll_multiplier	(H:0x030C L:0x030D) */
	0x029E, /* frame_length_lines	(H:0x0160 L:0x0161) */
	0x0D78, /* line_length_pck	(H:0x0162 L:0x0163) */
};

static const u32 *sensor_imx219_setfiles_A[] = {
	/* 16x12 margin */
	sensor_imx219_setfile_A_3280x2458_30fps,
	sensor_imx219_setfile_A_3280x1846_30fps,
	sensor_imx219_setfile_A_1640x924_60fps,
	sensor_imx219_setfile_A_1640x1232_60fps,
	sensor_imx219_setfile_A_816x604_120fps,
	sensor_imx219_setfile_A_816x460_120fps,
};

static const u32 sensor_imx219_setfile_A_sizes[] = {
	/* 16x12 margin */
	sizeof(sensor_imx219_setfile_A_3280x2458_30fps) / sizeof(sensor_imx219_setfile_A_3280x2458_30fps[0]),
	sizeof(sensor_imx219_setfile_A_3280x1846_30fps) / sizeof(sensor_imx219_setfile_A_3280x1846_30fps[0]),
	sizeof(sensor_imx219_setfile_A_1640x924_60fps) / sizeof(sensor_imx219_setfile_A_1640x924_60fps[0]),
	sizeof(sensor_imx219_setfile_A_1640x1232_60fps) / sizeof(sensor_imx219_setfile_A_1640x1232_60fps[0]),
	sizeof(sensor_imx219_setfile_A_816x604_120fps) / sizeof(sensor_imx219_setfile_A_816x604_120fps[0]),
	sizeof(sensor_imx219_setfile_A_816x460_120fps) / sizeof(sensor_imx219_setfile_A_816x460_120fps[0]),
};

static const struct sensor_pll_info *sensor_imx219_pllinfos_A[] = {
	/* 16x12 margin */
	&sensor_imx219_pllinfo_A_3280x2458_30fps,
	&sensor_imx219_pllinfo_A_3280x1846_30fps,
	&sensor_imx219_pllinfo_A_1640x924_60fps,
	&sensor_imx219_pllinfo_A_1640x1232_60fps,
	&sensor_imx219_pllinfo_A_816x604_120fps,
	&sensor_imx219_pllinfo_A_816x460_120fps,
};

#endif

