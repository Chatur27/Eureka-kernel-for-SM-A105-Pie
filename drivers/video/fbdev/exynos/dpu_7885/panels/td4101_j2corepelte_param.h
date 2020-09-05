#ifndef __TD4101_PARAM_H__
#define __TD4101_PARAM_H__
#include <linux/types.h>
#include <linux/kernel.h>

#define EXTEND_BRIGHTNESS	306
#define UI_MAX_BRIGHTNESS	255
#define UI_DEFAULT_BRIGHTNESS	128

struct lcd_seq_info {
	unsigned char	*cmd;
	unsigned int	len;
	unsigned int	sleep;
};

struct i2c_rom_data {
	u8 addr;
	u8 val;
};

static const struct i2c_rom_data LM3632_INIT[] = {
	{0x09, 0x41},
	{0x02, 0x50},
	{0x03, 0x0D},
	{0x04, 0x07},
	{0x05, 0xFF},
	{0x0A, 0x19},
	{0x0D, 0x24},
	{0x0E, 0x26},
	{0x0F, 0x26},
	{0x0C, 0x1F},
};

static const unsigned char SEQ_TD4101_BLON[] = {
	0x53,
	0x0C,
};

static const unsigned char SEQ_TD4101_CABC_MODE[] = {
	0x55,
	0x03,
};

static const unsigned char SEQ_TD4101_CABC_MIN[] = {
	0x5E,
	0x30,
};

static const unsigned char SEQ_TD4101_BL[] = {
	0x51,
	0xFF,
};

static const unsigned char SEQ_TD4101_ADDRESS[] = {
	0x36,
	0x40, 0x00
};

static const unsigned char SEQ_TEON_CTL[] = {
	0x35,
	0x01,
};

static const unsigned char SEQ_SLEEP_OUT[] = {
	0x11,
	0x00, 0x00
};

const const unsigned char SEQ_SLEEP_IN[] = {
	0x10,
	0x00, 0x00
};

const const unsigned char SEQ_DISPLAY_OFF[] = {
	0x28,
	0x00, 0x00
};

static const unsigned char SEQ_DISPLAY_ON[] = {
	0x29,
	0x00, 0x00
};

static const unsigned char SEQ_TD4101_CABC_OFF[] = {
	0x55,
	0x00,
};


static const unsigned char SEQ_TD4101_B0[] = {
	0xB0,
	0x04,
};

static const unsigned char SEQ_TD4101_B3[] = {
	0xB3,
	0x10, 0x00, 0x06,
};

static const unsigned char SEQ_TD4101_B4[] = {
	0xB4,
	0x00, 0x01,
};

static const unsigned char SEQ_TD4101_B5[] = {
	0xB5,
	0x00, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_B6[] = {
	0xB6,
	0x32, 0x53, 0x80, 0x00, 0x00, 0x07, 0x86,
};

static const unsigned char SEQ_TD4101_B7[] = {
	0xB7,
	0x00,
};

static const unsigned char SEQ_TD4101_BA[] = {
	0xBA,
	0x07, 0x87, 0x3A, 0x0A, 0x3D, 0x88,
};

static const unsigned char SEQ_TD4101_BB[] = {
	0xBB,
	0x00, 0xB4, 0xA0,
};

static const unsigned char SEQ_TD4101_BC[] = {
	0xBC,
	0x00, 0xB4, 0xA0,
};

static const unsigned char SEQ_TD4101_BD[] = {
	0xBD,
	0x00, 0xB4, 0xA0,
};

static const unsigned char SEQ_TD4101_BE[] = {
	0xBE,
	0x04,
};

static const unsigned char SEQ_TD4101_BF[] = {
	0xBF,
	0x02, 0x3c, 0x80, 0x09,
};

static const unsigned char SEQ_TD4101_C0[] = {
	0xC0,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_C1[] = {
	0xC1,
	0x04, 0x48, 0x01, 0x03, 0x33, 0x08, 0x11, 0x00, 0x11, 0x00,
	0x73, 0x23, 0x23, 0x11, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDF, 0x00, 0x30, 0x00,
	0x01, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_C2[] = {
	0xC2,
	0x00, 0xF0, 0x03, 0xC0, 0x0A, 0x04, 0x08, 0x00, 0x24, 0x19,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00,
};

static const unsigned char SEQ_TD4101_C3[] = {
	0xC3,
	0x3D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x01, 0x01, 0x03, 0x28, 0x00, 0x01, 0x03, 0x01, 0x44, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF0, 0x00,
	0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x40, 0x20, 0x03,
};

static const unsigned char SEQ_TD4101_C4[] = {
	0xC4,
	0x70, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01,
};

static const unsigned char SEQ_TD4101_C5[] = {
	0xC5,
	0x08, 0x00, 0x00, 0x00, 0x00, 0x70, 0x00, 0x00, 0x2D, 0x41,
};

static const unsigned char SEQ_TD4101_C6[] = {
	0xC6,
	0xFA, 0x54, 0xD7, 0x00, 0x00, 0x54, 0xD7, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x80, 0xFA,
};

static const unsigned char SEQ_TD4101_C7[] = {
	0xC7,
	0x00, 0x0B, 0x14, 0x22, 0x2F, 0x38, 0x52, 0x65, 0x75, 0x83,
	0x38, 0x43, 0x53, 0x6E, 0x7B, 0x87, 0x8D, 0x96, 0xB8, 0x00,
	0x0B, 0x14, 0x22, 0x2F, 0x38, 0x52, 0x65, 0x75, 0x83, 0x38,
	0x49, 0x59, 0x6E, 0x7B, 0x87, 0x8D, 0x96, 0xB8, 0x00, 0x97,
	0x00, 0x97, 0x00, 0x97, 0x00, 0x97,
};

static const unsigned char SEQ_TD4101_C8[] = {
	0xC8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00,
	0x00, 0x00, 0x00, 0xFC, 0x00,
};

static const unsigned char SEQ_TD4101_C9[] = {
	0xC9,
	0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x00, 0x00, 0x00,
	0x00, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFC, 0x00,
};

static const unsigned char SEQ_TD4101_CA[] = {
	0xCA,
	0x1D, 0xFC, 0xFC, 0xFC, 0x00, 0xD9, 0xDA, 0x70, 0x00, 0xF0,
	0x26, 0xBA, 0x00, 0x9A, 0x00, 0x02, 0x39, 0xD6, 0x00, 0xEE,
	0x00, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_CC[] = {
	0xCC,
	0xD2, 0x72, 0x46, 0x42, 0x12, 0x16, 0x1A, 0x1E, 0x00, 0xD0,
	0x70, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x70, 0xD0, 0x00, 0x1C, 0x18, 0x14, 0x10,
	0x40, 0x44, 0x72, 0xD2, 0x00,
};

static const unsigned char SEQ_TD4101_CD[] = {
	0xCD,
	0xD3, 0x03, 0xF2, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_CE[] = {
	0xCE,
	0x7D, 0x40, 0x48, 0x56, 0x67, 0x78, 0x88, 0x98, 0xA7, 0xB5,
	0xC3, 0xD1, 0xDE, 0xE9, 0xF2, 0xFA, 0xFF, 0x3C, 0x00, 0x01,
	0x04, 0x45, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_CF[] = {
	0xCF,
	0x48, 0x10,
};

static const unsigned char SEQ_TD4101_D0[] = {
	0xD0,
	0x11, 0x04, 0x59, 0xE3, 0x03, 0x10, 0x10, 0x40, 0x19, 0x08,
	0x99, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_D1[] = {
	0xD1,
	0x04,
};

static const unsigned char SEQ_TD4101_D3[] = {
	0xD3,
	0xBB, 0x3B, 0x33, 0x3B, 0x44, 0x3B, 0x44, 0x3B, 0x00, 0x00,
	0xEC, 0x91, 0x87, 0x23, 0x22, 0xE7, 0xE7, 0x3B, 0xBB, 0x4F,
	0xD0, 0x3C, 0x10, 0x12, 0x10, 0x00, 0x10,
};

static const unsigned char SEQ_TD4101_D4[] = {
	0xD4,
	0x80, 0x04, 0x04, 0x33, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x90,
	0x05, 0x00, 0x64, 0x94,
};

static const unsigned char SEQ_TD4101_D6[] = {
	0xD6,
	0x41,
};

static const unsigned char SEQ_TD4101_D7[] = {
	0xD7,
	0xF6, 0xFF, 0x03, 0x05, 0x43, 0x24, 0x80, 0x1F, 0xC7, 0x1F,
	0x1B, 0x00, 0x0F, 0x01, 0x20, 0x08, 0x80, 0x3F, 0x00, 0x78,
	0x00, 0x40, 0x24, 0x15, 0x00, 0x33, 0x02, 0xC0, 0xAF, 0xCB,
	0x60, 0x30, 0xFC, 0x00, 0x3F, 0x00,
};

static const unsigned char SEQ_TD4101_D8[] = {
	0xD8,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static const unsigned char SEQ_TD4101_DD[] = {
	0xDD,
	0x30, 0x06, 0x23, 0x65,
};

static const unsigned char SEQ_TD4101_DE[] = {
	0xDE,
	0x00, 0xFF, 0xFF, 0x90,
};

/* platform brightness <-> bl reg */
static unsigned int brightness_table[EXTEND_BRIGHTNESS + 1] = {
	[0 ... 2]	=	0,
	[3 ... 4]	=	2,
	[5 ... 6]	=	3,
	[7 ... 8]	=	4,
	[9 ... 10]	=	5,
	[11 ... 12]	=	6,
	[13 ... 15]	=	7,
	[16 ... 16]	=	8,
	[17 ... 17]	=	9,
	[18 ... 18]	=	10,
	[19 ... 19]	=	11,
	[20 ... 21]	=	12,
	[22 ... 23]	=	13,
	[24 ... 24]	=	14,
	[25 ... 26]	=	15,
	[27 ... 28]	=	16,
	[29 ... 30]	=	17,
	[31 ... 31]	=	18,
	[32 ... 33]	=	19,
	[34 ... 35]	=	20,
	[36 ... 36]	=	21,
	[37 ... 38]	=	22,
	[39 ... 40]	=	23,
	[41 ... 41]	=	24,
	[42 ... 43]	=	25,
	[44 ... 45]	=	26,
	[46 ... 46]	=	27,
	[47 ... 48]	=	28,
	[49 ... 50]	=	29,
	[51 ... 51]	=	30,
	[52 ... 53]	=	31,
	[54 ... 55]	=	32,
	[56 ... 57]	=	33,
	[58 ... 58]	=	34,
	[59 ... 60]	=	35,
	[61 ... 62]	=	36,
	[63 ... 63]	=	37,
	[64 ... 65]	=	38,
	[66 ... 67]	=	39,
	[68 ... 68]	=	40,
	[69 ... 70]	=	41,
	[71 ... 72]	=	42,
	[73 ... 73]	=	43,
	[74 ... 75]	=	44,
	[76 ... 77]	=	45,
	[78 ... 79]	=	46,
	[80 ... 80]	=	47,
	[81 ... 82]	=	48,
	[83 ... 84]	=	49,
	[85 ... 85]	=	50,
	[86 ... 87]	=	51,
	[88 ... 89]	=	52,
	[90 ... 90]	=	53,
	[91 ... 92]	=	54,
	[93 ... 94]	=	55,
	[95 ... 95]	=	56,
	[96 ... 97]	=	57,
	[98 ... 99]	=	58,
	[100 ... 100]	=	59,
	[101 ... 102]	=	60,
	[103 ... 104]	=	61,
	[105 ... 106]	=	62,
	[107 ... 107]	=	63,
	[108 ... 109]	=	64,
	[110 ... 111]	=	65,
	[112 ... 112]	=	66,
	[113 ... 114]	=	67,
	[115 ... 116]	=	68,
	[117 ... 117]	=	69,
	[118 ... 119]	=	70,
	[120 ... 121]	=	71,
	[122 ... 122]	=	72,
	[123 ... 124]	=	73,
	[125 ... 126]	=	74,
	[127 ... 127]	=	75,
	[128 ... 129]	=	76,
	[130 ... 130]	=	77,
	[131 ... 131]	=	78,
	[132 ... 132]	=	79,
	[133 ... 134]	=	80,
	[135 ... 135]	=	81,
	[136 ... 136]	=	82,
	[137 ... 137]	=	83,
	[138 ... 138]	=	84,
	[139 ... 140]	=	85,
	[141 ... 141]	=	86,
	[142 ... 142]	=	87,
	[143 ... 143]	=	88,
	[144 ... 145]	=	89,
	[146 ... 146]	=	90,
	[147 ... 147]	=	91,
	[148 ... 148]	=	92,
	[149 ... 149]	=	93,
	[150 ... 151]	=	94,
	[152 ... 152]	=	95,
	[153 ... 153]	=	96,
	[154 ... 154]	=	97,
	[155 ... 156]	=	98,
	[157 ... 157]	=	99,
	[158 ... 158]	=	100,
	[159 ... 159]	=	101,
	[160 ... 160]	=	102,
	[161 ... 162]	=	103,
	[163 ... 163]	=	104,
	[164 ... 164]	=	105,
	[165 ... 165]	=	106,
	[166 ... 167]	=	107,
	[168 ... 168]	=	108,
	[169 ... 169]	=	109,
	[170 ... 170]	=	110,
	[171 ... 171]	=	111,
	[172 ... 173]	=	112,
	[174 ... 174]	=	113,
	[175 ... 175]	=	114,
	[176 ... 176]	=	115,
	[177 ... 178]	=	116,
	[179 ... 179]	=	117,
	[180 ... 180]	=	118,
	[181 ... 181]	=	119,
	[182 ... 182]	=	120,
	[183 ... 184]	=	121,
	[185 ... 185]	=	122,
	[186 ... 186]	=	123,
	[187 ... 187]	=	124,
	[188 ... 189]	=	125,
	[190 ... 190]	=	126,
	[191 ... 191]	=	127,
	[192 ... 192]	=	128,
	[193 ... 193]	=	129,
	[194 ... 195]	=	130,
	[196 ... 196]	=	131,
	[197 ... 197]	=	132,
	[198 ... 198]	=	133,
	[199 ... 200]	=	134,
	[201 ... 201]	=	135,
	[202 ... 202]	=	136,
	[203 ... 203]	=	137,
	[204 ... 204]	=	138,
	[205 ... 206]	=	139,
	[207 ... 207]	=	140,
	[208 ... 208]	=	141,
	[209 ... 209]	=	142,
	[210 ... 211]	=	143,
	[212 ... 212]	=	144,
	[213 ... 213]	=	145,
	[214 ... 214]	=	146,
	[215 ... 215]	=	147,
	[216 ... 217]	=	148,
	[218 ... 218]	=	149,
	[219 ... 219]	=	150,
	[220 ... 220]	=	151,
	[221 ... 222]	=	152,
	[223 ... 223]	=	153,
	[224 ... 224]	=	154,
	[225 ... 225]	=	155,
	[226 ... 226]	=	156,
	[227 ... 228]	=	157,
	[229 ... 229]	=	158,
	[230 ... 230]	=	159,
	[231 ... 231]	=	160,
	[232 ... 233]	=	161,
	[234 ... 234]	=	162,
	[235 ... 235]	=	163,
	[236 ... 236]	=	164,
	[237 ... 237]	=	165,
	[238 ... 239]	=	166,
	[240 ... 240]	=	167,
	[241 ... 241]	=	168,
	[242 ... 242]	=	169,
	[243 ... 244]	=	170,
	[245 ... 245]	=	171,
	[246 ... 246]	=	172,
	[247 ... 247]	=	173,
	[248 ... 248]	=	174,
	[249 ... 250]	=	175,
	[251 ... 251]	=	176,
	[252 ... 252]	=	177,
	[253 ... 253]	=	178,
	[254 ... 254]	=	179,
	[255 ... 305]	=	180,
	[306 ... 306]	=	242,
};

#endif /* __TD4101_PARAM_H__ */
