/*
 * arch/arm/mach-w90x900/include/mach/irqs.h
 *
 * Copyright (c) 2008 Nuvoton technology corporation.
 *
 * Wan ZongShun <mcuos.com@gmail.com>
 *
 * Based on arch/arm/mach-s3c2410/include/mach/irqs.h
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation;version 2 of the License.
 *
 */

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H

/*
 * we keep the first set of CPU IRQs out of the range of
 * the ISA space, so that the PC104 has them to itself
 * and we don't end up having to do horrible things to the
 * standard ISA drivers....
 *
 */

#define W90X900_IRQ(x)	(x)

/* Main cpu interrupts */

#if !defined(CONFIG_SOC_NUC970)

#define IRQ_WDT		W90X900_IRQ(1)
#define IRQ_GROUP0	W90X900_IRQ(2)
#define IRQ_GROUP1	W90X900_IRQ(3)
#define IRQ_ACTL	W90X900_IRQ(4)
#define IRQ_LCD		W90X900_IRQ(5)
#define IRQ_RTC		W90X900_IRQ(6)
#define IRQ_UART0	W90X900_IRQ(7)
#define IRQ_UART1	W90X900_IRQ(8)
#define IRQ_UART2	W90X900_IRQ(9)
#define IRQ_UART3	W90X900_IRQ(10)
#define IRQ_UART4	W90X900_IRQ(11)
#define IRQ_TIMER0	W90X900_IRQ(12)
#define IRQ_TIMER1	W90X900_IRQ(13)
#define IRQ_T_INT_GROUP	W90X900_IRQ(14)
#define IRQ_USBH	W90X900_IRQ(15)
#define IRQ_EMCTX	W90X900_IRQ(16)
#define IRQ_EMCRX	W90X900_IRQ(17)
#define IRQ_GDMAGROUP	W90X900_IRQ(18)
#define IRQ_DMAC	W90X900_IRQ(19)
#define IRQ_FMI		W90X900_IRQ(20)
#define IRQ_USBD	W90X900_IRQ(21)
#define IRQ_ATAPI	W90X900_IRQ(22)
#define IRQ_G2D		W90X900_IRQ(23)
#define IRQ_PCI		W90X900_IRQ(24)
#define IRQ_SCGROUP	W90X900_IRQ(25)
#define IRQ_I2CGROUP	W90X900_IRQ(26)
#define IRQ_SSP		W90X900_IRQ(27)
#define IRQ_PWM		W90X900_IRQ(28)
#define IRQ_KPI		W90X900_IRQ(29)
#define IRQ_P2SGROUP	W90X900_IRQ(30)
#define IRQ_ADC		W90X900_IRQ(31)
#define NR_IRQS		(IRQ_ADC+1)

/*for irq group*/

#define	IRQ_PS2_PORT0	0x10000000
#define	IRQ_PS2_PORT1	0x20000000
#define	IRQ_I2C_LINE0	0x04000000
#define	IRQ_I2C_LINE1	0x08000000
#define	IRQ_SC_CARD0	0x01000000
#define	IRQ_SC_CARD1	0x02000000
#define	IRQ_GDMA_CH0	0x00100000
#define	IRQ_GDMA_CH1	0x00200000
#define	IRQ_TIMER2	0x00010000
#define	IRQ_TIMER3	0x00020000
#define	IRQ_TIMER4	0x00040000
#define	IRQ_GROUP0_IRQ0	0x00000001
#define	IRQ_GROUP0_IRQ1	0x00000002
#define	IRQ_GROUP0_IRQ2	0x00000004
#define	IRQ_GROUP0_IRQ3	0x00000008
#define	IRQ_GROUP1_IRQ4	0x00000010
#define	IRQ_GROUP1_IRQ5	0x00000020
#define	IRQ_GROUP1_IRQ6	0x00000040
#define	IRQ_GROUP1_IRQ7	0x00000080

#else

/*For nuc970*/
#define IRQ_WDT		W90X900_IRQ(1)
#define IRQ_WWDT	W90X900_IRQ(2)
#define IRQ_LVD		W90X900_IRQ(3)
#define IRQ_EXT0	W90X900_IRQ(4)
#define IRQ_EXT1	W90X900_IRQ(5)
#define IRQ_EXT2	W90X900_IRQ(6)
#define IRQ_EXT3	W90X900_IRQ(7)
#define IRQ_EXT4	W90X900_IRQ(8)
#define IRQ_EXT5	W90X900_IRQ(9)
#define IRQ_EXT6	W90X900_IRQ(10)
#define IRQ_EXT7	W90X900_IRQ(11)
#define IRQ_ACTL	W90X900_IRQ(12)
#define IRQ_LCD		W90X900_IRQ(13)
#define IRQ_CAP		W90X900_IRQ(14)
#define IRQ_RTC		W90X900_IRQ(15)
#define IRQ_TMR0	W90X900_IRQ(16)
#define IRQ_TMR1	W90X900_IRQ(17)
#define IRQ_ADC		W90X900_IRQ(18)
#define IRQ_EMC0RX	W90X900_IRQ(19)
#define IRQ_EMC1RX	W90X900_IRQ(20)
#define IRQ_EMC0TX	W90X900_IRQ(21)
#define IRQ_EMC1TX	W90X900_IRQ(22)
#define IRQ_EHCI	W90X900_IRQ(23)
#define IRQ_OHCI	W90X900_IRQ(24)
#define IRQ_GDMA0	W90X900_IRQ(25)
#define IRQ_GDMA1	W90X900_IRQ(26)
#define IRQ_SDH		W90X900_IRQ(27)
#define IRQ_FMI		W90X900_IRQ(28)
#define IRQ_UDC		W90X900_IRQ(29)
#define IRQ_TMR2	W90X900_IRQ(30)
#define IRQ_TMR3	W90X900_IRQ(31)
#define IRQ_TMR4	W90X900_IRQ(32)
#define IRQ_JPEG	W90X900_IRQ(33)
#define IRQ_GE2D	W90X900_IRQ(34)
#define IRQ_CRYPTO	W90X900_IRQ(35)
#define IRQ_UART0	W90X900_IRQ(36)
#define IRQ_UART1	W90X900_IRQ(37)
#define IRQ_UART2	W90X900_IRQ(38)
#define IRQ_UART4	W90X900_IRQ(39)
#define IRQ_UART6	W90X900_IRQ(40)
#define IRQ_UART8	W90X900_IRQ(41)
#define IRQ_UART10	W90X900_IRQ(42)
#define IRQ_UART3	W90X900_IRQ(43)
#define IRQ_UART5	W90X900_IRQ(44)
#define IRQ_UART7	W90X900_IRQ(45)
#define IRQ_UART9	W90X900_IRQ(46)
#define IRQ_ETIMER0	W90X900_IRQ(47)
#define IRQ_ETIMER1	W90X900_IRQ(48)
#define IRQ_ETIMER2	W90X900_IRQ(49)
#define IRQ_ETIMER3	W90X900_IRQ(50)
#define IRQ_SPI0	W90X900_IRQ(51)
#define IRQ_SPI1	W90X900_IRQ(52)
#define IRQ_I2C0	W90X900_IRQ(53)
#define IRQ_I2C1	W90X900_IRQ(54)
#define IRQ_SMC0	W90X900_IRQ(55)
#define IRQ_SMC1	W90X900_IRQ(56)
#define IRQ_GPIO	W90X900_IRQ(57)
#define IRQ_CAN0	W90X900_IRQ(58)
#define IRQ_CAN1	W90X900_IRQ(59)
#define IRQ_PWM		W90X900_IRQ(60)
#define IRQ_KPI		W90X900_IRQ(61)
#define NR_IRQS		(IRQ_KPI+1)
#endif

#endif /* __ASM_ARCH_IRQ_H */
