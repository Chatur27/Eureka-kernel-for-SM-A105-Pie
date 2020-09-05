/*
 * Samsung Exynos5 SoC series FIMC-IS driver
 *
 * exynos5 fimc-is video functions
 *
 * Copyright (c) 2017 Samsung Electronics Co., Ltd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

const u32 fimc_is_csi_settle_table[] = {
	/* mipi_speed,	settle */
	2100,		46,
	2090,		46,
	2080,		46,
	2070,		45,
	2060,		45,
	2050,		45,
	2040,		45,
	2030,		44,
	2020,		44,
	2010,		44,
	2000,		44,
	1990,		44,
	1980,		43,
	1970,		43,
	1960,		43,
	1950,		43,
	1940,		42,
	1930,		42,
	1920,		42,
	1910,		42,
	1900,		42,
	1890,		41,
	1880,		41,
	1870,		41,
	1860,		41,
	1850,		40,
	1840,		40,
	1830,		40,
	1820,		40,
	1810,		40,
	1800,		39,
	1790,		39,
	1780,		39,
	1770,		39,
	1760,		38,
	1750,		38,
	1740,		38,
	1730,		38,
	1720,		38,
	1710,		37,
	1700,		37,
	1690,		37,
	1680,		37,
	1670,		36,
	1660,		36,
	1650,		36,
	1640,		36,
	1630,		36,
	1620,		35,
	1610,		35,
	1600,		35,
	1590,		35,
	1580,		34,
	1570,		34,
	1560,		34,
	1550,		34,
	1540,		34,
	1530,		33,
	1520,		33,
	1510,		33,
	1500,		33,
	1490,		32,
	1480,		32,
	1470,		32,
	1460,		32,
	1450,		32,
	1440,		31,
	1430,		31,
	1420,		31,
	1410,		31,
	1400,		30,
	1390,		30,
	1380,		30,
	1370,		30,
	1360,		30,
	1350,		29,
	1340,		29,
	1330,		29,
	1320,		29,
	1310,		28,
	1300,		28,
	1290,		28,
	1280,		28,
	1270,		28,
	1260,		27,
	1250,		27,
	1240,		27,
	1230,		27,
	1220,		26,
	1210,		26,
	1200,		26,
	1190,		26,
	1180,		26,
	1170,		25,
	1160,		25,
	1150,		25,
	1140,		25,
	1130,		25,
	1120,		24,
	1110,		24,
	1100,		24,
	1090,		24,
	1080,		23,
	1070,		23,
	1060,		23,
	1050,		23,
	1040,		23,
	1030,		22,
	1020,		22,
	1010,		22,
	1000,		22,
	990,		21,
	980,		21,
	970,		21,
	960,		21,
	950,		21,
	940,		20,
	930,		20,
	920,		20,
	910,		20,
	900,		19,
	890,		19,
	880,		19,
	870,		19,
	860,		19,
	850,		18,
	840,		18,
	830,		18,
	820,		18,
	810,		17,
	800,		17,
	790,		17,
	780,		17,
	770,		17,
	760,		16,
	750,		16,
	740,		16,
	730,		16,
	720,		15,
	710,		15,
	700,		15,
	690,		15,
	680,		15,
	670,		14,
	660,		14,
	650,		14,
	640,		14,
	630,		13,
	620,		13,
	610,		13,
	600,		13,
	590,		13,
	580,		12,
	570,		12,
	560,		12,
	550,		12,
	540,		11,
	530,		11,
	520,		11,
	510,		11,
	500,		11,
	490,		10,
	480,		10,
	470,		10,
	460,		10,
	450,		9,
	440,		9,
	430,		9,
	420,		9,
	410,		9,
	400,		8,
	390,		8,
	380,		8,
	370,		8,
	360,		7,
	350,		7,
	340,		7,
	330,		7,
	320,		7,
	310,		6,
	300,		6,
	290,		6,
	280,		6,
	270,		5,
	260,		5,
	250,		5,
	240,		5,
	230,		5,
	220,		4,
	210,		4,
	200,		4,
	190,		4,
	180,		3,
	170,		3,
	160,		3,
	150,		3,
	140,		3,
	130,		2,
	120,		2,
	110,		2,
	100,		2,
	90,		1,
	80,		1,
};
