/*
 *  pnx833x.h: Register mappings for PNX833X.
 *
 *  Copyright 2008 NXP Semiconductors
 *	  Chris Steel <chris.steel@nxp.com>
 *    Daniel Laird <daniel.j.laird@nxp.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __ASM_MIPS_MACH_PNX833X_PNX833X_H
#define __ASM_MIPS_MACH_PNX833X_PNX833X_H

/* All regs are accessed in KSEG1 */
#define PNX833X_BASE		(0xa0000000ul + 0x17E00000ul)

#define PNX833X_REG(offs)	(*((volatile unsigned long *)(PNX833X_BASE + offs)))

/* Registers are named exactly as in PNX833X docs, just with PNX833X_ prefix */

/* Read access to multibit fields */
#define PNX833X_BIT(val, reg, field)	((val) & PNX833X_##reg##_##field)
#define PNX833X_REGBIT(reg, field)	PNX833X_BIT(PNX833X_##reg, reg, field)

/* Use PNX833X_FIELD to extract a field from val */
#define PNX_FIELD(cpu, val, reg, field) \
		(((val) & PNX##cpu##_##reg##_##field##_MASK) >> \
			PNX##cpu##_##reg##_##field##_SHIFT)
#define PNX833X_FIELD(val, reg, field)	PNX_FIELD(833X, val, reg, field)
#define PNX8330_FIELD(val, reg, field)	PNX_FIELD(8330, val, reg, field)
#define PNX8335_FIELD(val, reg, field)	PNX_FIELD(8335, val, reg, field)

/* Use PNX833X_REGFIELD to extract a field from a register */
#define PNX833X_REGFIELD(reg, field)	PNX833X_FIELD(PNX833X_##reg, reg, field)
#define PNX8330_REGFIELD(reg, field)	PNX8330_FIELD(PNX8330_##reg, reg, field)
#define PNX8335_REGFIELD(reg, field)	PNX8335_FIELD(PNX8335_##reg, reg, field)


#define PNX_WRITEFIELD(cpu, val, reg, field) \
	(PNX##cpu##_##reg = (PNX##cpu##_##reg & ~(PNX##cpu##_##reg##_##field##_MASK)) | \
						((val) << PNX##cpu##_##reg##_##field##_SHIFT))
#define PNX833X_WRITEFIELD(val, reg, field) \
					PNX_WRITEFIELD(833X, val, reg, field)
#define PNX8330_WRITEFIELD(val, reg, field) \
					PNX_WRITEFIELD(8330, val, reg, field)
#define PNX8335_WRITEFIELD(val, reg, field) \
					PNX_WRITEFIELD(8335, val, reg, field)


/* Macros to detect CPU type */

#define PNX833X_CONFIG_MODULE_ID		PNX833X_REG(0x7FFC)
#define PNX833X_CONFIG_MODULE_ID_MAJREV_MASK	0x0000f000
#define PNX833X_CONFIG_MODULE_ID_MAJREV_SHIFT	12
#define PNX8330_CONFIG_MODULE_MAJREV		4
#define PNX8335_CONFIG_MODULE_MAJREV		5
#define CPU_IS_PNX8330	(PNX833X_REGFIELD(CONFIG_MODULE_ID, MAJREV) == \
					PNX8330_CONFIG_MODULE_MAJREV)
#define CPU_IS_PNX8335	(PNX833X_REGFIELD(CONFIG_MODULE_ID, MAJREV) == \
					PNX8335_CONFIG_MODULE_MAJREV)



#define PNX833X_RESET_CONTROL		PNX833X_REG(0x8004)
#define PNX833X_RESET_CONTROL_2		PNX833X_REG(0x8014)

#define PNX833X_PIC_REG(offs)		PNX833X_REG(0x01000 + (offs))
#define PNX833X_PIC_INT_PRIORITY	PNX833X_PIC_REG(0x0)
#define PNX833X_PIC_INT_SRC		PNX833X_PIC_REG(0x4)
#define PNX833X_PIC_INT_SRC_INT_SRC_MASK	0x00000FF8ul	/* bits 11:3 */
#define PNX833X_PIC_INT_SRC_INT_SRC_SHIFT	3
#define PNX833X_PIC_INT_REG(irq)	PNX833X_PIC_REG(0x10 + 4*(irq))

#define PNX833X_CLOCK_CPUCP_CTL PNX833X_REG(0x9228)
#define PNX833X_CLOCK_CPUCP_CTL_EXIT_RESET	0x00000002ul	/* bit 1 */
#define PNX833X_CLOCK_CPUCP_CTL_DIV_CLOCK_MASK	0x00000018ul	/* bits 4:3 */
#define PNX833X_CLOCK_CPUCP_CTL_DIV_CLOCK_SHIFT 3

#define PNX8335_CLOCK_PLL_CPU_CTL		PNX833X_REG(0x9020)
#define PNX8335_CLOCK_PLL_CPU_CTL_FREQ_MASK	0x1f
#define PNX8335_CLOCK_PLL_CPU_CTL_FREQ_SHIFT	0

#define PNX833X_CONFIG_MUX		PNX833X_REG(0x7004)
#define PNX833X_CONFIG_MUX_IDE_MUX	0x00000080		/* bit 7 */

#define PNX8330_CONFIG_POLYFUSE_7	PNX833X_REG(0x7040)
#define PNX8330_CONFIG_POLYFUSE_7_BOOT_MODE_MASK	0x00180000
#define PNX8330_CONFIG_POLYFUSE_7_BOOT_MODE_SHIFT	19

#define PNX833X_PIO_IN		PNX833X_REG(0xF000)
#define PNX833X_PIO_OUT		PNX833X_REG(0xF004)
#define PNX833X_PIO_DIR		PNX833X_REG(0xF008)
#define PNX833X_PIO_SEL		PNX833X_REG(0xF014)
#define PNX833X_PIO_INT_EDGE	PNX833X_REG(0xF020)
#define PNX833X_PIO_INT_HI	PNX833X_REG(0xF024)
#define PNX833X_PIO_INT_LO	PNX833X_REG(0xF028)
#define PNX833X_PIO_INT_STATUS	PNX833X_REG(0xFFE0)
#define PNX833X_PIO_INT_ENABLE	PNX833X_REG(0xFFE4)
#define PNX833X_PIO_INT_CLEAR	PNX833X_REG(0xFFE8)
#define PNX833X_PIO_IN2		PNX833X_REG(0xF05C)
#define PNX833X_PIO_OUT2	PNX833X_REG(0xF060)
#define PNX833X_PIO_DIR2	PNX833X_REG(0xF064)
#define PNX833X_PIO_SEL2	PNX833X_REG(0xF068)

#define PNX833X_UART0_PORTS_START	(PNX833X_BASE + 0xB000)
#define PNX833X_UART0_PORTS_END		(PNX833X_BASE + 0xBFFF)
#define PNX833X_UART1_PORTS_START	(PNX833X_BASE + 0xC000)
#define PNX833X_UART1_PORTS_END		(PNX833X_BASE + 0xCFFF)

#define PNX833X_USB_PORTS_START		(PNX833X_BASE + 0x19000)
#define PNX833X_USB_PORTS_END		(PNX833X_BASE + 0x19FFF)

#define PNX833X_CONFIG_USB		PNX833X_REG(0x7008)

#define PNX833X_I2C0_PORTS_START	(PNX833X_BASE + 0xD000)
#define PNX833X_I2C0_PORTS_END		(PNX833X_BASE + 0xDFFF)
#define PNX833X_I2C1_PORTS_START	(PNX833X_BASE + 0xE000)
#define PNX833X_I2C1_PORTS_END		(PNX833X_BASE + 0xEFFF)

#define PNX833X_IDE_PORTS_START		(PNX833X_BASE + 0x1A000)
#define PNX833X_IDE_PORTS_END		(PNX833X_BASE + 0x1AFFF)
#define PNX833X_IDE_MODULE_ID		PNX833X_REG(0x1AFFC)

#define PNX833X_IDE_MODULE_ID_MODULE_ID_MASK	0xFFFF0000
#define PNX833X_IDE_MODULE_ID_MODULE_ID_SHIFT	16
#define PNX833X_IDE_MODULE_ID_VALUE		0xA009


#define PNX833X_MIU_SEL0			PNX833X_REG(0x2004)
#define PNX833X_MIU_SEL0_TIMING		PNX833X_REG(0x2008)
#define PNX833X_MIU_SEL1			PNX833X_REG(0x200C)
#define PNX833X_MIU_SEL1_TIMING		PNX833X_REG(0x2010)
#define PNX833X_MIU_SEL2			PNX833X_REG(0x2014)
#define PNX833X_MIU_SEL2_TIMING		PNX833X_REG(0x2018)
#define PNX833X_MIU_SEL3			PNX833X_REG(0x201C)
#define PNX833X_MIU_SEL3_TIMING		PNX833X_REG(0x2020)

#define PNX833X_MIU_SEL0_SPI_MODE_ENABLE_MASK	(1 << 14)
#define PNX833X_MIU_SEL0_SPI_MODE_ENABLE_SHIFT	14

#define PNX833X_MIU_SEL0_BURST_MODE_ENABLE_MASK (1 << 7)
#define PNX833X_MIU_SEL0_BURST_MODE_ENABLE_SHIFT	7

#define PNX833X_MIU_SEL0_BURST_PAGE_LEN_MASK	(0xF << 9)
#define PNX833X_MIU_SEL0_BURST_PAGE_LEN_SHIFT	9

#define PNX833X_MIU_CONFIG_SPI		PNX833X_REG(0x2000)

#define PNX833X_MIU_CONFIG_SPI_OPCODE_MASK	(0xFF << 3)
#define PNX833X_MIU_CONFIG_SPI_OPCODE_SHIFT	3

#define PNX833X_MIU_CONFIG_SPI_DATA_ENABLE_MASK (1 << 2)
#define PNX833X_MIU_CONFIG_SPI_DATA_ENABLE_SHIFT	2

#define PNX833X_MIU_CONFIG_SPI_ADDR_ENABLE_MASK (1 << 1)
#define PNX833X_MIU_CONFIG_SPI_ADDR_ENABLE_SHIFT	1

#define PNX833X_MIU_CONFIG_SPI_SYNC_MASK	(1 << 0)
#define PNX833X_MIU_CONFIG_SPI_SYNC_SHIFT	0

#define PNX833X_WRITE_CONFIG_SPI(opcode, data_enable, addr_enable, sync) \
   (PNX833X_MIU_CONFIG_SPI =						\
    ((opcode) << PNX833X_MIU_CONFIG_SPI_OPCODE_SHIFT) |			\
    ((data_enable) << PNX833X_MIU_CONFIG_SPI_DATA_ENABLE_SHIFT) |	\
    ((addr_enable) << PNX833X_MIU_CONFIG_SPI_ADDR_ENABLE_SHIFT) |	\
    ((sync) << PNX833X_MIU_CONFIG_SPI_SYNC_SHIFT))

#define PNX8335_IP3902_PORTS_START		(PNX833X_BASE + 0x2F000)
#define PNX8335_IP3902_PORTS_END		(PNX833X_BASE + 0x2FFFF)
#define PNX8335_IP3902_MODULE_ID		PNX833X_REG(0x2FFFC)

#define PNX8335_IP3902_MODULE_ID_MODULE_ID_MASK		0xFFFF0000
#define PNX8335_IP3902_MODULE_ID_MODULE_ID_SHIFT	16
#define PNX8335_IP3902_MODULE_ID_VALUE			0x3902

 /* I/O location(gets remapped)*/
#define PNX8335_NAND_BASE	    0x18000000
/* I/O location with CLE high */
#define PNX8335_NAND_CLE_MASK	0x00100000
/* I/O location with ALE high */
#define PNX8335_NAND_ALE_MASK	0x00010000

#define PNX8335_SATA_PORTS_START	(PNX833X_BASE + 0x2E000)
#define PNX8335_SATA_PORTS_END		(PNX833X_BASE + 0x2EFFF)
#define PNX8335_SATA_MODULE_ID		PNX833X_REG(0x2EFFC)

#define PNX8335_SATA_MODULE_ID_MODULE_ID_MASK	0xFFFF0000
#define PNX8335_SATA_MODULE_ID_MODULE_ID_SHIFT	16
#define PNX8335_SATA_MODULE_ID_VALUE		0xA099

#endif
