/*
 * Copyright 2016 Wan Zongshun <mcuos.com@gmail.com>
 *
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */

#ifndef __ASM_ARCH_REGS_GCR_H
#define __ASM_ARCH_REGS_GCR_H

/* NUC970 GCR regs */

#define REG_PDID	0x000
#define REG_PWRON	0x004
#define REG_ARBCON	0x008
#define REG_LVRDCR	0x020
#define REG_MISCFCR	0x030
#define REG_MISCIER	0x040
#define REG_MISCISR	0x044
#define REG_ROMSUM0	0x048
#define REG_ROMSUM1	0x04C
#define REG_WKUPSER	0x058
#define REG_WKUPSSR	0x05C
#define REG_AHBIPRST	0x060
#define REG_APBIPRST0	0x064
#define REG_APBIPRST1	0x068
#define REG_RSTSTS	0x06C
#define REG_DDR_DS_CR	0x0E0
#define REG_PORDISCR	0x100
#define REG_ICEDBGCR	0x104
#define REG_WRPRTR	0x1FC
#define REG_MFP_GPA_L	0x070
#define REG_MFP_GPA_H	0x074
#define REG_MFP_GPB_L	0x078
#define REG_MFP_GPB_H	0x07C
#define REG_MFP_GPC_L	0x080
#define REG_MFP_GPC_H	0x084
#define REG_MFP_GPD_L	0x088
#define REG_MFP_GPD_H	0x08C
#define REG_MFP_GPE_L	0x090
#define REG_MFP_GPE_H	0x094
#define REG_MFP_GPF_L	0x098
#define REG_MFP_GPF_H	0x09C
#define REG_MFP_GPG_L	0x0A0
#define REG_MFP_GPG_H	0x0A4
#define REG_MFP_GPH_L	0x0A8
#define REG_MFP_GPH_H	0x0AC
#define REG_MFP_GPI_L	0x0B0
#define REG_MFP_GPI_H	0x0B4
#define REG_MFP_GPJ_L	0x0B8

#endif /*  __ASM_ARCH_REGS_GCR_H */
