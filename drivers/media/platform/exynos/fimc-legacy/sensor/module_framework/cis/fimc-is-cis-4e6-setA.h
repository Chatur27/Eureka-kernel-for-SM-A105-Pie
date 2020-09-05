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

#ifndef FIMC_IS_CIS_4E6_SET_A_H
#define FIMC_IS_CIS_4E6_SET_A_H

#include "fimc-is-cis.h"
#include "fimc-is-cis-4e6.h"

/* EXTCLK 26Mhz */
const u32 sensor_4e6_setfile_A_2608x1960_30fps[] = {
	0x535A	,0xC700	,0x02
	,0x5402	,0x1500	,0x02
	,0x5400	,0x061D	,0x02
	,0x6102	,0xC000	,0x02
	,0x614C	,0x25AA	,0x02
	,0x614E	,0x25B8	,0x02
	,0x618C	,0x08D4	,0x02
	,0x618E	,0x08D6	,0x02
	,0x6028	,0x2000	,0x02
	,0x602A	,0x0668	,0x02
	,0x6F12	,0x4010	,0x02
	,0x602A	,0x11A8	,0x02
	,0x6F12	,0x3AF9	,0x02
	,0x6F12	,0x1410	,0x02
	,0x6F12	,0x39F9	,0x02
	,0x6F12	,0x1410	,0x02
	,0x6028	,0x2000	,0x02
	,0x602A	,0x0524	,0x02
	,0x6F12	,0x0007	,0x02
	,0x602A	,0x12BC	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x12C2	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x12CA	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1010	,0x02
	,0x602A	,0x12FC	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x1302	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x130A	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1010	,0x02
	,0x602A	,0x14B8	,0x02
	,0x6F12	,0x0101	,0x02
	,0x602A	,0x14C0	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0x0000	,0x02
	,0x602A	,0x1488	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x602A	,0x1496	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x602A	,0x14A4	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x602A	,0x147A	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0x0002	,0x02
	,0x6F12	,0xFFFC	,0x02
	,0x602A	,0x0512	,0x02
	,0x6F12	,0x0111	,0x02
	,0x602A	,0x14AC	,0x02
	,0x6F12	,0x0000	,0x02
	,0x327A	,0x0001	,0x02
	,0x3282	,0x000A	,0x02
	,0x3296	,0x0418	,0x02
	,0x32E0	,0x0000	,0x02
	,0x3286	,0x9000	,0x02
	,0x3298	,0x4007	,0x02
	,0x32AA	,0x0000	,0x02
	,0x327C	,0x0400	,0x02
	,0x328A	,0x0800	,0x02
	,0x3284	,0x3700	,0x02
	,0x32A0	,0x0320	,0x02
	,0x32A2	,0x1000	,0x02
	,0x32A4	,0x0C00	,0x02
	,0x3204	,0x000C	,0x02
	,0x3206	,0x000B	,0x02
	,0x3208	,0x0009	,0x02
	,0x3210	,0x0007	,0x02
	,0x3212	,0x0007	,0x02
	,0x3218	,0x031c	,0x02
	,0x321A	,0x3224	,0x02
	,0x321C	,0x0700	,0x02
	,0x321E	,0x0800	,0x02
	,0x3220	,0x1300	,0x02
	,0x3226	,0x525C	,0x02
	,0x3228	,0x0304	,0x02
	,0x0200	,0x0408	,0x02
	,0x5428	,0x1A00	,0x02
	,0x3412	,0x65AA	,0x02
	,0x535E	,0x0132	,0x02
	,0x5360	,0xC800	,0x02
	,0x0304	,0x0006	,0x02
	,0x0306	,0x00A6	,0x02
	,0x3300	,0x0000	,0x02
	,0x5362	,0x0A00	,0x02
	,0x5364	,0x31A0	,0x02
	,0x534E	,0x4910	,0x02
	,0x5330	,0xC403	,0x02
	,0x0340	,0x07F8	,0x02
	,0x0342	,0x0B68	,0x02
	,0x0202	,0x03FC	,0x02
	,0x021E	,0x03FC	,0x02
	,0x0344	,0x0000	,0x02
	,0x0346	,0x0000	,0x02
	,0x0348	,0x0A2F	,0x02
	,0x034A	,0x07A7	,0x02
	,0x034C	,0x0A30	,0x02
	,0x034E	,0x07A8	,0x02
	,0x0382	,0x0001	,0x02
	,0x0386	,0x0001	,0x02
	,0x3500	,0x0022	,0x02
	,0x3088	,0x0000	,0x02
	,0x0216	,0x0100	,0x02
	,0x5332	,0x04	,0x01
#if defined(CONFIG_SOC_EXYNOS8895)
	,0x5333	,0xE0	,0x01
#else
	,0x5333	,0xE2	,0x01
#endif
	,0x5080	,0x0100	,0x02
};

/* EXTCLK 26Mhz */
const u32 sensor_4e6_setfile_A_1304x980_30fps[] = {
	0x535B, 0x00  , 0x01,
	0x5402, 0x15  , 0x01,
	0x5401, 0x1D  , 0x01,
	0x6102, 0xC000, 0x02,
	0x614C, 0x25AA, 0x02,
	0x614E, 0x25B8, 0x02,
	0x618C, 0x08D4, 0x02,
	0x618E, 0x08D6, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x11A8, 0x02,
	0x6F12, 0x3AF9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6F12, 0x39F9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x12BC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12C2, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12CA, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x12FC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x1302, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x130A, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x14B8, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14B9, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14C0, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x1488, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x1496, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x14A4, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x602A, 0x147A, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0002, 0x02,
	0x6F12, 0xFFFC, 0x02,
	0x602A, 0x0512, 0x02,
	0x6F12, 0x0111, 0x02,
	0x602A, 0x066A, 0x02,
	0x6F12, 0x4110, 0x02,
	0x602A, 0x14AC, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x0524, 0x02,
	0x6F12, 0x00  , 0x01,
	0x3285, 0x00  , 0x01,
	0x327A, 0x0001, 0x02,
	0x3283, 0x0A  , 0x01,
	0x3297, 0x18  , 0x01,
	0x32E1, 0x00  , 0x01,
	0x3286, 0x9000, 0x02,
	0x3298, 0x40  , 0x01,
	0x32AA, 0x01  , 0x01,
	0x327C, 0x0400, 0x02,
	0x328A, 0x0800, 0x02,
	0x3284, 0x37  , 0x01,
	0x32A1, 0x20  , 0x01,
	0x32A2, 0x10  , 0x01,
	0x32A4, 0x0C  , 0x01,
	0x3204, 0x000C, 0x02,
	0x3206, 0x000B, 0x02,
	0x3208, 0x0009, 0x02,
	0x3210, 0x0007, 0x02,
	0x3212, 0x0007, 0x02,
	0x0200, 0x0408, 0x02,
	0x3219, 0x1C  , 0x01,
	0x321A, 0x32  , 0x01,
	0x321B, 0x24  , 0x01,
	0x321C, 0x07  , 0x01,
	0x321E, 0x08  , 0x01,
	0x3220, 0x13  , 0x01,
	0x3226, 0x52  , 0x01,
	0x3227, 0x5C  , 0x01,
	0x3228, 0x03  , 0x01,
	0x0305, 0x0A  , 0x01,
	0x0306, 0x01  , 0x01,
	0x0307, 0x15  , 0x01,
	0x5363, 0x00  , 0x01,
	0x5364, 0x31  , 0x01,
	0x5365, 0xD0  , 0x01,
	0x534E, 0x49  , 0x01,
	0x534F, 0x10  , 0x01,
	0x535F, 0x32  , 0x01,
	0x5360, 0xC8  , 0x01,
	0x3412, 0x65  , 0x01,
	0x3413, 0xAA  , 0x01,
	0x5428, 0x1A  , 0x01,
	0x0340, 0x0424, 0x02,
	0x0342, 0x1612, 0x02,
	0x021E, 0x03FC, 0x02,
	0x0344, 0x0000, 0x02,
	0x0346, 0x0000, 0x02,
	0x0348, 0x0A2F, 0x02,
	0x034A, 0x07A7, 0x02,
	0x034C, 0x0518, 0x02,
	0x034E, 0x03D4, 0x02,
	0x3500, 0x01  , 0x01,
	0x3089, 0x01  , 0x01,
	0x0216, 0x01  , 0x01,
	0x5333, 0xE0  , 0x01,
	0x5080, 0x01  , 0x01,
};

/* EXTCLK 26Mhz */
const u32 sensor_4e6_setfile_A_1304x980_15fps[] = {
	0x535B, 0x00  , 0x01,
	0x5402, 0x15  , 0x01,
	0x5401, 0x1D  , 0x01,
	0x6102, 0xC000, 0x02,
	0x614C, 0x25AA, 0x02,
	0x614E, 0x25B8, 0x02,
	0x618C, 0x08D4, 0x02,
	0x618E, 0x08D6, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x11A8, 0x02,
	0x6F12, 0x3AF9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6F12, 0x39F9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x12BC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12C2, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12CA, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x12FC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x1302, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x130A, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x14B8, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14B9, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14C0, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x1488, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x1496, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x14A4, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x602A, 0x147A, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0002, 0x02,
	0x6F12, 0xFFFC, 0x02,
	0x602A, 0x0512, 0x02,
	0x6F12, 0x0111, 0x02,
	0x602A, 0x066A, 0x02,
	0x6F12, 0x4110, 0x02,
	0x602A, 0x14AC, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x0524, 0x02,
	0x6F12, 0x00  , 0x01,
	0x3285, 0x00  , 0x01,
	0x327A, 0x0001, 0x02,
	0x3283, 0x0A  , 0x01,
	0x3297, 0x18  , 0x01,
	0x32E1, 0x00  , 0x01,
	0x3286, 0x9000, 0x02,
	0x3298, 0x40  , 0x01,
	0x32AA, 0x01  , 0x01,
	0x327C, 0x0400, 0x02,
	0x328A, 0x0800, 0x02,
	0x3284, 0x37  , 0x01,
	0x32A1, 0x20  , 0x01,
	0x32A2, 0x10  , 0x01,
	0x32A4, 0x0C  , 0x01,
	0x3204, 0x000C, 0x02,
	0x3206, 0x000B, 0x02,
	0x3208, 0x0009, 0x02,
	0x3210, 0x0007, 0x02,
	0x3212, 0x0007, 0x02,
	0x0200, 0x0408, 0x02,
	0x3219, 0x1C  , 0x01,
	0x321A, 0x32  , 0x01,
	0x321B, 0x24  , 0x01,
	0x321C, 0x07  , 0x01,
	0x321E, 0x08  , 0x01,
	0x3220, 0x13  , 0x01,
	0x3226, 0x52  , 0x01,
	0x3227, 0x5C  , 0x01,
	0x3228, 0x03  , 0x01,
	0x3301, 0x01  , 0x01,
	0x0305, 0x06  , 0x01,
	0x0307, 0x9E  , 0x01,
	0x5363, 0x00  , 0x01,
	0x5364, 0x32  , 0x01,
	0x5365, 0x7A  , 0x01,
	0x534E, 0x49  , 0x01,
	0x535F, 0x32  , 0x01,
	0x5360, 0xC8  , 0x01,
	0x3412, 0x65  , 0x01,
	0x3413, 0xAA  , 0x01,
	0x5428, 0x1A  , 0x01,
	0x534F, 0x10  , 0x01,
	0x0340, 0x07DA, 0x02,
	0x0342, 0x0B04, 0x02,
	0x021E, 0x03FC, 0x02,
	0x0344, 0x0000, 0x02,
	0x0346, 0x0000, 0x02,
	0x0348, 0x0A2F, 0x02,
	0x034A, 0x07A7, 0x02,
	0x034C, 0x0518, 0x02,
	0x034E, 0x03D4, 0x02,
	0x3500, 0x01  , 0x01,
	0x3089, 0x01  , 0x01,
	0x0216, 0x01  , 0x01,
	0x5333, 0xE0  , 0x01,
	0x5080, 0x01  , 0x01,
};

/* EXTCLK 26Mhz */
const u32 sensor_4e6_setfile_A_1304x980_7fps[] = {
	0x535B, 0x00  , 0x01,
	0x5402, 0x15  , 0x01,
	0x5401, 0x1D  , 0x01,
	0x6102, 0xC000, 0x02,
	0x614C, 0x25AA, 0x02,
	0x614E, 0x25B8, 0x02,
	0x618C, 0x08D4, 0x02,
	0x618E, 0x08D6, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x11A8, 0x02,
	0x6F12, 0x3AF9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6F12, 0x39F9, 0x02,
	0x6F12, 0x1410, 0x02,
	0x6028, 0x2000, 0x02,
	0x602A, 0x12BC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12C2, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x12CA, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x12FC, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x1302, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1020, 0x02,
	0x602A, 0x130A, 0x02,
	0x6F12, 0x1020, 0x02,
	0x6F12, 0x1010, 0x02,
	0x602A, 0x14B8, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14B9, 0x02,
	0x6F12, 0x01  , 0x01,
	0x602A, 0x14C0, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0xFFDA, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x1488, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x1496, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x6F12, 0xFF80, 0x02,
	0x602A, 0x14A4, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x6F12, 0xFFC0, 0x02,
	0x602A, 0x147A, 0x02,
	0x6F12, 0x0000, 0x02,
	0x6F12, 0x0002, 0x02,
	0x6F12, 0xFFFC, 0x02,
	0x602A, 0x0512, 0x02,
	0x6F12, 0x0111, 0x02,
	0x602A, 0x066A, 0x02,
	0x6F12, 0x4110, 0x02,
	0x602A, 0x14AC, 0x02,
	0x6F12, 0x0000, 0x02,
	0x602A, 0x0524, 0x02,
	0x6F12, 0x00  , 0x01,
	0x3285, 0x00  , 0x01,
	0x327A, 0x0001, 0x02,
	0x3283, 0x0A  , 0x01,
	0x3297, 0x18  , 0x01,
	0x32E1, 0x00  , 0x01,
	0x3286, 0x9000, 0x02,
	0x3298, 0x40  , 0x01,
	0x32AA, 0x01  , 0x01,
	0x327C, 0x0400, 0x02,
	0x328A, 0x0800, 0x02,
	0x3284, 0x37  , 0x01,
	0x32A1, 0x20  , 0x01,
	0x32A2, 0x10  , 0x01,
	0x32A4, 0x0C  , 0x01,
	0x3204, 0x000C, 0x02,
	0x3206, 0x000B, 0x02,
	0x3208, 0x0009, 0x02,
	0x3210, 0x0007, 0x02,
	0x3212, 0x0007, 0x02,
	0x0200, 0x0408, 0x02,
	0x3219, 0x1C  , 0x01,
	0x321A, 0x32  , 0x01,
	0x321B, 0x24  , 0x01,
	0x321C, 0x07  , 0x01,
	0x321E, 0x08  , 0x01,
	0x3220, 0x13  , 0x01,
	0x3226, 0x52  , 0x01,
	0x3227, 0x5C  , 0x01,
	0x3228, 0x03  , 0x01,
	0x3301, 0x01  , 0x01,
	0x0305, 0x06  , 0x01,
	0x0307, 0x9E  , 0x01,
	0x5363, 0x00  , 0x01,
	0x5364, 0x32  , 0x01,
	0x5365, 0x7A  , 0x01,
	0x534E, 0x49  , 0x01,
	0x535F, 0x32  , 0x01,
	0x5360, 0xC8  , 0x01,
	0x3412, 0x65  , 0x01,
	0x3413, 0xAA  , 0x01,
	0x5428, 0x1A  , 0x01,
	0x534F, 0x10  , 0x01,
	0x0340, 0x10CC, 0x02,
	0x0342, 0x0B04, 0x02,
	0x021E, 0x03FC, 0x02,
	0x0344, 0x0000, 0x02,
	0x0346, 0x0000, 0x02,
	0x0348, 0x0A2F, 0x02,
	0x034A, 0x07A7, 0x02,
	0x034C, 0x0518, 0x02,
	0x034E, 0x03D4, 0x02,
	0x3500, 0x01  , 0x01,
	0x3089, 0x01  , 0x01,
	0x0216, 0x01  , 0x01,
	0x5333, 0xE0  , 0x01,
	0x5080, 0x01  , 0x01,
};

/* EXTCLK 26Mhz */
const u32 sensor_4e6_setfile_A_652x488_112fps[] = {
	0x535A	,0xC700	,0x02
	,0x5402	,0x1500	,0x02
	,0x5400	,0x061D	,0x02
	,0x6102	,0xC000	,0x02
	,0x614C	,0x25AA	,0x02
	,0x614E	,0x25B8	,0x02
	,0x618C	,0x08D4	,0x02
	,0x618E	,0x08D6	,0x02
	,0x6028	,0x2000	,0x02
	,0x602A	,0x066A	,0x02
	,0x6F12	,0x4110	,0x02
	,0x602A	,0x11A8	,0x02
	,0x6F12	,0x3AF9	,0x02
	,0x6F12	,0x1410	,0x02
	,0x6F12	,0x39F9	,0x02
	,0x6F12	,0x1410	,0x02
	,0x6028	,0x2000	,0x02
	,0x602A	,0x0524	,0x02
	,0x6F12	,0x0007	,0x02
	,0x602A	,0x12BC	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x12C2	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x12CA	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1010	,0x02
	,0x602A	,0x12FC	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x1302	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1020	,0x02
	,0x602A	,0x130A	,0x02
	,0x6F12	,0x1020	,0x02
	,0x6F12	,0x1010	,0x02
	,0x602A	,0x14B8	,0x02
	,0x6F12	,0x0101	,0x02
	,0x602A	,0x14C0	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0xFFDA	,0x02
	,0x6F12	,0x0000	,0x02
	,0x602A	,0x1488	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x602A	,0x1496	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x6F12	,0xFF80	,0x02
	,0x602A	,0x14A4	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x6F12	,0xFFC0	,0x02
	,0x602A	,0x147A	,0x02
	,0x6F12	,0x0000	,0x02
	,0x6F12	,0x0002	,0x02
	,0x6F12	,0xFFFC	,0x02
	,0x602A	,0x0512	,0x02
	,0x6F12	,0x0111	,0x02
	,0x602A	,0x14AC	,0x02
	,0x6F12	,0x0000	,0x02
	,0x327A	,0x0001	,0x02
	,0x3282	,0x000A	,0x02
	,0x3296	,0x0418	,0x02
	,0x32E0	,0x0000	,0x02
	,0x3286	,0x9000	,0x02
	,0x3298	,0x4007	,0x02
	,0x32AA	,0x0100	,0x02
	,0x327C	,0x0400	,0x02
	,0x328A	,0x0800	,0x02
	,0x3284	,0x3700	,0x02
	,0x32A0	,0x0320	,0x02
	,0x32A2	,0x1000	,0x02
	,0x32A4	,0x0C00	,0x02
	,0x3204	,0x000C	,0x02
	,0x3206	,0x000B	,0x02
	,0x3208	,0x0009	,0x02
	,0x3210	,0x0007	,0x02
	,0x3212	,0x0007	,0x02
	,0x3218	,0x031c	,0x02
	,0x321A	,0x3224	,0x02
	,0x321C	,0x0700	,0x02
	,0x321E	,0x0800	,0x02
	,0x3220	,0x1300	,0x02
	,0x3226	,0x525C	,0x02
	,0x3228	,0x0304	,0x02
	,0x0200	,0x0408	,0x02
	,0x5428	,0x1A00	,0x02
	,0x3412	,0x65AA	,0x02
	,0x535E	,0x0132	,0x02
	,0x5360	,0xC800	,0x02
	,0x0304	,0x0006	,0x02
	,0x0306	,0x00A6	,0x02
	,0x3300	,0x0000	,0x02
	,0x5362	,0x0A00	,0x02
	,0x5364	,0x31A0	,0x02
	,0x534E	,0x4910	,0x02
	,0x5330	,0xD403	,0x02
	,0x0340	,0x0238	,0x02
	,0x0342	,0x0B04	,0x02
	,0x0202	,0x01F4	,0x02
	,0x021E	,0x01F4	,0x02
	,0x0344	,0x0000	,0x02
	,0x0346	,0x0000	,0x02
	,0x0348	,0x0A2F	,0x02
	,0x034A	,0x079F	,0x02
	,0x034C	,0x028C	,0x02
	,0x034E	,0x01E8	,0x02
	,0x0382	,0x0003	,0x02
	,0x0386	,0x0003	,0x02
	,0x3500	,0x0122	,0x02
	,0x3088	,0x0001	,0x02
	,0x0216	,0x0100	,0x02
	,0x5332	,0x04	,0x01
#if defined(CONFIG_SOC_EXYNOS8895)
	,0x5333	,0xE0	,0x01
#else
	,0x5333	,0xE2	,0x01
#endif
	,0x5080	,0x0100	,0x02
};

/* TODO: 0x0301, 0x3C1C(vt_pix, vt_sys clk div need to check */
const struct sensor_pll_info sensor_4e6_pllinfo_A_2608x1960_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div  (0x0301) */
	0x05, /* vt_sys_clk_div  (0x3C1C[7:4]) */
	0x06, /* pre_pll_clk_div (0x0305) */
	0xA6, /* pll_multiplier        (0x0307) */
	0x00, /* op_pix_clk_div        (0x0309) */
	0x00, /* op_sys_clk_div  (0x030B) */

	0x00, /* secnd_pre_pll_clk_div	(0x030D) */
	0x00, /* secnd_pll_multiplier	(0x030F) */
	0x07F8, /* frame_length_lines    (0x0341) */
	0x0B68, /* line_length_pck       (0x0343) */
};

const struct sensor_pll_info sensor_4e6_pllinfo_A_1304x980_30fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div  (0x0301) */
	0x05, /* vt_sys_clk_div  (0x3C1C[7:4]) */
	0x06, /* pre_pll_clk_div (0x0305) */
	0xA6, /* pll_multiplier        (0x0307) */
	0x00, /* op_pix_clk_div        (0x0309) */
	0x00, /* op_sys_clk_div  (0x030B) */

	0x00, /* secnd_pre_pll_clk_div	(0x030D) */
	0x00, /* secnd_pll_multiplier	(0x030F) */
	0x0424, /* frame_length_lines    (0x0341) */
	0x1612, /* line_length_pck       (0x0343) */
};

const struct sensor_pll_info sensor_4e6_pllinfo_A_1304x980_15fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div  (0x0301) */
	0x05, /* vt_sys_clk_div  (0x3C1C[7:4]) */
	0x06, /* pre_pll_clk_div (0x0305) */
	0x9C, /* pll_multiplier        (0x0307) */
	0x00, /* op_pix_clk_div        (0x0309) */
	0x00, /* op_sys_clk_div  (0x030B) */

	0x00, /* secnd_pre_pll_clk_div	(0x030D) */
	0x00, /* secnd_pll_multiplier	(0x030F) */
	0x07DA, /* frame_length_lines    (0x0341) */
	0x0B04, /* line_length_pck       (0x0343) */
};

const struct sensor_pll_info sensor_4e6_pllinfo_A_1304x980_7fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div  (0x0301) */
	0x05, /* vt_sys_clk_div  (0x3C1C[7:4]) */
	0x06, /* pre_pll_clk_div (0x0305) */
	0x9C, /* pll_multiplier        (0x0307) */
	0x00, /* op_pix_clk_div        (0x0309) */
	0x00, /* op_sys_clk_div  (0x030B) */

	0x00, /* secnd_pre_pll_clk_div	(0x030D) */
	0x00, /* secnd_pll_multiplier	(0x030F) */
	0x10CC, /* frame_length_lines    (0x0341) */
	0x0B04, /* line_length_pck       (0x0343) */
};

const struct sensor_pll_info sensor_4e6_pllinfo_A_652x488_112fps = {
	EXT_CLK_Mhz * 1000 * 1000, /* ext_clk */
	0x01, /* vt_pix_clk_div  (0x0301) */
	0x05, /* vt_sys_clk_div  (0x3C1C[7:4]) */
	0x06, /* pre_pll_clk_div (0x0305) */
	0xA6, /* pll_multiplier        (0x0307) */
	0x00, /* op_pix_clk_div        (0x0309) */
	0x00, /* op_sys_clk_div  (0x030B) */

	0x00, /* secnd_pre_pll_clk_div	(0x030D) */
	0x00, /* secnd_pll_multiplier	(0x030F) */
	0x0238, /* frame_length_lines    (0x0341) */
	0x0B04, /* line_length_pck       (0x0343) */
};

static const u32 *sensor_4e6_setfiles_A[] = {
	sensor_4e6_setfile_A_2608x1960_30fps,
	sensor_4e6_setfile_A_652x488_112fps,
	sensor_4e6_setfile_A_1304x980_30fps,
	sensor_4e6_setfile_A_1304x980_15fps,
	sensor_4e6_setfile_A_1304x980_7fps,
};

static const u32 sensor_4e6_setfile_A_sizes[] = {
	sizeof(sensor_4e6_setfile_A_2608x1960_30fps) / sizeof(sensor_4e6_setfile_A_2608x1960_30fps[0]),
	sizeof(sensor_4e6_setfile_A_652x488_112fps) / sizeof(sensor_4e6_setfile_A_652x488_112fps[0]),
	sizeof(sensor_4e6_setfile_A_1304x980_30fps) / sizeof(sensor_4e6_setfile_A_1304x980_30fps[0]),
	sizeof(sensor_4e6_setfile_A_1304x980_15fps) / sizeof(sensor_4e6_setfile_A_1304x980_15fps[0]),
	sizeof(sensor_4e6_setfile_A_1304x980_7fps) / sizeof(sensor_4e6_setfile_A_1304x980_7fps[0]),
};

static const struct sensor_pll_info *sensor_4e6_pllinfos_A[] = {
	&sensor_4e6_pllinfo_A_2608x1960_30fps,
	&sensor_4e6_pllinfo_A_652x488_112fps,
	&sensor_4e6_pllinfo_A_1304x980_30fps,
	&sensor_4e6_pllinfo_A_1304x980_15fps,
	&sensor_4e6_pllinfo_A_1304x980_7fps,
};

#endif

