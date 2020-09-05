/*
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * S5PV210 - Clock register definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_REGS_CLOCK_H
#define __ASM_ARCH_REGS_CLOCK_H __FILE__

#include <plat/map-base.h>

#define S5P_CLKREG(x)		(S3C_VA_SYS + (x))

#define S5P_APLL_LOCK		S5P_CLKREG(0x00)
#define S5P_MPLL_LOCK		S5P_CLKREG(0x08)
#define S5P_EPLL_LOCK		S5P_CLKREG(0x10)
#define S5P_VPLL_LOCK		S5P_CLKREG(0x20)

#define S5P_APLL_CON		S5P_CLKREG(0x100)
#define S5P_MPLL_CON		S5P_CLKREG(0x108)
#define S5P_EPLL_CON		S5P_CLKREG(0x110)
#define S5P_EPLL_CON1		S5P_CLKREG(0x114)
#define S5P_VPLL_CON		S5P_CLKREG(0x120)

#define S5P_CLK_SRC0		S5P_CLKREG(0x200)
#define S5P_CLK_SRC1		S5P_CLKREG(0x204)
#define S5P_CLK_SRC2		S5P_CLKREG(0x208)
#define S5P_CLK_SRC3		S5P_CLKREG(0x20C)
#define S5P_CLK_SRC4		S5P_CLKREG(0x210)
#define S5P_CLK_SRC5		S5P_CLKREG(0x214)
#define S5P_CLK_SRC6		S5P_CLKREG(0x218)

#define S5P_CLK_SRC_MASK0	S5P_CLKREG(0x280)
#define S5P_CLK_SRC_MASK1	S5P_CLKREG(0x284)

#define S5P_CLK_DIV0		S5P_CLKREG(0x300)
#define S5P_CLK_DIV1		S5P_CLKREG(0x304)
#define S5P_CLK_DIV2		S5P_CLKREG(0x308)
#define S5P_CLK_DIV3		S5P_CLKREG(0x30C)
#define S5P_CLK_DIV4		S5P_CLKREG(0x310)
#define S5P_CLK_DIV5		S5P_CLKREG(0x314)
#define S5P_CLK_DIV6		S5P_CLKREG(0x318)
#define S5P_CLK_DIV7		S5P_CLKREG(0x31C)

#define S5P_CLKGATE_MAIN0	S5P_CLKREG(0x400)
#define S5P_CLKGATE_MAIN1	S5P_CLKREG(0x404)
#define S5P_CLKGATE_MAIN2	S5P_CLKREG(0x408)

#define S5P_CLKGATE_PERI0	S5P_CLKREG(0x420)
#define S5P_CLKGATE_PERI1	S5P_CLKREG(0x424)

#define S5P_CLKGATE_SCLK0	S5P_CLKREG(0x440)
#define S5P_CLKGATE_SCLK1	S5P_CLKREG(0x444)
#define S5P_CLKGATE_IP0		S5P_CLKREG(0x460)
#define S5P_CLKGATE_IP1		S5P_CLKREG(0x464)
#define S5P_CLKGATE_IP2		S5P_CLKREG(0x468)
#define S5P_CLKGATE_IP3		S5P_CLKREG(0x46C)
#define S5P_CLKGATE_IP4		S5P_CLKREG(0x470)

#define S5P_CLKGATE_BLOCK	S5P_CLKREG(0x480)
#define S5P_CLKGATE_BUS0	S5P_CLKREG(0x484)
#define S5P_CLKGATE_BUS1	S5P_CLKREG(0x488)
#define S5P_CLK_OUT		S5P_CLKREG(0x500)

/* DIV/MUX STATUS */
#define S5P_CLKDIV_STAT0	S5P_CLKREG(0x1000)
#define S5P_CLKDIV_STAT1	S5P_CLKREG(0x1004)
#define S5P_CLKMUX_STAT0	S5P_CLKREG(0x1100)
#define S5P_CLKMUX_STAT1	S5P_CLKREG(0x1104)

/* CLKSRC0 */
#define S5P_CLKSRC0_MUX200_SHIFT	(16)
#define S5P_CLKSRC0_MUX200_MASK		(0x1 << S5P_CLKSRC0_MUX200_SHIFT)
#define S5P_CLKSRC0_MUX166_MASK		(0x1<<20)
#define S5P_CLKSRC0_MUX133_MASK		(0x1<<24)

/* CLKSRC2 */
#define S5P_CLKSRC2_G3D_SHIFT           (0)
#define S5P_CLKSRC2_G3D_MASK            (0x3 << S5P_CLKSRC2_G3D_SHIFT)
#define S5P_CLKSRC2_MFC_SHIFT           (4)
#define S5P_CLKSRC2_MFC_MASK            (0x3 << S5P_CLKSRC2_MFC_SHIFT)

/* CLKSRC6*/
#define S5P_CLKSRC6_ONEDRAM_SHIFT       (24)
#define S5P_CLKSRC6_ONEDRAM_MASK        (0x3 << S5P_CLKSRC6_ONEDRAM_SHIFT)

/* CLKDIV0 */
#define S5P_CLKDIV0_APLL_SHIFT		(0)
#define S5P_CLKDIV0_APLL_MASK		(0x7 << S5P_CLKDIV0_APLL_SHIFT)
#define S5P_CLKDIV0_A2M_SHIFT		(4)
#define S5P_CLKDIV0_A2M_MASK		(0x7 << S5P_CLKDIV0_A2M_SHIFT)
#define S5P_CLKDIV0_HCLK200_SHIFT	(8)
#define S5P_CLKDIV0_HCLK200_MASK	(0x7 << S5P_CLKDIV0_HCLK200_SHIFT)
#define S5P_CLKDIV0_PCLK100_SHIFT	(12)
#define S5P_CLKDIV0_PCLK100_MASK	(0x7 << S5P_CLKDIV0_PCLK100_SHIFT)
#define S5P_CLKDIV0_HCLK166_SHIFT	(16)
#define S5P_CLKDIV0_HCLK166_MASK	(0xF << S5P_CLKDIV0_HCLK166_SHIFT)
#define S5P_CLKDIV0_PCLK83_SHIFT	(20)
#define S5P_CLKDIV0_PCLK83_MASK		(0x7 << S5P_CLKDIV0_PCLK83_SHIFT)
#define S5P_CLKDIV0_HCLK133_SHIFT	(24)
#define S5P_CLKDIV0_HCLK133_MASK	(0xF << S5P_CLKDIV0_HCLK133_SHIFT)
#define S5P_CLKDIV0_PCLK66_SHIFT	(28)
#define S5P_CLKDIV0_PCLK66_MASK		(0x7 << S5P_CLKDIV0_PCLK66_SHIFT)

/* CLKDIV2 */
#define S5P_CLKDIV2_G3D_SHIFT           (0)
#define S5P_CLKDIV2_G3D_MASK            (0xF << S5P_CLKDIV2_G3D_SHIFT)
#define S5P_CLKDIV2_MFC_SHIFT           (4)
#define S5P_CLKDIV2_MFC_MASK            (0xF << S5P_CLKDIV2_MFC_SHIFT)

/* CLKDIV6 */
#define S5P_CLKDIV6_ONEDRAM_SHIFT       (28)
#define S5P_CLKDIV6_ONEDRAM_MASK        (0xF << S5P_CLKDIV6_ONEDRAM_SHIFT)

#define S5P_SWRESET		S5P_CLKREG(0x2000)

#define S5P_ARM_MCS_CON		S5P_CLKREG(0x6100)

/* Registers related to power management */
#define S5P_PWR_CFG		S5P_CLKREG(0xC000)
#define S5P_EINT_WAKEUP_MASK	S5P_CLKREG(0xC004)
#define S5P_WAKEUP_MASK		S5P_CLKREG(0xC008)
#define S5P_PWR_MODE		S5P_CLKREG(0xC00C)
#define S5P_NORMAL_CFG		S5P_CLKREG(0xC010)
#define S5P_IDLE_CFG		S5P_CLKREG(0xC020)
#define S5P_STOP_CFG		S5P_CLKREG(0xC030)
#define S5P_STOP_MEM_CFG	S5P_CLKREG(0xC034)
#define S5P_SLEEP_CFG		S5P_CLKREG(0xC040)

#define S5P_OSC_FREQ		S5P_CLKREG(0xC100)
#define S5P_OSC_STABLE		S5P_CLKREG(0xC104)
#define S5P_PWR_STABLE		S5P_CLKREG(0xC108)
#define S5P_MTC_STABLE		S5P_CLKREG(0xC110)
#define S5P_CLAMP_STABLE	S5P_CLKREG(0xC114)

#define S5P_WAKEUP_STAT		S5P_CLKREG(0xC200)
#define S5P_BLK_PWR_STAT	S5P_CLKREG(0xC204)

#define S5P_OTHERS		S5P_CLKREG(0xE000)
#define S5P_OM_STAT		S5P_CLKREG(0xE100)
#define S5P_HDMI_PHY_CONTROL	S5P_CLKREG(0xE804)
#define S5P_USB_PHY_CONTROL	S5P_CLKREG(0xE80C)
#define S5P_DAC_PHY_CONTROL	S5P_CLKREG(0xE810)

#define S5P_INFORM0		S5P_CLKREG(0xF000)
#define S5P_INFORM1		S5P_CLKREG(0xF004)
#define S5P_INFORM2		S5P_CLKREG(0xF008)
#define S5P_INFORM3		S5P_CLKREG(0xF00C)
#define S5P_INFORM4		S5P_CLKREG(0xF010)
#define S5P_INFORM5		S5P_CLKREG(0xF014)
#define S5P_INFORM6		S5P_CLKREG(0xF018)
#define S5P_INFORM7		S5P_CLKREG(0xF01C)

#define S5P_RST_STAT		S5P_CLKREG(0xA000)
#define S5P_OSC_CON		S5P_CLKREG(0x8000)
#define S5P_MDNIE_SEL		S5P_CLKREG(0x7008)
#define S5P_MIPI_PHY_CON0	S5P_CLKREG(0x7200)
#define S5P_MIPI_PHY_CON1	S5P_CLKREG(0x7204)

#define S5P_IDLE_CFG_TL_MASK	(3 << 30)
#define S5P_IDLE_CFG_TM_MASK	(3 << 28)
#define S5P_IDLE_CFG_TL_ON	(2 << 30)
#define S5P_IDLE_CFG_TM_ON	(2 << 28)
#define S5P_IDLE_CFG_DIDLE	(1 << 0)

#define S5P_CFG_WFI_CLEAN		(~(3 << 8))
#define S5P_CFG_WFI_IDLE		(1 << 8)
#define S5P_CFG_WFI_STOP		(2 << 8)
#define S5P_CFG_WFI_SLEEP		(3 << 8)

#define S5P_OTHER_SYS_INT		24
#define S5P_OTHER_STA_TYPE		23
#define S5P_OTHER_SYSC_INTOFF		(1 << 0)
#define STA_TYPE_EXPON			0
#define STA_TYPE_SFR			1

#define S5P_PWR_STA_EXP_SCALE		0
#define S5P_PWR_STA_CNT			4

#define S5P_PWR_STABLE_COUNT		85500

#define S5P_SLEEP_CFG_OSC_EN		(1 << 0)
#define S5P_SLEEP_CFG_USBOSC_EN		(1 << 1)

/* OTHERS Resgister */
#define S5P_OTHERS_RET_IO		(1 << 31)
#define S5P_OTHERS_RET_CF		(1 << 30)
#define S5P_OTHERS_RET_MMC		(1 << 29)
#define S5P_OTHERS_RET_UART		(1 << 28)
#define S5P_OTHERS_USB_SIG_MASK		(1 << 16)

/* S5P_DAC_CONTROL */
#define S5P_DAC_ENABLE			(1)
#define S5P_DAC_DISABLE			(0)

#endif /* __ASM_ARCH_REGS_CLOCK_H */
