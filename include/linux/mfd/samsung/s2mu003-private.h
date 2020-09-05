/*
 * s2mu003-private.h - Voltage regulator driver for the Samsung s2mpb02
 *
 *  Copyright (C) 2014 Samsung Electrnoics
 *  XXX <xxx@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#ifndef __LINUX_MFD_S2MU003_PRIV_H
#define __LINUX_MFD_S2MU003_PRIV_H


/* S2MU003 registers */
enum s2mpu003_reg {
	S2MU003_CHG_STATUS,
	S2MU003_CHG_CTRL1,
	S2MU003_CHG_CTRL2,
	S2MU003_PMIC_ID,
	S2MU003_CHG_CTRL3,
	S2MU003_CHG_CTRL4,
	S2MU003_CHG_CTRL5,
	S2MU003_SoftRESET,
	S2MU003_CHG_CTRL6,
	S2MU003_CHG_CTRL7,
	S2MU003_CHG_CTRL8,
	S2MU003_CHG_STATUS2,
	S2MU003_CHG_STATUS3,
	S2MU003_CHG_STATUS4,
	S2MU003_CHG_CTRL9,
	S2MU003_FLED_CH1_CTRL0 = 0x20,
	S2MU003_FLED_CH1_CTRL1,
	S2MU003_FLED_CH1_CTRL2,
	S2MU003_FLED_CH1_CTRL3,
	S2MU003_FLED_CH1_CTRL4,
	S2MU003_FLED_CH1_CTRL5,
	S2MU003_FLED_CH2_CTRL0,
	S2MU003_FLED_CH2_CTRL1,
	S2MU003_FLED_CH2_CTRL2,
	S2MU003_FLED_CH2_CTRL3,
	S2MU003_FLED_CH2_CTRL4,
	S2MU003_FLED_CH2_CTRL5,
	S2MU003_FLED_CTRL0,
	S2MU003_FLED_CTRL1,
	S2MU003_FLED_CTRL2,
	S2MU003_FLED_CTRL3,
	S2MU003_Buck_LDO_CTRL = 0x41,
	S2MU003_Buck_CTRL,
	S2MU003_LDO_CTRL,
	S2MU003_Buck_CTRL2 = 0x45,
	S2MU003_MRSTB_CTRL = 0X47,
	S2MU003_CHG_INT1 = 0X60,
	S2MU003_CHG_INT2,
	S2MU003_CHG_INT3,
	S2MU003_CHG_INT1M,
	S2MU003_CHG_INT2M,
	S2MU003_CHG_INT3M,
	S2MU003_FLED_INT,
	S2MU003_FLED_INTM,
	S2MU003_PMIC_INT,
	S2MU003_PMIC_INTM,
	S2MU003_SHUTDOWN_CTRL,
	S2MU003_OFF_EVENT,
	S2MU003_FLED_STATUS,
	S2MU003_PMIC_STATUS,

};

/* S2MU003 regulator ids */
enum S2MU003_regulators {
	S2MU003_CAMLDO,
	S2MU003_BUCK,
	S2MU003_REG_MAX,
};

#define S2MU003_BUCK_MIN1	600000
#define S2MU003_LDO_MIN1	800000
#define S2MU003_BUCK_STEP1	100000
#define S2MU003_LDO_STEP1	100000
#define S2MU003_CAMLDO_VSEL_MASK	0x1F
#define S2MU003_BUCK_VSEL_MASK	0x1F
#define S2MU003_BUCK_ENABLE_MASK 0x10
#define S2MU003_CAMLDO_ENABLE_MASK	0x20

#define S2MU003_RAMP_DELAY	12000

#define S2MU003_ENABLE_TIME_LDO		180
#define S2MU003_ENABLE_TIME_BUCK	100

#define S2MU003_BUCK_ENABLE_SHIFT	0x04
#define S2MU003_CAMLDO_ENABLE_SHIFT	0x06

#define S2MU003_CAMLDO_N_VOLTAGES	(S2MU003_CAMLDO_VSEL_MASK + 1)
#define S2MU003_BUCK_N_VOLTAGES		(S2MU003_BUCK_VSEL_MASK + 1)

#define S2MU003_REGULATOR_MAX (S2MU003_REG_MAX)

#endif /* __LINUX_MFD_S2MU003_PRIV_H */
