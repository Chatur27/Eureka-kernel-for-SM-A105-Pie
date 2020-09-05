#ifndef __ASM_ARCH_REGS_BTS_H
#define __ASM_ARCH_REGS_BTS_H

/* EXYNOS5433 BTS SFR base address */
#define EXYNOS5433_PA_BTS_DECONM0	0x13A80000
#define EXYNOS5433_PA_BTS_DECONM1	0x13A90000
#define EXYNOS5433_PA_BTS_DECONM2	0x13AA0000
#define EXYNOS5433_PA_BTS_DECONM3	0x13AB0000
#define EXYNOS5433_PA_BTS_DECONM4	0x13AC0000
#define EXYNOS5433_PA_BTS_DECONTV_M0	0x13B00000
#define EXYNOS5433_PA_BTS_DECONTV_M1	0x13B10000
#define EXYNOS5433_PA_BTS_DECONTV_M2	0x13B20000
#define EXYNOS5433_PA_BTS_DECONTV_M3	0x13B30000
#define EXYNOS5433_PA_BTS_FIMC_LITE0	0x12000000
#define EXYNOS5433_PA_BTS_FIMC_LITE1	0x12010000
#define EXYNOS5433_PA_BTS_FIMC_LITE2	0x14500000
#define EXYNOS5433_PA_BTS_FIMC_LITE3	0x12020000
#define EXYNOS5433_PA_BTS_3AA0		0x12030000
#define EXYNOS5433_PA_BTS_3AA1		0x12040000
#define EXYNOS5433_PA_BTS_GSCL0		0x13CC0000
#define EXYNOS5433_PA_BTS_GSCL1		0x13CD0000
#define EXYNOS5433_PA_BTS_GSCL2		0x13CE0000
#define EXYNOS5433_PA_BTS_MFC0		0x15220000
#define EXYNOS5433_PA_BTS_MFC1		0x15230000

#define EXYNOS5433_PA_DREX0		0x10400000
#define EXYNOS5433_PA_DREX1		0x10440000

/* EXYNOS7420 BTS SFR base address */
#define EXYNOS7420_PA_BTS_USBDRD30		0x10EA0000
#define EXYNOS7420_PA_BTS_MODEMX		0x10E30000
#define EXYNOS7420_PA_BTS_SDCARDX		0x10E20000
#define EXYNOS7420_PA_BTS_WIFI1			0x15640000
#define EXYNOS7420_PA_BTS_EMBEDDED		0x15620000
#define EXYNOS7420_PA_BTS_M2M1			0x15090000
#define EXYNOS7420_PA_BTS_M2M0			0x15080000
#define EXYNOS7420_PA_BTS_JPEG0			0x15070000
#define EXYNOS7420_PA_BTS_G2D			0x15180000
#define EXYNOS7420_PA_BTS_G3D0			0x14A00000
#define EXYNOS7420_PA_BTS_G3D1			0x14A20000
#define EXYNOS7420_PA_BTS_SLIMSSS		0x111F0000
#define EXYNOS7420_PA_BTS_SSS			0x111E0000
#define EXYNOS7420_PA_BTS_SMDMA			0x111D0000
#define EXYNOS7420_PA_BTS_MCOMP			0x111A0000



#define EXYNOS7420_PA_BTS_DISP_RO_0		0x13A80000
#define EXYNOS7420_PA_BTS_DISP_RO_1		0x13A90000
#define EXYNOS7420_PA_BTS_DISP_RW_0		0x13AA0000
#define EXYNOS7420_PA_BTS_DISP_RW_1		0x13AB0000
#define EXYNOS7420_PA_BTS_VPP0			0x13E60000
#define EXYNOS7420_PA_BTS_VPP1			0x13E70000
#define EXYNOS7420_PA_BTS_VPP2			0x13E80000
#define EXYNOS7420_PA_BTS_VPP3			0x13E90000
#define EXYNOS7420_PA_BTS_TREX_FIMC_BNS_A	0x14400000
#define EXYNOS7420_PA_BTS_TREX_FIMC_BNS_B	0x14410000
#define EXYNOS7420_PA_BTS_TREX_FIMC_BNS_C	0x14500000
#define EXYNOS7420_PA_BTS_TREX_FIMC_BNS_D	0x14420000
#define EXYNOS7420_PA_BTS_TREX_3AA0		0x14430000
#define EXYNOS7420_PA_BTS_TREX_3AA1		0x14440000
#define EXYNOS7420_PA_BTS_TREX_ISPCPU		0x14540000
#define EXYNOS7420_PA_BTS_TREX_VRA		0x14530000
#define EXYNOS7420_PA_BTS_TREX_SCALER		0x14520000
#define EXYNOS7420_PA_BTS_TREX_ISP1		0x14510000
#define EXYNOS7420_PA_BTS_TREX_TPU		0x14610000
#define EXYNOS7420_PA_BTS_TREX_ISP0		0x14600000
#define EXYNOS7420_PA_BTS_MFC_0			0x15220000
#define EXYNOS7420_PA_BTS_MFC_1			0x15230000
#define EXYNOS7420_PA_BTS_BIG			0x10530000
#define EXYNOS7420_PA_BTS_LITTLE		0x10540000

#define EXYNOS7_PA_DREX0	0x10800000
#define EXYNOS7_PA_DREX1	0x10900000
#define EXYNOS7_PA_DREX2	0x10A00000
#define EXYNOS7_PA_DREX3	0x10B00000
#define EXYNOS7_PA_NSP		0x10550000
#define EXYNOS7_PA_SYSREG	0x10050000

#define EXYNOS7_TREX_ID_MASK	0x3F

/* EXYNOS7420 BTS system register offet */
#define FSYS0_QOS_VAL0	0x1038

/* EXYNOS7420 BTS SFR offset*/
#define READ_QOS_CONTROL			0x0100
#define READ_QOS_MODE				0x0104
#define READ_CHANNEL_PRIORITY			0x0200
#define READ_TOKEN_MAX_VALUE			0x0204
#define READ_BW_UPPER_BOUNDARY			0x020C
#define READ_BW_LOWER_BOUNDARY			0x0210
#define READ_INITIAL_TOKEN_VALUE		0x0214
#define READ_DEMOTION_WINDOW			0x0220
#define READ_DEMOTION_TOKEN			0x0224
#define READ_DEFAULT_WINDOW			0x0228
#define READ_DEFAULT_TOKEN			0x022C
#define READ_PROMOTION_WINDOW			0X0230
#define READ_PROMOTION_TOKEN			0x0234
#define READ_ISSUE_CAPABILITY_UPPER_BOUNDARY	0x0240
#define READ_ISSUE_CAPABILITY_LOWER_BOUNDARY	0x0244
#define READ_FLEXIBLE_BLOCKING_CONTROL		0x0280
#define READ_FLEXIBLE_BLOCKING_POLARITY		0x0284
#define READ_MO					0x02F0

#define WRITE_QOS_CONTROL			0x0180
#define WRITE_QOS_MODE				0x0184
#define WRITE_CHANNEL_PRIORITY			0x0300
#define WRITE_TOKEN_MAX_VALUE			0x0304
#define WRITE_BW_UPPER_BOUNDARY			0x030C
#define WRITE_BW_LOWER_BOUNDARY			0x0310
#define WRITE_INITIAL_TOKEN_VALUE		0x0314
#define WRITE_DEMOTION_WINDOW			0x0320
#define WRITE_DEMOTION_TOKEN			0x0324
#define WRITE_DEFAULT_WINDOW			0x0328
#define WRITE_DEFAULT_TOKEN			0x032C
#define WRITE_PROMOTION_WINDOW			0X0330
#define WRITE_PROMOTION_TOKEN			0x0334
#define WRITE_ISSUE_CAPABILITY_UPPER_BOUNDARY	0x0340
#define WRITE_ISSUE_CAPABILITY_LOWER_BOUNDARY	0x0344
#define WRITE_FLEXIBLE_BLOCKING_CONTROL		0x0380
#define WRITE_FLEXIBLE_BLOCKING_POLARITY	0x0384
#define WRITE_MO				0x03F0

#define FBM_MODESEL0		0x00
#define FBM_MODESEL1		0x04
#define FBM_MODESEL2		0x08
#define FBM_THRESHOLD0		0x40
#define FBM_THRESHOLD1		0x44
#define FBM_THRESHOLD2		0x48
#define FBM_OUTSEL0		0x80
#define FBM_OUTSEL2		0x88
#define FBM_OUTSEL20		0xD0

#define TREX_QOS_CONTROL				0x0000
#define TREX_DEMOTION_QOS_VALUE				0x0080
#define TREX_DEFAULT_QOS_VALUE				0x0084
#define TREX_PROMOTION_QOS_VALUE			0x0088
#define TREX_QOS_THRESHOLD_FOR_EMERGENCY_RISING		0x00D0
#define TREX_QOS_THRESHOLD_FOR_EMERGENCY_FALLING	0x00D4

#define QOS_TIMEOUT_0x8		0xA0
#define QOS_TIMEOUT_0xA		0xB0
#define QOS_TIMEOUT_0xB		0xB8
#define QOS_TIMEOUT_0xC		0xC0
#define QOS_TIMEOUT_0xD		0xC8
#define QOS_TIMEOUT_0xE		0xD0
#define QOS_TIMEOUT_0xF		0xD8
#define QOS_TIMEOUT_0x5		0x88
#define QOS_TIMEOUT_0x4		0x80
#define BRB_CON				0x100
#define BRB_THRESHOLD		0x104
#define NSP_CH0			0x0008
#define NSP_CH1			0x0408
#define NSP_CH2			0x0808
#define NSP_CH3			0x0C08

#define SYSREG_FSYS0 0x1064
#define SYSREG_FSYS1 0x2664
#define SYSREG_APM_R 0x1438
#define SYSREG_APM_W 0x143C
#define SYSREG_RTIC 0x1440
#define SYSREG_PDMA_SECURE_R 0x144C
#define SYSREG_PDMA_SECURE_W 0x1450
#define SYSREG_AUD 0x0248

/* Exynos8890 BTS */
#define EXYNOS8890_PA_BTS_TREX_DISP0_0	0x11F30000
#define EXYNOS8890_PA_BTS_TREX_DISP0_1	0x11F40000
#define EXYNOS8890_PA_BTS_TREX_DISP1_0	0x11F50000
#define EXYNOS8890_PA_BTS_TREX_DISP1_1	0x11F60000
#define EXYNOS8890_PA_BTS_TREX_ISP0	0x11F20000
#define EXYNOS8890_PA_BTS_TREX_CAM0	0x11F70000
#define EXYNOS8890_PA_BTS_TREX_CAM1	0x11F10000
#define EXYNOS8890_PA_BTS_TREX_CP	0x10730000
#define EXYNOS8890_PA_BTS_TREX_MFC0	0x11D30000
#define EXYNOS8890_PA_BTS_TREX_MFC1	0x11D40000
#define EXYNOS8890_PA_BTS_TREX_G3D0	0x10680000
#define EXYNOS8890_PA_BTS_TREX_G3D1	0x10690000
#define EXYNOS8890_PA_BTS_TREX_FSYS0	0x11D00000
#define EXYNOS8890_PA_BTS_TREX_FSYS1	0x11F00000
#define EXYNOS8890_PA_BTS_TREX_MSCL0	0x11D10000
#define EXYNOS8890_PA_BTS_TREX_MSCL1	0x11D20000

#define EXYNOS8_PA_SYSREG	0x10050000

#define EXYNOS8_PA_TREX_CCORE0	0x10704000
#define EXYNOS8_PA_TREX_CCORE1	0x10714000

#define EXYNOS8_PA_SCI		0x10500000
#define CMDTOKEN		0x00EC

#define SCI_CTRL		0x0000
#define READ_QURGENT		0x0010
#define WRITE_QURGENT		0x0030
#define VC_NUM0		0x0050
#define VC_NUM1		0x0054
#define TH_IMM0		0x0100
#define TH_IMM1		0x0104
#define TH_IMM2		0x0108
#define TH_IMM3		0x010C
#define TH_IMM4		0x0110
#define TH_IMM5		0x0114
#define TH_IMM6		0x0118
#define TH_HIGH0		0x0200
#define TH_HIGH1		0x0204
#define TH_HIGH2		0x0208
#define TH_HIGH3		0x020C
#define TH_HIGH4		0x0210
#define TH_HIGH5		0x0214
#define TH_HIGH6		0x0218

#endif /* __ASM_ARCH_REGS_BTS_H */
