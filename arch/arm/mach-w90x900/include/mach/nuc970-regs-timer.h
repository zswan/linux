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

#ifndef __ASM_ARCH_REGS_TIMER_H
#define __ASM_ARCH_REGS_TIMER_H

/* Timer Registers */

#define TMR_BA			0x0

#define REG_TMR_TCSR0		(TMR_BA+0x00)
#define REG_TMR_TICR0		(TMR_BA+0x04)
#define REG_TMR_TDR0		(TMR_BA+0x08)


#define REG_TMR_TCSR1		(TMR_BA+0x10)
#define REG_TMR_TICR1		(TMR_BA+0x14)
#define REG_TMR_TDR1		(TMR_BA+0x18)


#define REG_TMR_TCSR2		(TMR_BA+0x20)
#define REG_TMR_TICR2		(TMR_BA+0x24)
#define REG_TMR_TDR2		(TMR_BA+0x28)

#define REG_TMR_TCSR3		(TMR_BA+0x30)
#define REG_TMR_TICR3		(TMR_BA+0x34)
#define REG_TMR_TDR3		(TMR_BA+0x38)

#define REG_TMR_TCSR4		(TMR_BA+0x40)
#define REG_TMR_TICR4		(TMR_BA+0x44)
#define REG_TMR_TDR4		(TMR_BA+0x48)

#define REG_TMR_TISR		(TMR_BA+0x60)


#endif /*  __ASM_ARCH_REGS_TIMER_H */
