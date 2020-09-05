/* drivers/video/fbdev/exynos/dpu/panels/s6e3hf4_param.h
 *
 * Samsung SoC MIPI LCD CONTROL functions
 *
 * Copyright (c) 2016 Samsung Electronics
 *
 * Jiun Yu, <jiun.yu@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __S6E3HF4_PARAM_H__
#define __S6E3HF4_PARAM_H__

#include <linux/types.h>
#include <linux/kernel.h>

#define S6E3HF4_CODE_REG			0xD6
#define S6E3HF4_CODE_LEN			5

#define S6E3HF4_MAX_BRIGHTNESS	360
#define S6E3HF4_HBM_BRIGHTNESS	600

static unsigned char SEQ_TEST_KEY_ON_F0[] = {
	0xF0,
	0x5A, 0x5A
};
/* HACK for dsc err */
static const unsigned char SEQ_TEST_KEY_ON_BA[] = {
	0xBA,
	0x00
};

static const unsigned char _SEQ_TEST_KEY_ON_2A[] = {
	0x2A,
	0x00, 0x00, 0x02, 0xCF, 0x00, 0x00
};

static const unsigned char _SEQ_TEST_KEY_ON_2B[] = {
	0x2B,
	0x00, 0x00, 0x04, 0xff
};


static const unsigned char SEQ_TEST_KEY_OFF_F0[] = {
	0xF0,
	0xA5, 0xA5
};

static const unsigned char SEQ_TEST_KEY_ON_F1[] = {
	0xF1,
	0x5A, 0x5A,
};

static const unsigned char SEQ_TEST_KEY_OFF_F1[] = {
	0xF1,
	0xA5, 0xA5,
};

static const unsigned char SEQ_TEST_KEY_ON_F2[] = {
	0xF2,
	0x41, 0x0E, 0x06, 0x28, 0xB8, 0x80,
	0x54, 0xE0, 0xB4, 0x40, 0x09, 0x22,
};

static const unsigned char SEQ_TEST_KEY_ON_F4[] = {
	0xF4,
	0xAB, 0x1E, 0x13, 0x8A, 0x1F, 0x0C, 0x09, 0x00, 0x00,
};

static const unsigned char SEQ_TEST_KEY_ON_F6[] = {
	0xF6,
	0x43, 0x07, 0x17, 0x30, 0xAA, 0x00, 0xC3, 0xC5, 0xD1, 0x01,
};

static const unsigned char SEQ_TEST_KEY_ON_B1[] = {
	0xB1,
	0x10, 0x03, 0x10, 0x10, 0x10, 0x80, 0x40,
};

static const unsigned char SEQ_TEST_KEY_ON_B3[] = {
	0xB3,
	0x68,
};

static const unsigned char SEQ_TEST_KEY_ON_B4[] = {
	0xB4,
	0x50, 0x99, 0x27, 0x0A, 0x45,
};

static const unsigned char SEQ_TEST_KEY_ON_B5[] = {
	0xB5,
	0x19, 0xBC, 0x4A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x55,
	0x54, 0x20, 0x00, 0x00, 0x0A, 0xAA, 0xAF, 0x0F, 0x01, 0x11,
	0x11, 0x10, 0x00, 0x00, 0x8F, 0x52, 0x30, 0x00, 0x00, 0x00,
	0x00,
};

static const unsigned char SEQ_TEST_KEY_ON_C7[] = {
	0xC7,
	0x00,
};

static const unsigned char SEQ_TEST_KEY_ON_CB[] = {
	0xCB,
	0x12, 0x11, 0x81, 0x01, 0x00, 0x63, 0x82, 0x00, 0xE2, 0x0A,
	0x05, 0x00, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x15, 0x9A, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00,
	0x12, 0xCF, 0x00, 0x00, 0xD5, 0x11, 0x13, 0x0E, 0x45, 0x46,
	0xC2, 0x15, 0x15, 0xD5, 0xD5, 0xD5, 0xD5, 0xD1, 0x53, 0xCE,
	0xC5, 0xC6, 0x02, 0x15, 0x15, 0x15, 0x15, 0x15, 0x00, 0xE2,
	0x00, 0x00, 0x7B, 0x4C, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00,
};

static const unsigned char SEQ_TEST_KEY_ON_EB[] = {
	0xEB,
	0xFF, 0x53,
};

static const unsigned char SEQ_TEST_KEY_ON_CA[] = {
	0xCA,
	0x01, 0x5A, 0x01, 0x26, 0x01, 0x80, 0xDF, 0xDE, 0xDF, 0xDA,
	0xDA, 0xDC, 0xC2, 0xBE, 0xC3, 0xC7, 0xC2, 0xC8, 0xD4, 0xD0,
	0xD4, 0xCF, 0xCB, 0xCE, 0xA6, 0xA8, 0xA7, 0xA7, 0xC5, 0xAA,
	0x00, 0x00, 0x00, 0x33, 0x04,
};

static const unsigned char SEQ_TEST_KEY_ON_F7[] = {
	0xF7,
	0x03,
};

static const unsigned char SEQ_TEST_KEY_ON_53[] = {
	0x53,
	0x28,
};

static const unsigned char SEQ_TEST_KEY_ON_51[] = {
	0x51,
	0xFF,
};

static const unsigned char SEQ_TEST_KEY_ON_2A[] = {
	0x2A,
	0x00, 0x00, 0x05, 0x9F, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TEST_KEY_ON_2B[] = {
	0x2B,
	0x00, 0x00, 0x09, 0xFF,
};

static const unsigned char SEQ_TEST_KEY_ON_E5[] = {
	0xE5,
	0x23, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x03, 0x20,
};

static const unsigned char SEQ_TEST_KEY_ON_FC[] = {
	0xFC,
	0x5A, 0x5A
};

static const unsigned char SEQ_TEST_KEY_OFF_FC[] = {
	0xFC,
	0xA5, 0xA5
};

static const unsigned char SEQ_SLEEP_OUT[] = {
	0x11
};

static const unsigned char SEQ_SLEEP_IN[] = {
	0x10
};

static const unsigned char SEQ_DISPLAY_ON[] = {
	0x29
};

static const unsigned char SEQ_DISPLAY_OFF[] = {
	0x28
};


static const unsigned char SEQ_GAMMA_CONDITION_SET[] = {
	0xCA,
	0x01, 0x00, 0x01, 0x00, 0x01, 0x00,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x80, 0x80, 0x80,
	0x00, 0x00, 0x00,
	0x00, 0x00
};

static const unsigned char SEQ_AOR_CONTROL[] = {
	0xB1,
	0x10, 0x03
};

static const unsigned char SEQ_TSET_ELVSS_SET[] = {
	0xB5,
	0x19,	/* temperature 25 */
	0x9C,	/* MPS_CON: ACL OFF */
	0x0A	/* ELVSS: MAX*/
};

static const unsigned char SEQ_GAMMA_UPDATE[] = {
	0xF7,
	0x03
};

static const unsigned char SEQ_GAMMA_UPDATE_L[] = {
	0xF7,
	0x00,
};

static unsigned char SEQ_TSET_GLOBAL[] = {
	0xB0,
	0x01
};

static const unsigned char SEQ_TSET[] = {
	0xB5,
	0x19
};

static const unsigned char SEQ_VINT_SET[] = {
	0xF4,
	0xAB,                   /* VINT */
	0x1E                    /* 360nit */
};

static const unsigned char SEQ_TE_ON[] = {
	0x35,
	0x00
};
static const unsigned char SEQ_TE_OFF[] = {
	0x34,
};

static const unsigned char SEQ_TSP_TE[] = {
	0xBD,
	0x11, 0x11, 0x02, 0x16, 0x02, 0x16
};

static const unsigned char SEQ_PENTILE_SETTING[] = {
	0xC0,
	0x00, 0x00, 0xD8, 0xD8
};

static const unsigned char SEQ_POC_SETTING1[] = {
	0xB0,
	0x20
};

static const unsigned char SEQ_POC_SETTING2[] = {
	0xFE,
	0x04
};

static const unsigned char SEQ_PCD_SETTING[] = {
	0xCC,
	0x40, 0x51
};

static const unsigned char SEQ_ERR_FG_SETTING[] = {
	0xED,
	0x44
};

static const unsigned char SEQ_TE_START_SETTING[] = {
	0xB9,
	0x01, 0x0A, 0x07, 0x00, 0x0D
};

static const unsigned char SEQ_HBM_OFF[] = {
	0x53,
	0x00
};

static const unsigned char SEQ_HBM_ON[] = {
	0x53,
	0xC0
};
static const unsigned char SEQ_ACL_OFF[] = {
	0x55,
	0x00
};

static const unsigned char SEQ_ACL_8[] = {
	0x55,
	0x02,
};

static const unsigned char SEQ_ACL_OFF_OPR_AVR[] = {
	0xB4,
	0x40
};

static const unsigned char SEQ_ACL_ON_OPR_AVR[] = {
	0xB4,
	0x50
};

static const unsigned char SEQ_DSC_EN[] = {
	0x01, 0x00
};

static const unsigned char SEQ_PPS_SLICE4[] = {
	0x11, 0x00, 0x00, 0x89, 0x30,
	0x80, 0x0A, 0x00, 0x05, 0xA0,
	0x00, 0x40, 0x01, 0x68, 0x01,
	0x68, 0x02, 0x00, 0x01, 0xB4,

	0x00, 0x20, 0x04, 0xF2, 0x00,
	0x05, 0x00, 0x0C, 0x01, 0x87,
	0x02, 0x63, 0x18, 0x00, 0x10,
	0xF0, 0x03, 0x0C, 0x20, 0x00,

	0x06, 0x0B, 0x0B, 0x33, 0x0E,
	0x1C, 0x2A, 0x38, 0x46, 0x54,
	0x62, 0x69, 0x70, 0x77, 0x79,
	0x7B, 0x7D, 0x7E, 0x01, 0x02,

	0x01, 0x00, 0x09, 0x40, 0x09,
	0xBE, 0x19, 0xFC, 0x19, 0xFA,
	0x19, 0xF8, 0x1A, 0x38, 0x1A,
	0x78, 0x1A, 0xB6, 0x2A, 0xF6,

	0x2B, 0x34, 0x2B, 0x74, 0x3B,
	0x74, 0x6B, 0xF4, 0x00, 0x00
};

static const unsigned char SEQ_PPS_SLICE2[] = {
	0x11, 0x00, 0x00, 0x89, 0x30,
	0x80, 0x0A, 0x00, 0x05, 0xA0,
	0x00, 0x20, 0x02, 0xD0, 0x02,
	0xD0, 0x02, 0x00, 0x02, 0x68,

	0x00, 0x20, 0x03, 0x87, 0x00,
	0x0A, 0x00, 0x0C, 0x03, 0x19,
	0x02, 0x63, 0x18, 0x00, 0x10,
	0xF0, 0x03, 0x0C, 0x20, 0x00,

	0x06, 0x0B, 0x0B, 0x33, 0x0E,
	0x1C, 0x2A, 0x38, 0x46, 0x54,
	0x62, 0x69, 0x70, 0x77, 0x79,
	0x7B, 0x7D, 0x7E, 0x01, 0x02,

	0x01, 0x00, 0x09, 0x40, 0x09,
	0xBE, 0x19, 0xFC, 0x19, 0xFA,
	0x19, 0xF8, 0x1A, 0x38, 0x1A,
	0x78, 0x1A, 0xB6, 0x2A, 0xF6,

	0x2B, 0x34, 0x2B, 0x74, 0x3B,
	0x74, 0x6B, 0xF4, 0x00, 0x00
};

static const unsigned char SEQ_PPS_3[] = {
	0x11, 0x00, 0x00, 0x89,
	0x30, 0x80, 0x0a, 0x00, 0x05,
	0xa0, 0x00, 0x10, 0x05, 0xa0,
	0x05, 0xa0, 0x02, 0x00, 0x03,
	0xd0, 0x00, 0x20, 0x02, 0x33,
	0x00, 0x14, 0x00, 0x0c, 0x06,
	0x67, 0x02, 0x63, 0x18, 0x00,
	0x10, 0xf0, 0x03, 0x0c, 0x20,
	0x00, 0x06, 0x0b, 0x0b, 0x33,
	0x0e, 0x1c, 0x2a, 0x38, 0x46,
	0x54, 0x62, 0x69, 0x70, 0x77,
	0x79, 0x7b, 0x7d, 0x7e, 0x01,
	0x02, 0x01, 0x00, 0x09, 0x40,
	0x09, 0xbe, 0x19, 0xfc, 0x19,
	0xfa, 0x19, 0xf8, 0x1a, 0x38,
	0x1a, 0x78, 0x1a, 0xb6, 0x2a,
	0xf6, 0x2b, 0x34, 0x2b, 0x74,
	0x3b, 0x74, 0x6b, 0xf4, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

static const unsigned char SEQ_WQXGA_PARTIAL_UPDATE[] = {
	0x2A,
	0x00, 0x00, 0x05, 0x9F,
};

#endif /* __S6E3HF4_PARAM_H__ */
