#ifndef __HX8279D_WISDOM_PARAM_H__
#define __HX8279D_WISDOM_PARAM_H__

#include <linux/types.h>
#include <linux/kernel.h>

#define EXTEND_BRIGHTNESS	306
#define UI_MAX_BRIGHTNESS	255
#define UI_DEFAULT_BRIGHTNESS	117

#define HX8279D_ID_REG				0xFC
#define HX8279D_DUAL_REG				0xB1

#define HX8279D_ID_LEN				3 /* 1 */

#define PANEL_STATE_SUSPENED	0
#define PANEL_STATE_RESUMED		1
#define PANEL_STATE_SUSPENDING	2

struct mipi_cmd {
	u8 cmd[2];
};

static const struct mipi_cmd SEQ_CMD_TABLE[] = {
	{{0xB0, 0x01} },
	{{0xC8, 0x00} },
	{{0xC9, 0x00} },
	{{0xCC, 0x26} },
	{{0xCD, 0x26} },
	{{0xDC, 0x00} },
	{{0xDD, 0x00} },
	{{0xE0, 0x26} },
	{{0xE1, 0x26} },
	{{0xB0, 0x03} },
	{{0xC8, 0x0D} },
	{{0xC9, 0x0B} },
	{{0xC3, 0x2A} },
	{{0xE7, 0x2A} },
	{{0xC5, 0x2A} },
	{{0xDE, 0x2A} },
	{{0xB0, 0x00} },
	{{0xB6, 0x03} },
	{{0xBA, 0x87} },
	{{0xBF, 0x15} },
	{{0xC0, 0x11} },
	{{0xC2, 0x04} },
	{{0xC3, 0x02} },
	{{0xC4, 0x04} },
	{{0xC5, 0x02} },
	{{0xB0, 0x06} },
	{{0xC0, 0xA5} },
	{{0xD5, 0x3F} },
	{{0xB0, 0x02} },
	{{0xC0, 0x00} },
	{{0xC1, 0x11} },
	{{0xC2, 0x1C} },
	{{0xC3, 0x2E} },
	{{0xC4, 0x3D} },
	{{0xC5, 0x3D} },
	{{0xC6, 0x3A} },
	{{0xC7, 0x38} },
	{{0xC8, 0x36} },
	{{0xC9, 0x35} },
	{{0xCA, 0x35} },
	{{0xCB, 0x35} },
	{{0xCC, 0x38} },
	{{0xCD, 0x2D} },
	{{0xCE, 0x2E} },
	{{0xCF, 0x32} },
	{{0xD0, 0x04} },
	{{0xD2, 0x00} },
	{{0xD3, 0x0D} },
	{{0xD4, 0x18} },
	{{0xD5, 0x28} },
	{{0xD6, 0x33} },
	{{0xD7, 0x35} },
	{{0xD8, 0x36} },
	{{0xD9, 0x38} },
	{{0xDA, 0x38} },
	{{0xDB, 0x33} },
	{{0xDC, 0x35} },
	{{0xDD, 0x37} },
	{{0xDE, 0x3A} },
	{{0xDF, 0x2D} },
	{{0xE0, 0x2E} },
	{{0xE1, 0x2C} },
	{{0xE2, 0x01} },
	{{0xB0, 0x07} },
	{{0xB1, 0x04} },
	{{0xB2, 0x04} },
	{{0xB3, 0x08} },
	{{0xB4, 0x16} },
	{{0xB5, 0x21} },
	{{0xB6, 0x2F} },
	{{0xB7, 0x4D} },
	{{0xB8, 0x6C} },
	{{0xB9, 0xB0} },
	{{0xBA, 0xF9} },
	{{0xBB, 0x87} },
	{{0xBC, 0x1A} },
	{{0xBD, 0x1F} },
	{{0xBE, 0xA4} },
	{{0xBF, 0x1C} },
	{{0xC0, 0x55} },
	{{0xC1, 0x89} },
	{{0xC2, 0xA2} },
	{{0xC3, 0xBB} },
	{{0xC4, 0xC6} },
	{{0xC5, 0xD2} },
	{{0xC6, 0xDC} },
	{{0xC7, 0xE1} },
	{{0xC8, 0xE4} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x06} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x08} },
	{{0xB1, 0x04} },
	{{0xB2, 0x03} },
	{{0xB3, 0x09} },
	{{0xB4, 0x17} },
	{{0xB5, 0x22} },
	{{0xB6, 0x30} },
	{{0xB7, 0x4F} },
	{{0xB8, 0x6E} },
	{{0xB9, 0xB3} },
	{{0xBA, 0xFC} },
	{{0xBB, 0x8A} },
	{{0xBC, 0x1E} },
	{{0xBD, 0x23} },
	{{0xBE, 0xA8} },
	{{0xBF, 0x21} },
	{{0xC0, 0x59} },
	{{0xC1, 0x8D} },
	{{0xC2, 0xA6} },
	{{0xC3, 0xBF} },
	{{0xC4, 0xCA} },
	{{0xC5, 0xD5} },
	{{0xC6, 0xE0} },
	{{0xC7, 0xE5} },
	{{0xC8, 0xE8} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x06} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x09} },
	{{0xB1, 0x04} },
	{{0xB2, 0x06} },
	{{0xB3, 0x0A} },
	{{0xB4, 0x18} },
	{{0xB5, 0x24} },
	{{0xB6, 0x32} },
	{{0xB7, 0x53} },
	{{0xB8, 0x73} },
	{{0xB9, 0xBB} },
	{{0xBA, 0x09} },
	{{0xBB, 0x9C} },
	{{0xBC, 0x34} },
	{{0xBD, 0x38} },
	{{0xBE, 0xC2} },
	{{0xBF, 0x3A} },
	{{0xC0, 0x72} },
	{{0xC1, 0xA5} },
	{{0xC2, 0xBF} },
	{{0xC3, 0xD5} },
	{{0xC4, 0xDF} },
	{{0xC5, 0xE9} },
	{{0xC6, 0xF4} },
	{{0xC7, 0xFA} },
	{{0xC8, 0xFC} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x16} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x0A} },
	{{0xB1, 0x04} },
	{{0xB2, 0x04} },
	{{0xB3, 0x08} },
	{{0xB4, 0x16} },
	{{0xB5, 0x21} },
	{{0xB6, 0x2F} },
	{{0xB7, 0x4D} },
	{{0xB8, 0x6C} },
	{{0xB9, 0xB0} },
	{{0xBA, 0xF9} },
	{{0xBB, 0x87} },
	{{0xBC, 0x1A} },
	{{0xBD, 0x1F} },
	{{0xBE, 0xA4} },
	{{0xBF, 0x1C} },
	{{0xC0, 0x55} },
	{{0xC1, 0x89} },
	{{0xC2, 0xA2} },
	{{0xC3, 0xBB} },
	{{0xC4, 0xC6} },
	{{0xC5, 0xD2} },
	{{0xC6, 0xDC} },
	{{0xC7, 0xE1} },
	{{0xC8, 0xE4} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x06} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x0B} },
	{{0xB1, 0x04} },
	{{0xB2, 0x03} },
	{{0xB3, 0x09} },
	{{0xB4, 0x17} },
	{{0xB5, 0x22} },
	{{0xB6, 0x30} },
	{{0xB7, 0x4F} },
	{{0xB8, 0x6E} },
	{{0xB9, 0xB3} },
	{{0xBA, 0xFC} },
	{{0xBB, 0x8A} },
	{{0xBC, 0x1E} },
	{{0xBD, 0x23} },
	{{0xBE, 0xA8} },
	{{0xBF, 0x21} },
	{{0xC0, 0x59} },
	{{0xC1, 0x8D} },
	{{0xC2, 0xA6} },
	{{0xC3, 0xBF} },
	{{0xC4, 0xCA} },
	{{0xC5, 0xD5} },
	{{0xC6, 0xE0} },
	{{0xC7, 0xE5} },
	{{0xC8, 0xE8} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x06} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x0C} },
	{{0xB1, 0x04} },
	{{0xB2, 0x06} },
	{{0xB3, 0x0A} },
	{{0xB4, 0x18} },
	{{0xB5, 0x24} },
	{{0xB6, 0x32} },
	{{0xB7, 0x53} },
	{{0xB8, 0x73} },
	{{0xB9, 0xBB} },
	{{0xBA, 0x09} },
	{{0xBB, 0x9C} },
	{{0xBC, 0x34} },
	{{0xBD, 0x38} },
	{{0xBE, 0xC2} },
	{{0xBF, 0x3A} },
	{{0xC0, 0x72} },
	{{0xC1, 0xA5} },
	{{0xC2, 0xBF} },
	{{0xC3, 0xD5} },
	{{0xC4, 0xDF} },
	{{0xC5, 0xE9} },
	{{0xC6, 0xF4} },
	{{0xC7, 0xFA} },
	{{0xC8, 0xFC} },
	{{0xC9, 0x00} },
	{{0xCA, 0x00} },
	{{0xCB, 0x16} },
	{{0xCC, 0xAF} },
	{{0xCD, 0xFF} },
	{{0xCE, 0xFF} },
	{{0xB0, 0x04} },
	{{0xB5, 0x02} },
	{{0xB6, 0x01} },
};

static unsigned char SEQ_SLEEP_IN[] = {
	0xB2, 0x00
};

static unsigned char SEQ_TABLE_0[] = {
	0xB0, 0x00
};

static unsigned char SEQ_TABLE_4[] = {
	0xB0, 0x04
};

static unsigned char SEQ_TABLE_5[] = {
	0xB0, 0x05
};

static unsigned char SEQ_EOTP_DISABLE[] = {
	0xB1, 0xE5
};

static const unsigned char SEQ_TLPX_80NS[] = {
	0xB3, 0x52
};

static unsigned char SEQ_DISPLAY_ON[] = {
	0xB3, 0x08
};

static unsigned char SEQ_DISPLAY_OFF[] = {
	0xB3, 0x8C // Display off, BL off
};

static unsigned char SEQ_SCREEN_OFF[] = {
	0xB3, 0x88 // Display off
};

static unsigned char SEQ_BL_00[] = {
	0xB8, 0x00 // BL off
};

/* platform brightness <-> bl reg */
static unsigned int brightness_table[EXTEND_BRIGHTNESS + 1] = {
	0,
	1, 1, 2, 2, 3, 3, 4, 5, 5, 6, /* 1: 1 */
	6, 7, 7, 8, 9, 9, 10, 10, 11, 12,
	12, 13, 13, 14, 14, 15, 16, 16, 17, 17,
	18, 19, 19, 20, 20, 21, 21, 22, 23, 23,
	24, 24, 25, 26, 26, 27, 27, 28, 28, 29,
	30, 30, 31, 31, 32, 33, 33, 34, 34, 35,
	35, 36, 37, 37, 38, 38, 39, 40, 40, 41,
	41, 42, 42, 43, 44, 44, 45, 45, 46, 47,
	47, 48, 48, 49, 49, 50, 51, 51, 52, 52,
	53, 54, 54, 55, 55, 56, 56, 57, 58, 58,
	59, 59, 60, 61, 61, 62, 62, 63, 63, 64,
	65, 65, 66, 66, 67, 68, 68, 69, 69, 70,
	70, 71, 72, 72, 73, 73, 74, 75, 75, 76, /* 128: 75 */
	77, 78, 79, 80, 81, 82, 83, 84, 85, 86,
	87, 88, 89, 90, 91, 92, 93, 94, 95, 96,
	97, 98, 99, 100, 101, 102, 103, 104, 105, 106,
	107, 108, 109, 110, 111, 112, 113, 114, 115, 116,
	116, 117, 118, 119, 120, 121, 122, 123, 124, 125,
	126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
	136, 137, 138, 139, 140, 141, 142, 143, 144, 145,
	146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
	156, 157, 157, 158, 159, 160, 161, 162, 163, 164,
	165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
	175, 176, 177, 178, 179, 180, 181, 182, 183, 184,
	185, 186, 187, 188, 189, 190, 191, 192, 193, 194,
	195, 196, 197, 198, 199, 199, 200, 200, 201, 202, /* 255: 199 */
	202, 203, 204, 204, 205, 206, 206, 207, 207, 208,
	209, 209, 210, 211, 211, 212, 213, 213, 214, 215,
	215, 216, 216, 217, 218, 218, 219, 220, 220, 221,
	222, 222, 223, 223, 224, 225, 225, 226, 227, 227,
	228, 229, 229, 230, 231, 232,
};

#endif /* __HX8279D_WISDOM_PARAM_H__ */

