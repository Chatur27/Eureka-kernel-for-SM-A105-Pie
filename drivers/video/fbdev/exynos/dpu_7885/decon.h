/*
 * Copyright (c) 2016 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com
 *
 * Header file for Exynos DECON driver
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef ___SAMSUNG_DECON_H__
#define ___SAMSUNG_DECON_H__

#include <linux/fb.h>
#include <linux/kernel.h>
#include <linux/clk.h>
#include <linux/spinlock.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/kthread.h>
#include <linux/pm_qos.h>
#include <linux/delay.h>
#include <linux/seq_file.h>
#include <linux/platform_device.h>
#include <media/v4l2-device.h>
#include <media/videobuf2-core.h>
#include <soc/samsung/bts.h>
#include <soc/samsung/exynos-itmon.h>

#include "regs-decon.h"
#include "./panels/decon_lcd.h"
#include "dsim.h"
#include "../../../../staging/android/sw_sync.h"

#define MAX_DECON_CNT		3
#define SUCCESS_EXYNOS_SMC	0

extern struct ion_device *ion_exynos;
extern struct decon_device *decon_drvdata[MAX_DECON_CNT];
extern int decon_log_level;
extern int dpu_bts_log_level;
extern int win_update_log_level;
extern struct decon_bts_ops decon_bts_control;

#define DECON_MODULE_NAME	"exynos-decon"
#define MAX_NAME_SIZE		32
#define MAX_PLANE_CNT		3
#define DECON_ENTER_HIBER_CNT	3
#define DECON_ENTER_LPD_CNT	3
#define MIN_BLK_MODE_WIDTH	144
#define MIN_BLK_MODE_HEIGHT	16
#define VSYNC_TIMEOUT_MSEC	200
#define DEFAULT_BPP		32

#define MAX_DECON_WIN		4
#define MAX_DPP_SUBDEV		4	/* check later */

#define MIN_WIN_BLOCK_WIDTH	360
#define MIN_WIN_BLOCK_HEIGHT	222

#if defined(CONFIG_DPU_20)
#define CHIP_VER		(7885)
#define FD_TRY_CNT		3
#define VALID_FD_VAL		3
#endif
#define DECON_WIN_UPDATE_IDX	(4)

#ifndef KHZ
#define KHZ (1000)
#endif
#ifndef MHZ
#define MHZ (1000*1000)
#endif

#define SHADOW_UPDATE_TIMEOUT	(300 * 1000) /* 300ms */
#define IDLE_WAIT_TIMEOUT	(50 * 1000) /* 50ms */
#define CEIL(x)			((x-(u32)(x) > 0 ? (u32)(x+1) : (u32)(x)))
#define DSC_INIT_XMIT_DELAY	0x200

#define EINT_PEND(x)		((x == 0) ? 2 : ((x == 1) ? 4 : 1))

#define MAX_DSC_SLICE_CNT	4

void dpu_debug_printk(const char *function_name, const char *format, ...);

#define decon_err(fmt, ...)							\
	do {									\
		if (decon_log_level >= 3) {					\
			pr_err(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
			exynos_ss_printk(fmt, ##__VA_ARGS__);			\
		}								\
	} while (0)

#define decon_warn(fmt, ...)							\
	do {									\
		if (decon_log_level >= 4) {					\
			pr_warn(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
			exynos_ss_printk(fmt, ##__VA_ARGS__);			\
		}								\
	} while (0)

#define decon_info(fmt, ...)							\
	do {									\
		if (decon_log_level >= 6)					\
			pr_info(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
	} while (0)

#define decon_dbg(fmt, ...)							\
	do {									\
		if (decon_log_level >= 7)					\
			pr_info(pr_fmt("decon: "fmt), ##__VA_ARGS__);			\
	} while (0)

#define DPU_DEBUG_WIN(fmt, args...)						\
	do {									\
		if (win_update_log_level >= 7)					\
			dpu_debug_printk("WIN_UPDATE", fmt,  ##args);		\
	} while (0)

#define DPU_DEBUG_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 8)					\
			dpu_debug_printk("BTS", fmt,  ##args);			\
	} while (0)

#define DPU_LOG_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 7)					\
			dpu_debug_printk("BTS", fmt,  ##args);			\
	} while (0)

#define DPU_INFO_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 6)					\
			dpu_debug_printk("BTS", fmt,  ##args);			\
	} while (0)

#define DPU_ERR_BTS(fmt, args...)						\
	do {									\
		if (dpu_bts_log_level >= 3)					\
			dpu_debug_printk("BTS", fmt, ##args);			\
	} while (0)

enum decon_trig_mode {
	DECON_HW_TRIG = 0,
	DECON_SW_TRIG
};

enum decon_out_type {
	DECON_OUT_DSI = 0,
	DECON_OUT_EDP,
	DECON_OUT_DP,
	DECON_OUT_WB
};

enum decon_dsi_mode {
	DSI_MODE_SINGLE = 0,
	DSI_MODE_DUAL_DSI,
	DSI_MODE_DUAL_DISPLAY,
	DSI_MODE_NONE
};

enum decon_hold_scheme {
	/*  should be set to this value in case of DSIM video mode */
	DECON_VCLK_HOLD_ONLY		= 0x00,
	/*  should be set to this value in case of DSIM command mode */
	DECON_VCLK_RUNNING_VDEN_DISABLE = 0x01,
	DECON_VCLK_HOLD_VDEN_DISABLE	= 0x02,
	/*  should be set to this value in case of HDMI, eDP */
	DECON_VCLK_NOT_AFFECTED		= 0x03,
};

enum decon_rgb_order {
	DECON_RGB = 0x0,
	DECON_GBR = 0x1,
	DECON_BRG = 0x2,
	DECON_BGR = 0x4,
	DECON_RBG = 0x5,
	DECON_GRB = 0x6,
};

enum decon_win_func {
	PD_FUNC_CLEAR			= 0x0,
	PD_FUNC_COPY			= 0x1,
	PD_FUNC_DESTINATION		= 0x2,
	PD_FUNC_SOURCE_OVER		= 0x3,
	PD_FUNC_DESTINATION_OVER	= 0x4,
	PD_FUNC_SOURCE_IN		= 0x5,
	PD_FUNC_DESTINATION_IN		= 0x6,
	PD_FUNC_SOURCE_OUT		= 0x7,
	PD_FUNC_DESTINATION_OUT		= 0x8,
	PD_FUNC_SOURCE_A_TOP		= 0x9,
	PD_FUNC_DESTINATION_A_TOP	= 0xa,
	PD_FUNC_XOR			= 0xb,
	PD_FUNC_PLUS			= 0xc,
	PD_FUNC_USER_DEFINED		= 0xd,
};

enum decon_win_alpha_coef {
	BND_COEF_ZERO			= 0x0,
	BND_COEF_ONE			= 0x1,
	BND_COEF_AF			= 0x2,
	BND_COEF_1_M_AF		= 0x3,
	BND_COEF_AB			= 0x4,
	BND_COEF_1_M_AB		= 0x5,
	BND_COEF_PLNAE_ALPHA0		= 0x6,
	BND_COEF_1_M_PLNAE_ALPHA0	= 0x7,
	BND_COEF_PLNAE_ALPHA1		= 0x8,
	BND_COEF_1_M_PLNAE_ALPHA1	= 0x9,
	BND_COEF_ALPHA_MULT		= 0xA,
	BND_COEF_1_M_ALPHA_MULT	= 0xB,
};

enum decon_win_alpha_sel {
	ALPHA_MULT_SRC_SEL_ALPHA0 = 0,
	ALPHA_MULT_SRC_SEL_ALPHA1 = 1,
	ALPHA_MULT_SRC_SEL_AF = 2,
	ALPHA_MULT_SRC_SEL_AB = 3,
};

enum decon_fifo_mode {
	DECON_FIFO_00K = 0,
	DECON_FIFO_04K,
	DECON_FIFO_08K,
	DECON_FIFO_16K,
};

enum decon_merger_mode {
	DECON_LRM_NO		= 0x0,
	DECON_LRM_NOSWAP_RF	= 0x4,
	DECON_LRM_NOSWAP_LF	= 0x5,
	DECON_LRM_SWAP_RF	= 0x6,
	DECON_LRM_SWAP_LF	= 0x7,
};

enum decon_te_src {
	DECON_TE_FROM_DDI0 = 0,
	DECON_TE_FROM_DDI1,
	DECON_TE_FROM_DDI2,
	DECON_TE_FROM_USB,
};

enum decon_set_trig {
	DECON_TRIG_DISABLE = 0,
	DECON_TRIG_ENABLE
};

enum decon_idma_type {
	IDMA_G0 = 0,	/* Dedicated to WIN3 */
	IDMA_G1,
	IDMA_VG0,
	IDMA_GF,
	/* this will be removed later */
	/* start */
	IDMA_VG1,
	IDMA_VGF0,
	IDMA_VGF1,
	ODMA_WB,
	/* end */
	IDMA_G0_S,
#if defined(CONFIG_DPU_20)
	IDMA_VGS0,
#endif
};

/*
 * DECON_STATE_ON : disp power on, decon/dsim clock on & lcd on
 * DECON_HIBER : disp power off, decon/dsim clock off & lcd on
 * DECON_STATE_OFF : disp power off, decon/dsim clock off & lcd off
 */
enum decon_state {
	DECON_STATE_INIT = 0,
	DECON_STATE_ON,
	DECON_STATE_HIBER,
	DECON_STATE_OFF,
	DECON_STATE_TUI,
};

/* To find a proper CLOCK ratio */
enum decon_clk_id {
	CLK_ID_VCLK = 0,
	CLK_ID_ECLK,
	CLK_ID_ACLK,
	CLK_ID_PCLK,
	CLK_ID_DPLL, /* DPU_PLL */
	CLK_ID_RESOLUTION,
	CLK_ID_MIC_RATIO,
	CLK_ID_DSC_RATIO,
	CLK_ID_MAX,
};

enum decon_path_cfg {
	PATH_CON_ID_DSIM_IF0 = 0,
	PATH_CON_ID_DSIM_IF1 = 1,
	PATH_CON_ID_DUAL_DSC = 4,
	PATH_CON_ID_DSCC_EN = 7,
};

enum decon_data_path {
	/* No comp - FF0 - FORMATTER0 - DSIM_IF0 */
	DPATH_NOCOMP_FF0_FORMATTER0_DSIMIF0			= 0x001,
	/* No comp - FF0 - FORMATTER1 - DSIM_IF1 */
	DPATH_NOCOMP_FF0_FORMATTER1_DSIMIF1			= 0x002,
	/* No comp - SPLITTER - FF0/1 - FORMATTER0/1 - DSIM_IF0/1 */
	DPATH_NOCOMP_SPLITTER_FF0FF1_FORMATTER01_DSIMIF01	= 0x003,

	/* DSC_ENC0 - FF0 - FORMATTER0 - DSIM_IF0 */
	DPATH_DSCENC0_FF0_FORMATTER0_DSIMIF0		= 0x011,
	/* DSC_ENC0 - FF0 - FORMATTER1 - DSIM_IF1 */
	DPATH_DSCENC0_FF0_FORMATTER1_DSIMIF1		= 0x012,

	/* DSCC,DSC_ENC0 - SPLITTER - FF0/1 - FORMATTER0/1 - DSIM_IF0/1 */
	DPATH_DSCC_DSCENC0_SPLITTER_FF01_FORMATTER01_DSIMIF01	= 0x093,

	/* DSCC,DSC_ENC0/1 - FF0/1 - FORMATTER0 - DSIM_IF0 */
	DPATH_DSCC_DSCENC01_FF01_FORMATTER0_DSIMIF0	= 0x0B1,
	/* DSCC,DSC_ENC0/1 - FF0/1 - FORMATTER1 - DSIM_IF1 */
	DPATH_DSCC_DSCENC01_FF01_FORMATTER1_DSIMIF1	= 0x0B2,
	/* DSCC,DSC_ENC0/1 - FF0/1 - FORMATTER0/1 - DSIM_IF0/1 */
	DPATH_DSCC_DSCENC01_FF01_FORMATTER01_DSIMIF01	= 0x0B3,
	/* WB_PRE */
	DPATH_WBPRE_ONLY					= 0x100,
};

enum decon1_data_path {
	/* No comp - FF0 - FORMATTER0 - DSIM_IF0 */
	DECON1_NOCOMP_FF0_FORMATTER0_DSIMIF0	= 0x001,
	/* No comp - FF0 - FORMATTER0 - WB_POST */
	DECON1_NOCOMP_FF0_FORMATTER0_WBPOST		= 0x004,
	/* DSC_ENC1 - FF0 - FORMATTER0  - DSIM_IF0 */
	DECON1_DSCENC1_FF0_FORMATTER0_DSIMIF0	= 0x011,
	/* DSC_ENC1 - FF0 - FORMATTER0  - WB_POST */
	DECON1_DSCENC1_FF0_FORMATTER0_WBPOST	= 0x014,
	/* WB_PRE */
	DECON1_WBPRE_ONLY				= 0x100,
};

enum decon2_data_path {
	/* No comp - FF0 - FORMATTER0  - WB_POST */
	DECON2_NOCOMP_FF0_FORMATTER0_WBPOST		= 0x004,
	/* No comp - FF0 - FORMATTER0  - DISPIF */
	DECON2_NOCOMP_FF0_FORMATTER0_DISPIF		= 0x008,
	/* WB_PRE */
	DECON2_WBPRE_ONLY				= 0x100,
};

enum decon_dsc_id {
	DECON_DSC_ENC0 = 0x0,
	DECON_DSC_ENC1 = 0x1,
};

enum decon_s_data_path {
	/*  BLENDER-OUT -> No Comp -> u0_FF -> u0_DISPIF */
	DECONS_DATAPATH_NOCOMP_U0FF_DISP0			= 0x01,
	/*  BLENDER-OUT -> No Comp -> u0_FF -> POST_WB */
	DECONS_DATAPATH_NOCOMP_U0FF_POSTWB			= 0x04,
	/*  BLENDER-OUT -> ENC1 -> u0_FF --> u0_DISPIF */
	DECONS_DATAPATH_ENC0_U0FF_DISP0				= 0x21,
	/*  BLENDER-OUT -> ENC1 -> u0_FF -> POST_WB */
	DECONS_DATAPATH_ENC0_U0FF_POSTWB			= 0x24,
	/*  u0_DISPIF (mapcolor mode) */
	DECONS_DATAPATH_DISP0_COLORMAP				= 0x41,
	/*  PRE_WB & BLENDER-OUT -> No Comp -> u0_FF -> u0_DISPIF */
	DECONS_DATAPATH_PREWB_NOCOMP_U0FF_DISP0			= 0x81,
	/*  PRE_WB & BLENDER-OUT -> ENC1 -> u0_FF -> u0_DISPIF */
	DECONS_DATAPATH_PREWB_ENC0_U0FF_DISP0			= 0xA1,
	/*  PRE_WB only */
	DECONS_DATAPATH_PREWB_ONLY				= 0xC0,
};

enum decon_enhance_path {
	ENHANCEPATH_ENHANCE_ALL_OFF	= 0x0,
	ENHANCEPATH_DITHER_ON		= 0x1,
	ENHANCEPATH_DPU_ON		= 0x2,
	ENHANCEPATH_DPU_DITHER_ON	= 0x3,
	ENHANCEPATH_MDNIE_ON		= 0x4,
	ENHANCEPATH_MDNIE_DITHER_ON	= 0x5,
};

enum decon_share_path {
	SHAREPATH_DQE_USE		= 0x0,
	SHAREPATH_VG0_USE		= 0x1,
	SHAREPATH_VG1_USE		= 0x2,
	SHAREPATH_VGF1_USE		= 0x3,
	SHAREPATH_VGF0_USE		= 0x4,
};

#if defined(CONFIG_DPU_20)
enum decon_pixel_format {
	DECON_PIXEL_FORMAT_ARGB_8888 = 0,
	DECON_PIXEL_FORMAT_ABGR_8888,
	DECON_PIXEL_FORMAT_RGBA_8888,
	DECON_PIXEL_FORMAT_BGRA_8888,
	DECON_PIXEL_FORMAT_XRGB_8888,
	DECON_PIXEL_FORMAT_XBGR_8888,
	DECON_PIXEL_FORMAT_RGBX_8888,
	DECON_PIXEL_FORMAT_BGRX_8888,
	DECON_PIXEL_FORMAT_RGBA_5551,
	DECON_PIXEL_FORMAT_BGRA_5551,
	DECON_PIXEL_FORMAT_ABGR_4444,
	DECON_PIXEL_FORMAT_RGBA_4444,
	DECON_PIXEL_FORMAT_BGRA_4444,
	DECON_PIXEL_FORMAT_RGB_565,
	DECON_PIXEL_FORMAT_BGR_565,
	DECON_PIXEL_FORMAT_ARGB_2101010,
	DECON_PIXEL_FORMAT_ABGR_2101010,
	DECON_PIXEL_FORMAT_RGBA_1010102,
	DECON_PIXEL_FORMAT_BGRA_1010102,
	DECON_PIXEL_FORMAT_NV16,
	DECON_PIXEL_FORMAT_NV61,
	DECON_PIXEL_FORMAT_YVU422_3P,
	DECON_PIXEL_FORMAT_NV12,
	DECON_PIXEL_FORMAT_NV21,
	DECON_PIXEL_FORMAT_NV12M,
	DECON_PIXEL_FORMAT_NV21M,
	DECON_PIXEL_FORMAT_YUV420,
	DECON_PIXEL_FORMAT_YVU420,
	DECON_PIXEL_FORMAT_YUV420M,
	DECON_PIXEL_FORMAT_YVU420M,
	DECON_PIXEL_FORMAT_NV12N,
	DECON_PIXEL_FORMAT_NV12N_10B,
	DECON_PIXEL_FORMAT_NV12M_P010,
	DECON_PIXEL_FORMAT_NV21M_P010,
	DECON_PIXEL_FORMAT_NV12M_S10B,
	DECON_PIXEL_FORMAT_NV21M_S10B,
	DECON_PIXEL_FORMAT_NV16M_P210,
	DECON_PIXEL_FORMAT_NV61M_P210,
	DECON_PIXEL_FORMAT_NV16M_S10B,
	DECON_PIXEL_FORMAT_NV61M_S10B,
	DECON_PIXEL_FORMAT_NV12_P010,
	DECON_PIXEL_FORMAT_MAX,
};
#else
enum decon_pixel_format {
	DECON_PIXEL_FORMAT_ARGB_8888 = 0,
	DECON_PIXEL_FORMAT_ABGR_8888,
	DECON_PIXEL_FORMAT_RGBA_8888,
	DECON_PIXEL_FORMAT_BGRA_8888,
	DECON_PIXEL_FORMAT_XRGB_8888,
	DECON_PIXEL_FORMAT_XBGR_8888,
	DECON_PIXEL_FORMAT_RGBX_8888,
	DECON_PIXEL_FORMAT_BGRX_8888,

	DECON_PIXEL_FORMAT_RGBA_5551,
	DECON_PIXEL_FORMAT_RGB_565,
	DECON_PIXEL_FORMAT_NV16,
	DECON_PIXEL_FORMAT_NV61,

	DECON_PIXEL_FORMAT_YVU422_3P,

	DECON_PIXEL_FORMAT_NV12,
	DECON_PIXEL_FORMAT_NV21,
	DECON_PIXEL_FORMAT_NV12M,
	DECON_PIXEL_FORMAT_NV21M,

	DECON_PIXEL_FORMAT_YUV420,
	DECON_PIXEL_FORMAT_YVU420,
	DECON_PIXEL_FORMAT_YUV420M,
	DECON_PIXEL_FORMAT_YVU420M,
	DECON_PIXEL_FORMAT_NV12N,
	DECON_PIXEL_FORMAT_MAX,
};
#endif

enum decon_blending {
	DECON_BLENDING_NONE = 0,
	DECON_BLENDING_PREMULT = 1,
	DECON_BLENDING_COVERAGE = 2,
	DECON_BLENDING_MAX = 3,
};

#if defined(CONFIG_DPU_20)
enum dpp_flip {
	DPP_FLIP_NONE = 0x0,
	DPP_FLIP_X,
	DPP_FLIP_Y,
	DPP_FLIP_XY,
	DPP_ROT_90,
	DPP_ROT_90_XFLIP,
	DPP_ROT_90_YFLIP,
	DPP_ROT_270,
};
#else
enum dpp_flip {
	DPP_FLIP_NONE = 0x0,
	DPP_FLIP_X,
	DPP_FLIP_Y,
	DPP_FLIP_XY,
};
#endif

#if defined(CONFIG_DPU_20)
enum dpp_csc_eq {
	/* eq_mode : 6bits [5:0] */
	CSC_STANDARD_SHIFT = 0,
	CSC_BT_601 = 0,
	CSC_BT_709 = 1,
	CSC_BT_2020 = 2,
	CSC_DCI_P3 = 3,
	CSC_BT_601_625,
	CSC_BT_601_625_UNADJUSTED,
	CSC_BT_601_525,
	CSC_BT_601_525_UNADJUSTED,
	CSC_BT_2020_CONSTANT_LUMINANCE,
	CSC_BT_470M,
	CSC_FILM,
	CSC_ADOBE_RGB,
	CSC_UNSPECIFIED = 63,
	/* eq_mode : 3bits [8:6] */
	CSC_RANGE_SHIFT = 6,
	CSC_RANGE_LIMITED = 0x0,
	CSC_RANGE_FULL = 0x1,
	CSC_RANGE_EXTENDED,
	CSC_RANGE_UNSPECIFIED = 7
};
#else
enum dpp_csc_eq {
	/* eq_mode : 6bits [5:0] */
	CSC_STANDARD_SHIFT = 0,
	CSC_BT_601 = 0,
	CSC_BT_709 = 1,
	CSC_BT_2020 = 2,
	CSC_DCI_P3 = 3,
	/* eq_mode : 3bits [8:6] */
	CSC_RANGE_SHIFT = 6,
	CSC_RANGE_LIMITED = 0x0,
	CSC_RANGE_FULL = 0x1,
};
#endif

enum dpp_comp_src {
	DPP_COMP_SRC_NONE = 0,
	DPP_COMP_SRC_G2D,
	DPP_COMP_SRC_GPU
};

enum dpu_frm_status {
	DPU_FRM_DECON_TIMEOUT = (1 << 0),
	DPU_FRM_SYSMMU_FAULT = (1 << 8),
	DPU_FRM_UNDERRUN = (1 << 16),
	DPU_FRM_RX_ECC = (1 << 24),
	DPU_FRM_TE_TIMEOUT = (1 << 28),
	DPU_FRM_NO_TE = (1 << 29),
	DPU_FRM_SW_RECOVERY = (1 << 31),
};

struct decon_clocks {
	unsigned long decon[CLK_ID_DPLL + 1];
};

struct decon_mode_info {
	enum decon_psr_mode psr_mode;
	enum decon_trig_mode trig_mode;
	enum decon_out_type out_type;
	enum decon_dsi_mode dsi_mode;
};

struct decon_param {
	struct decon_mode_info psr;
	struct decon_lcd *lcd_info;
	u32 nr_windows;
	void __iomem *disp_ss_regs;
};

struct decon_window_regs {
	u32 wincon;
	u32 start_pos;
	u32 end_pos;
	u32 colormap;
	u32 start_time;
	u32 pixel_count;
	u32 whole_w;
	u32 whole_h;
	u32 offset_x;
	u32 offset_y;
	u32 winmap_state;
	enum decon_idma_type type;
	int plane_alpha;
	enum decon_pixel_format format;
	enum decon_blending blend;
};

struct decon_dma_buf_data {
	struct ion_handle		*ion_handle;
	struct dma_buf			*dma_buf;
	struct dma_buf_attachment	*attachment;
	struct sg_table			*sg_table;
	dma_addr_t			dma_addr;
	struct sync_fence		*fence;
};

struct decon_win_rect {
	int x;
	int y;
	u32 w;
	u32 h;
};

struct decon_rect {
	u32 left;
	u32 top;
	u32 right;
	u32 bottom;
};

#if defined(CONFIG_DPU_20)
enum dpp_hdr_standard {
	DPP_HDR_OFF = 0,
	DPP_HDR_ST2084,
	DPP_HDR_HLG,
	DPP_TRANSFER_LINEAR,
	DPP_TRANSFER_SRGB,
	DPP_TRANSFER_SMPTE_170M,
	DPP_TRANSFER_GAMMA2_2,
	DPP_TRANSFER_GAMMA2_6,
	DPP_TRANSFER_GAMMA2_8
};
#endif
struct dpp_params {
	dma_addr_t addr[MAX_PLANE_CNT];
	enum dpp_flip flip;
	enum dpp_csc_eq eq_mode;
	enum dpp_comp_src comp_src;
#if defined(CONFIG_DPU_20)
	enum dpp_hdr_standard hdr_std;
	u32 min_luminance;
	u32 max_luminance;
#endif
};

struct decon_frame {
	int x;
	int y;
	u32 w;
	u32 h;
	u32 f_w;
	u32 f_h;
};

struct decon_win_config {
	enum {
		DECON_WIN_STATE_DISABLED = 0,
		DECON_WIN_STATE_COLOR,
		DECON_WIN_STATE_BUFFER,
		DECON_WIN_STATE_UPDATE,
	} state;

	/* Reusability:This struct is used for IDMA and ODMA */
	union {
		__u32 color;
		struct {
			int				fd_idma[3];
			int				fence_fd;

#if defined(CONFIG_DPU_20)
			int				rel_fence;
#endif
			int				plane_alpha;
			enum decon_blending		blending;
			enum decon_idma_type		idma_type;
			enum decon_pixel_format		format;
			struct dpp_params		dpp_parm;
			/* no read area of IDMA */
			struct decon_win_rect		block_area;
			struct decon_win_rect		transparent_area;
			struct decon_win_rect		opaque_area;
			/* source framebuffer coordinates */
			struct decon_frame		src;
		};
	};

	/* destination OSD coordinates */
	struct decon_frame dst;
	bool protection;
	bool compression;
};

struct decon_reg_data {
	u32 num_of_window;
	int plane_cnt[MAX_DECON_WIN + 1];
	struct list_head list;
	struct decon_rect blender_bg;
	struct decon_win_config dpp_config[MAX_DECON_WIN + 1];
	struct decon_win_rect block_rect[MAX_DECON_WIN];
	struct decon_window_regs win_regs[MAX_DECON_WIN];
	struct decon_dma_buf_data dma_buf_data[MAX_DECON_WIN + 1][MAX_PLANE_CNT];

	/*
	 * If window update size is changed, that size has to be applied to
	 * DECON, DSIM and panel in case of below
	 * - full size -> partial size
	 * - partial size -> different partial size
	 * - partial size -> full size
	 *
	 * need_update flag indicates whether changes are applied to hw or not
	 */
	bool need_update;
	/* current update region */
	struct decon_rect up_region;
	/* protected contents playback */
	bool protection[MAX_DECON_WIN];
	/* release fence*/
	struct sync_pt *pt;
};

struct decon_win_config_data {
	int	fence;
	int	fd_odma;
	struct decon_win_config config[MAX_DECON_WIN + 1];
};

#if defined(CONFIG_DPU_20)
enum hwc_ver {
	HWC_INIT = 0,
	HWC_1_0 = 1,
	HWC_2_0 = 2,
};

struct decon_disp_info {
	enum hwc_ver ver;
	enum decon_psr_mode psr_mode;
	struct lcd_mres_info mres_info;
	u32 chip_ver;
	unsigned char reverved[128];
};
#endif

struct dpu_size_info {
	u32 w_in;
	u32 h_in;
	u32 w_out;
	u32 h_out;
};

#ifdef CONFIG_DECON_EVENT_LOG
/**
 * Display Subsystem event management status.
 *
 * These status labels are used internally by the DECON to indicate the
 * current status of a device with operations.
 */
typedef enum dpu_event_type {
	/* Related with FB interface */
	DPU_EVT_BLANK = 0,
	DPU_EVT_UNBLANK,
	DPU_EVT_ACT_VSYNC,
	DPU_EVT_DEACT_VSYNC,
	DPU_EVT_WIN_CONFIG,
#if defined(CONFIG_DPU_20)
	DPU_EVT_DISP_INFO,
#endif

	/* Related with interrupt */
	DPU_EVT_TE_INTERRUPT,
	DPU_EVT_UNDERRUN,
	DPU_EVT_DECON_FRAMEDONE,
	DPU_EVT_DSIM_FRAMEDONE,
	DPU_EVT_RSC_CONFLICT,
	DPU_EVT_DSIM_PL_FIFO_EMPTY,
	DPU_EVT_DSIM_PH_FIFO_EMPTY,
	DPU_EVT_DSIM_VT_STATUS,

	/* Related with async event */
	DPU_EVT_UPDATE_HANDLER,
	DPU_EVT_DSIM_COMMAND,
	DPU_EVT_TRIG_MASK,
	DPU_EVT_TRIG_UNMASK,
	DPU_EVT_DECON_FRAMEDONE_WAIT,
	DPU_EVT_DECON_SHUTDOWN,
	DPU_EVT_DSIM_SHUTDOWN,
	DPU_EVT_DECON_FRAMESTART,
	DPU_EVT_ACQUIRE_FENCE,
	DPU_EVT_RELEASE_FENCE,

	/* Related with DPP */
	DPU_EVT_DPP_WINCON,
	DPU_EVT_DPP_FRAMEDONE,
	DPU_EVT_DPP_STOP,
	DPU_EVT_DPP_UPDATE_DONE,
	DPU_EVT_DPP_SHADOW_UPDATE,
	DPU_EVT_DPP_SUSPEND,
	DPU_EVT_DPP_RESUME,

	/* Related with PM */
	DPU_EVT_DECON_SUSPEND,
	DPU_EVT_DECON_RESUME,
	DPU_EVT_DECON_INSTANT_OFF,
	DPU_EVT_ENTER_HIBER,
	DPU_EVT_EXIT_HIBER,
	DPU_EVT_DSIM_SUSPEND,
	DPU_EVT_DSIM_RESUME,
	DPU_EVT_ENTER_ULPS,
	DPU_EVT_EXIT_ULPS,
	DPU_EVT_LINECNT_ZERO,

	/* write-back events */
	DPU_EVT_WB_SET_BUFFER,
	DPU_EVT_WB_SW_TRIGGER,

	DPU_EVT_DMA_FRAMEDONE,
	DPU_EVT_DMA_RECOVERY,

	DPU_EVT_DECON_SET_BUFFER,

	DPU_EVT_DOZE,
	DPU_EVT_DOZE_SUSPEND,

	DPU_EVT_MAX, /* End of EVENT */
} dpu_event_t;

/* Related with Fence */
#define ACQUIRE_FENCE_LEN 40
#define RELEASE_FENCE_LEN 32
struct disp_log_fence {
	char acquire_fence[MAX_DECON_WIN][ACQUIRE_FENCE_LEN];
	char release_fence[RELEASE_FENCE_LEN];
	u32 timeline_value;
	int timeline_max;
};

/* Related with PM */
struct disp_log_pm {
	u32 pm_status;		/* ACTIVE(1) or SUSPENDED(0) */
	ktime_t elapsed;	/* End time - Start time */
};

/* Related with S3CFB_WIN_CONFIG */
struct decon_update_reg_data {
	struct decon_window_regs 	win_regs[MAX_DECON_WIN];
	struct decon_win_config 	win_config[MAX_DECON_WIN + 1];
	struct decon_win_rect 		win;
};

/* Related with MIPI COMMAND read/write */
#define DPU_CALLSTACK_MAX 4
struct dsim_log_cmd_buf {
	u32 id;
	u8 buf;
	u32 size;
	u32 pl_cnt;
	u32 line_cnt;
	void *caller[DPU_CALLSTACK_MAX];
};

/* Related with DPP */
struct disp_log_dpp {
	u32 id;
	u32 start_cnt;
	u32 done_cnt;
	u32 comp_src;
};

/**
 * struct dpu_log - Display Subsystem Log
 * This struct includes DECON/DSIM/DPP
 */
struct dpu_log {
	ktime_t time;
	dpu_event_t type;
	union {
		struct disp_log_dpp dpp;
		struct decon_update_reg_data reg;
		struct dsim_log_cmd_buf cmd_buf;
		struct disp_log_pm pm;
		struct disp_log_fence fence;
	} data;
};

struct dpu_size_err_info {
	ktime_t time;
	struct dpu_size_info info;
};

/* Definitions below are used in the DECON */
#define	DPU_EVENT_LOG_MAX	SZ_1K
#define	DPU_EVENT_PRINT_MAX	512
#define	DPU_EVENT_SIZE_ERR_MAX	16
typedef enum dpu_event_log_level_type {
	DPU_EVENT_LEVEL_LOW = 0,
	DPU_EVENT_LEVEL_HIGH,
} dpu_log_level_t;

/* APIs below are used in the DECON/DSIM/DPP driver */
#define DPU_EVENT_START() ktime_t start = ktime_get()
void DPU_EVENT_LOG(dpu_event_t type, struct v4l2_subdev *sd, ktime_t time);
void DPU_EVENT_LOG_WINCON(struct v4l2_subdev *sd, struct decon_reg_data *regs);
void DPU_EVENT_LOG_FENCE(struct v4l2_subdev *sd, struct decon_reg_data *regs, dpu_event_t type);
void DPU_EVENT_LOG_CMD(struct v4l2_subdev *sd, u32 cmd_id, unsigned long data, u32 size);
void DPU_EVENT_SHOW(struct seq_file *s, struct decon_device *decon);
void DPU_EVENT_LOG_WIN_CONFIG(struct v4l2_subdev *sd, struct decon_win_config_data *win_data);
void DPU_EVENT_LOG_INSTANT_OFF(struct v4l2_subdev *sd);
int decon_create_debugfs(struct decon_device *decon);
void decon_destroy_debugfs(struct decon_device *decon);
#else /*!*/
#define DPU_EVENT_START(...) do { } while(0)
#define DPU_EVENT_LOG(...) do { } while(0)
#define DPU_EVENT_LOG_WINCON(...) do { } while(0)
#define DPU_EVENT_LOG_FENCE(...) do { } while (0)
#define DPU_EVENT_LOG_CMD(...) do { } while(0)
#define DPU_EVENT_SHOW(...) do { } while(0)
#define DPU_EVENT_LOG_WIN_CONFIG(...) do { } while(0)
#define DPU_EVENT_LOG_INSTANT_OFF(...) do { } while (0)
#define decon_create_debugfs(...) do { } while(0)
#define decon_destroy_debugfs(..) do { } while(0)
#endif

#if defined(CONFIG_DPU_20)
/* HDR information of panel */
enum decon_hdr_type {
	HDR_NONE = 0,
	HDR_DOLBY_VISION = 1,
	HDR_HDR10 = 2,
	HDR_HLG = 3,
};

struct decon_hdr_capabilities {
	unsigned int out_types[HDR_CAPA_NUM];
};

struct decon_hdr_capabilities_info {
	int out_num;
	int max_luminance;
	int max_average_luminance;
	int min_luminance;
};
#endif
struct decon_resources {
	int irq;
	void __iomem *regs;
	void __iomem *ss_regs;
	struct clk *aclk;
	struct clk *dpll;
	struct clk *pclk;
	struct clk *eclk;
	struct clk *eclk_leaf;
	struct clk *vclk;
	struct clk *vclk_leaf;
	struct clk *busp;
	struct clk *busd;
	struct pinctrl *pinctrl;
	struct pinctrl_state *hw_te_on;
	struct pinctrl_state *hw_te_off;
};

struct decon_dt_info {
	enum decon_psr_mode psr_mode;
	enum decon_trig_mode trig_mode;
	enum decon_dsi_mode dsi_mode;
	enum decon_out_type out_type;
	int out_idx[MAX_DSIM_CNT];
	int max_win;
	int dft_win;
	int dft_idma;
	u32 disp_freq;
	u32 mif_freq;
};

struct decon_win {
	struct decon_device *decon;
	struct fb_info *fbinfo;

	struct fb_videomode videomode;
	struct decon_dma_buf_data dma_buf_data[MAX_PLANE_CNT];
	int plane_cnt;

	int idx;
	int dpp_id;
	u32 pseudo_palette[16];
};

struct decon_debug {
	void __iomem *eint_pend;
	struct dentry *debug_root;
#ifdef CONFIG_DECON_EVENT_LOG
	struct dentry *debug_event;
	struct dentry *debug_dump;
	struct dentry *debug_bts;
	struct dentry *debug_win;
	struct dentry *debug_afbc_rcv;
	struct dpu_log event_log[DPU_EVENT_LOG_MAX];
	atomic_t event_log_idx;
	dpu_log_level_t event_log_level;
#endif
};

struct decon_update_regs {
	struct mutex lock;
	struct list_head list;
	struct task_struct *thread;
	struct kthread_worker worker;
	struct kthread_work work;
};

struct decon_vsync {
	wait_queue_head_t wait;
	ktime_t timestamp;
	bool active;
	int irq_refcount;
	struct mutex lock;
	struct task_struct *thread;
};

struct decon_hiber {
	struct mutex lock;
	struct work_struct work;
	struct workqueue_struct *wq;
	atomic_t trig_cnt;
	atomic_t block_cnt;
	bool init_status;
	void __iomem *cam_status;
};

struct decon_win_update {
	bool enabled;
	u32 rect_w;
	u32 rect_h;
	u32 hori_cnt;
	u32 verti_cnt;
	/* previous update region */
	struct decon_rect prev_up_region;
};

struct decon_bts_ops {
	void (*bts_init)(struct decon_device *decon);
	void (*bts_calc_bw)(struct decon_device *decon, struct decon_reg_data *regs);
	void (*bts_update_bw)(struct decon_device *decon, struct decon_reg_data *regs,
			u32 is_after);
	void (*bts_release_bw)(struct decon_device *decon);
	void (*bts_update_qos_mif)(struct decon_device *decon, u32 mif_freq);
	void (*bts_update_qos_int)(struct decon_device *decon, u32 int_freq);
	void (*bts_update_qos_disp)(struct decon_device *decon, u32 disp_freq);
	void (*bts_deinit)(struct decon_device *decon);
};

struct decon_bts {
	u32 resol_clk;
	u32 bw[BTS_DPP_MAX];
	/* each decon must know other decon's BW to get overall BW */
	u32 ch_bw[3][BTS_DPU_MAX];
	u32 peak;
	u32 total_bw;
	u32 prev_total_bw;
	u32 max_disp_freq;
	u32 prev_max_disp_freq;
	enum bts_bw_type type;
	struct decon_bts_ops *ops;
	struct bts_decon_info bts_info;
	struct pm_qos_request mif_qos;
	struct pm_qos_request int_qos;
	struct pm_qos_request disp_qos;
	u32 disp_freq_minlock;
};

#define ABD_EVENT_LOG_MAX	50
#define ABD_LOG_MAX		10

struct abd_event_log {
	u64 stamp;
	const char *print;
};

struct abd_event {
	struct abd_event_log log[ABD_EVENT_LOG_MAX];
	atomic_t log_idx;
};

struct abd_log {
	u64 stamp;

	unsigned int level;
	unsigned int state;
	unsigned int onoff;

	unsigned int winid;
	struct sync_fence fence;

	unsigned int frm_status;
	unsigned long mif;
	unsigned long iint;
	unsigned long disp;
	struct decon_bts bts;
};

struct abd_trace {
	const char *name;
	unsigned int count;
	unsigned int lcdon_flag;
	struct abd_log log[ABD_LOG_MAX];
};

struct abd_pin {
	const char *name;
	unsigned int irq;
	struct irq_desc *desc;
	int gpio;
	int level;
	int active_level;

	struct abd_trace p_first;
	struct abd_trace p_lcdon;
	struct abd_trace p_event;

	irq_handler_t	handler;
	void		*dev_id;
};

enum {
	ABD_PIN_PCD,
	ABD_PIN_DET,
	ABD_PIN_ERR,
	ABD_PIN_CON,
	ABD_PIN_MAX
};

struct abd_protect {
	struct abd_pin pin[ABD_PIN_MAX];
	struct abd_event event;

	struct abd_trace f_first;
	struct abd_trace f_lcdon;
	struct abd_trace f_event;

	struct abd_trace u_first;
	struct abd_trace u_lcdon;
	struct abd_trace u_event;

	unsigned int irq_enable;
	struct notifier_block reboot_notifier;
	spinlock_t slock;

	struct workqueue_struct *con_workqueue;
	struct work_struct con_work;
	unsigned int con_irq;

	struct notifier_block fb_notifier;
	struct fb_ops decon_fbops;
};

void decon_abd_enable(struct decon_device *decon, int enable);
int decon_abd_register(struct decon_device *decon);
void decon_abd_save_log_fto(struct abd_protect *abd, struct sync_fence *fence);
void decon_abd_save_log_udr(struct abd_protect *abd, unsigned long mif, unsigned long iint, unsigned long disp);
int decon_abd_register_pin_handler(int irq, irq_handler_t handler, void *dev_id);
void decon_abd_save_log_event(struct abd_protect *abd, const char *print);

struct decon_device {
	int id;
	enum decon_state state;

	unsigned int ignore_vsync;
	struct abd_protect abd;
	unsigned int esd_recovery;
	atomic_t win_config;

#if defined(CONFIG_EXYNOS_SUPPORT_DOZE)
	unsigned int doze_state;
#endif

	unsigned long prev_used_dpp;
	unsigned long cur_using_dpp;
	unsigned long dpp_err_stat;

	struct mutex lock;
	struct mutex pm_lock;
	spinlock_t slock;

	struct ion_client *ion_client;

	struct sw_sync_timeline *timeline;
	int timeline_max;

	struct v4l2_subdev *out_sd[MAX_DSIM_CNT];
	struct v4l2_subdev *dsim_sd[MAX_DSIM_CNT];
	struct v4l2_subdev *dpp_sd[MAX_DPP_SUBDEV];
	struct v4l2_subdev *displayport_sd;
	struct v4l2_device v4l2_dev;
	struct v4l2_subdev sd;

	struct device *dev;
	struct decon_dt_info dt;
	struct decon_win *win[MAX_DECON_WIN];
	struct decon_resources res;
	struct decon_debug d;
	struct decon_update_regs up;
	struct decon_vsync vsync;
	struct decon_lcd *lcd_info;
	struct decon_win_update win_up;
	struct decon_hiber hiber;
	struct decon_bts bts;

	int frame_cnt;
	int frame_cnt_target;
	wait_queue_head_t wait_vstatus;
	int eint_status;
#ifdef CONFIG_LOGGING_BIGDATA_BUG
	int eint_pend;
#endif

	u32 prev_protection_bitmask;
	unsigned long prev_aclk_khz;
	int version;

	atomic_t is_shutdown;
	int idle_ip_index;
	/*
	 * frm_status (error info)
	 * [0]=decon timeout,  [8]=sysmmu fault,  [16]=underrun,  [24]=rx ecc
	 * [28]=TE signal but timeout(irq), [29]=no TE signal, [31]=SW Recovery
	 */
	u32 frm_status;
#if defined(CONFIG_DPU_20)
	enum hwc_ver ver;
#endif
#ifdef CONFIG_EXYNOS_ITMON
	struct notifier_block itmon_nb;
#endif
#ifdef CONFIG_EXYNOS_PD
	struct exynos_pm_domain *exynos_pd;
#endif
#ifdef CONFIG_EXYNOS_SUPPORT_FB_HANDOVER
	unsigned int reserved_release;
#endif
};

static inline struct decon_device *get_decon_drvdata(u32 id)
{
	return decon_drvdata[id];
}

/* register access subroutines */
static inline u32 decon_read(u32 id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(id);
	return readl(decon->res.regs + reg_id);
}

static inline u32 decon_read_mask(u32 id, u32 reg_id, u32 mask)
{
	u32 val = decon_read(id, reg_id);
	val &= (mask);
	return val;
}

static inline void decon_write(u32 id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(id);
	writel(val, decon->res.regs + reg_id);
}

static inline void decon_write_mask(u32 id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = decon_read(id, reg_id);

	val = (val & mask) | (old & ~mask);
	decon_write(id, reg_id, val);
}

static inline u32 dsc_read(u32 dsc_id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(0);
	u32 dsc_offset = dsc_id ? DSC1_OFFSET : DSC0_OFFSET;

	return readl(decon->res.regs + dsc_offset + reg_id);
}

static inline void dsc_write(u32 dsc_id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(0);
	u32 dsc_offset = dsc_id ? DSC1_OFFSET : DSC0_OFFSET;

	writel(val, decon->res.regs + dsc_offset + reg_id);
}

static inline void dsc_write_mask(u32 dsc_id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = dsc_read(dsc_id, reg_id);

	val = (val & mask) | (old & ~mask);
	dsc_write(dsc_id, reg_id, val);
}

static inline u32 sysreg_read(u32 id, u32 reg_id)
{
	struct decon_device *decon = get_decon_drvdata(id);
	return readl(decon->res.ss_regs + reg_id);
}

static inline u32 sysreg_read_mask(u32 id, u32 reg_id, u32 mask)
{
	u32 val = sysreg_read(id, reg_id);
	val &= (mask);
	return val;
}

static inline void sysreg_write(u32 id, u32 reg_id, u32 val)
{
	struct decon_device *decon = get_decon_drvdata(id);
	writel(val, decon->res.ss_regs + reg_id);
}

static inline void sysreg_write_mask(u32 id, u32 reg_id, u32 val, u32 mask)
{
	u32 old = sysreg_read(id, reg_id);

	val = (val & mask) | (old & ~mask);
	sysreg_write(id, reg_id, val);
}

/* common function API */
bool decon_validate_x_alignment(struct decon_device *decon, int x, u32 w,
		u32 bits_per_pixel);
int decon_wait_for_vsync(struct decon_device *decon, u32 timeout);

/* DECON to DSI interface functions */
int decon_register_irq(struct decon_device *decon);
int decon_get_clocks(struct decon_device *decon);
void decon_set_clocks(struct decon_device *decon);
int decon_get_out_sd(struct decon_device *decon);
int decon_get_pinctrl(struct decon_device *decon);
int decon_register_ext_irq(struct decon_device *decon);
int decon_create_vsync_thread(struct decon_device *decon);
void decon_destroy_vsync_thread(struct decon_device *decon);
int decon_create_psr_info(struct decon_device *decon);
void decon_destroy_psr_info(struct decon_device *decon);

/* DECON to writeback interface functions */
int decon_wb_register_irq(struct decon_device *decon);
int decon_wb_get_clocks(struct decon_device *decon);
void decon_wb_set_clocks(struct decon_device *decon);
int decon_wb_get_out_sd(struct decon_device *decon);

/* DECON to DISPLAYPORT interface functions */
int decon_displayport_register_irq(struct decon_device *decon);
int decon_displayport_create_vsync_thread(struct decon_device *decon);
int decon_displayport_get_clocks(struct decon_device *decon);
void decon_displayport_set_clocks(struct decon_device *decon);
int decon_displayport_get_out_sd(struct decon_device *decon);

/* window update related function */
#define DPU_FULL_RECT(r, lcd)			\
	do {					\
		(r)->left = 0;			\
		(r)->top = 0;			\
		(r)->right = (lcd)->xres - 1;	\
		(r)->bottom = (lcd)->yres - 1;	\
	} while (0)
void dpu_init_win_update(struct decon_device *decon);
void dpu_prepare_win_update_config(struct decon_device *decon,
		struct decon_win_config_data *win_data,
		struct decon_reg_data *regs);
void dpu_set_win_update_config(struct decon_device *decon,
		struct decon_reg_data *regs);
void dpu_set_win_update_partial_size(struct decon_device *decon,
		struct decon_rect *up_region);

/* internal only function API */
int decon_check_var(struct fb_var_screeninfo *var, struct fb_info *info);
int decon_set_par(struct fb_info *info);
int decon_pan_display(struct fb_var_screeninfo *var, struct fb_info *info);
int decon_setcolreg(unsigned regno,
			    unsigned red, unsigned green, unsigned blue,
			    unsigned transp, struct fb_info *info);
int decon_mmap(struct fb_info *info, struct vm_area_struct *vma);

u32 wincon(u32 transp_len, u32 a0, u32 a1, int plane_alpha,
		enum decon_blending blending, int idx);

static inline u32 win_start_pos(int x, int y)
{
	return (WIN_STRPTR_Y_F(y) | WIN_STRPTR_X_F(x));
}

static inline u32 win_end_pos(int x, int y,  u32 xres, u32 yres)
{
	return (WIN_ENDPTR_Y_F(y + yres - 1) | WIN_ENDPTR_X_F(x + xres - 1));
}


/* HIBER releated */
int decon_exit_hiber(struct decon_device *decon);
int decon_enter_hiber(struct decon_device *decon);
int decon_lcd_off(struct decon_device *decon);
int decon_register_hiber_work(struct decon_device *decon);
int decon_hiber_block_exit(struct decon_device *decon);
u32 decon_reg_get_cam_status(void __iomem *cam_status);

static inline void decon_hiber_block(struct decon_device *decon)
{
	if (decon)
		atomic_inc(&decon->hiber.block_cnt);
}

static inline bool decon_is_hiber_blocked(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt) > 0);
}

static inline int decon_get_hiber_block_cnt(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt));
}

static inline void decon_hiber_unblock(struct decon_device *decon)
{
	if (decon) {
		if (decon_is_hiber_blocked(decon))
			atomic_dec(&decon->hiber.block_cnt);
	}
}

static inline void decon_hiber_block_reset(struct decon_device *decon)
{
	if (decon)
		atomic_set(&decon->hiber.block_cnt, 0);
}

static inline void decon_hiber_trig_reset(struct decon_device *decon)
{
	if (decon)
		atomic_set(&decon->hiber.trig_cnt, 0);
}

static inline bool decon_min_lock_cond(struct decon_device *decon)
{
	return (atomic_read(&decon->hiber.block_cnt) <= 0);
}

#ifdef CONFIG_LCD_HMT
static inline bool is_hmd_running(struct decon_device *decon)
{
	struct dsim_device *dsim;
	dsim = container_of(decon->out_sd[0], struct dsim_device, sd);

	if ((dsim != NULL) &&
		(dsim->hmt_on))
		return true;
	else
		return false;
}
#endif

static inline bool is_cam_not_running(struct decon_device *decon)
{
	if (!decon->id)
		return (!(decon_reg_get_cam_status(decon->hiber.cam_status) & 0xF));
	else
		return true;
}
static inline bool decon_hiber_enter_cond(struct decon_device *decon)
{
	return ((atomic_read(&decon->hiber.block_cnt) <= 0)
#ifdef CONFIG_LCD_HMT
		&& (!is_hmd_running(decon))
#endif
		&& is_cam_not_running(decon)
		&& (atomic_inc_return(&decon->hiber.trig_cnt) >
			DECON_ENTER_HIBER_CNT));
}

static inline void decon_enter_shutdown(struct decon_device *decon)
{
	atomic_inc(&decon->is_shutdown);
}

static inline bool decon_is_enter_shutdown(struct decon_device *decon)
{
	return atomic_read(&decon->is_shutdown);
}

static inline void decon_enter_shutdown_reset(struct decon_device *decon)
{
	atomic_set(&decon->is_shutdown, 0);
}

/* CAL APIs list */
int decon_reg_init(u32 id, u32 dsi_idx, struct decon_param *p);
//void decon_reg_init_probe(u32 id, u32 dsi_idx, struct decon_param *p);
int decon_reg_start(u32 id, struct decon_mode_info *psr);
int decon_reg_stop(u32 id, u32 dsi_idx, struct decon_mode_info *psr);
int decon_reg_reset(u32 id);
void decon_reg_direct_on_off(u32 id, u32 en);
int decon_reg_instant_stop(u32 id, unsigned long timeout);
void decon_reg_release_resource(u32 id, struct decon_mode_info *psr);
void decon_reg_set_int(u32 id, struct decon_mode_info *psr, u32 en);
void decon_reg_set_window_control(u32 id, int win_idx,
		struct decon_window_regs *regs, u32 winmap_en);
void decon_reg_update_req_and_unmask(u32 id, struct decon_mode_info *psr);
int decon_reg_wait_update_done_and_mask(u32 id, struct decon_mode_info *psr,
		u32 timeout);
void decon_reg_set_trigger(u32 id, struct decon_mode_info *psr,
		enum decon_set_trig en);
void decon_reg_set_timeout_value(u32 id, u32 to_val);
int decon_reg_wait_for_update_timeout(u32 id, unsigned long timeout);
int decon_reg_get_interrupt_and_clear(u32 id, u32 *ext_irq);
void decon_reg_set_blender_bg_image_size(u32 id, enum decon_dsi_mode dsi_mode,
		struct decon_lcd *lcd_info);
void decon_reg_config_data_path_size(u32 id,
	u32 width, u32 height, u32 overlap_w);
void decon_reg_set_dispif_size(u32 id, u32 width, u32 height);
void decon_reg_get_clock_ratio(struct decon_clocks *clks,
		struct decon_lcd *lcd_info);
void decon_reg_clear_int_all(u32 id);
void decon_reg_all_win_shadow_update_req(u32 id);
void decon_reg_update_req_window(u32 id, u32 win_idx);
void decon_reg_set_partial_update(u32 id, enum decon_dsi_mode dsi_mode,
		struct decon_lcd *lcd_info, bool in_slice[]);
int decon_reg_wait_idle_status_timeout(u32 id, unsigned long timeout);
void decon_reg_set_start_crc(u32 id, u32 en);
void decon_reg_set_select_crc_bits(u32 id, u32 bit_sel);
void decon_reg_get_crc_data(u32 id, u32 *w0_data);
void dpu_sysreg_set_lpmux(void __iomem *sysreg);
void decon_reg_set_win_enable(u32 id, u32 win_idx, u32 en);

/* tui feature support external to security driver(gud) */
int decon_tui_protection(bool tui_en);
int decon_reg_stop_nreset(u32 id, struct decon_mode_info *psr);
void decon_reg_clear_all_update_req(u32 id);
void decon_reg_update_req_global(u32 id);

/* helper functions */
int dpu_get_sd_by_drvname(struct decon_device *decon, char *drvname);
u32 dpu_translate_fmt_to_dpp(u32 format);
u32 dpu_get_bpp(enum decon_pixel_format fmt);
int dpu_get_plane_cnt(enum decon_pixel_format format);
u32 dpu_get_alpha_len(int format);
void dpu_unify_rect(struct decon_rect *r1, struct decon_rect *r2,
		struct decon_rect *dst);
void decon_dump(struct decon_device *decon);
void decon_to_psr_info(struct decon_device *decon, struct decon_mode_info *psr);
void decon_to_init_param(struct decon_device *decon, struct decon_param *p);
void decon_create_timeline(struct decon_device *decon, char *name);
#if defined(CONFIG_DPU_20)
void decon_create_release_fences(struct decon_device *decon,
		struct decon_win_config_data *win_data,
		struct sync_fence *fence);
#endif
int decon_create_fence(struct decon_device *decon,
		struct sync_fence **fence, struct decon_reg_data *regs);
void decon_install_fence(struct sync_fence *fence, int fd);
int decon_wait_fence(struct sync_fence *fence);
void decon_signal_fence(struct decon_device *decon);

bool decon_intersect(struct decon_rect *r1, struct decon_rect *r2);
int decon_intersection(struct decon_rect *r1,
		struct decon_rect *r2, struct decon_rect *r3);

bool is_decon_rect_differ(struct decon_rect *r1, struct decon_rect *r2);
bool is_rgb32(int format);
bool is_scaling(struct decon_win_config *config);
bool is_full(struct decon_rect *r, struct decon_lcd *lcd);
bool is_decon_opaque_format(int format);
void __iomem *dpu_get_sysreg_addr(void);
u32 dpu_dma_type_to_channel(enum decon_idma_type type);
#if defined(CONFIG_EXYNOS_CONTENT_PATH_PROTECTION)
void decon_set_protected_content(struct decon_device *decon,
		struct decon_reg_data *regs);
#endif

int decon_runtime_suspend(struct device *dev);
int decon_runtime_resume(struct device *dev);
void decon_dpp_stop(struct decon_device *decon, bool do_reset);
#if defined(CONFIG_EXYNOS_SUPPORT_DOZE)
int decon_set_doze_mode(struct decon_device *decon, u32 mode);

enum doze_state {
	DOZE_STATE_NORMAL,
	DOZE_STATE_DOZE,
	DOZE_STATE_SUSPEND,
	DOZE_STATE_DOZE_SUSPEND
};

#define IS_DOZE(doze_state)		(doze_state == DOZE_STATE_DOZE || doze_state == DOZE_STATE_DOZE_SUSPEND)
#endif
/* IOCTL commands */
#define S3CFB_SET_VSYNC_INT		_IOW('F', 206, __u32)
#define S3CFB_WIN_CONFIG		_IOW('F', 209, \
						struct decon_win_config_data)
#if defined(CONFIG_DPU_20)
#define EXYNOS_DISP_INFO               _IOW('F', 260, \
						struct decon_disp_info)
#endif
#define S3CFB_START_CRC			_IOW('F', 270, u32)
#define S3CFB_SEL_CRC_BITS		_IOW('F', 271, u32)
#define S3CFB_GET_CRC_DATA		_IOR('F', 272, u32)

#define S3CFB_CLEAR_FRAME_STATUS		_IOR('F', 273, u32)
#define S3CFB_GET_FRAME_STATUS		_IOR('F', 274, u32)

#define EXYNOS_GET_DISPLAYPORT_CONFIG		_IOW('F', 300, \
						struct exynos_displayport_data)
#define EXYNOS_SET_DISPLAYPORT_CONFIG		_IOW('F', 301, \
						struct exynos_displayport_data)
#define S3CFB_POWER_MODE		_IOW('F', 223, __u32)

#if defined(CONFIG_DPU_20)
/* HDR support */
#define S3CFB_GET_HDR_CAPABILITIES _IOW('F', 400, struct decon_hdr_capabilities)
#define S3CFB_GET_HDR_CAPABILITIES_NUM _IOW('F', 401, struct decon_hdr_capabilities_info)
#endif
enum doze_mode {
	DECON_PWR_OFF = 0,
	DECON_PWR_DOZE,
	DECON_PWR_NORMAL,
	DECON_PWR_DOZE_SUSPEND,
	DECON_PWR_MAX,
};
#endif /* ___SAMSUNG_DECON_H__ */
