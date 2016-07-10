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

#ifndef __DT_BINDINGS_CLOCK_NUC970_H
#define __DT_BINDINGS_CLOCK_NUC970_H

/*SOURCE*/

#define	XIN			0
#define	APLL			1
#define	UPLL			2
#define	XIN32K			3
#define	XIN128_DIV		4

/*ECLK*/

#define	USB_APLLDIV		5
#define	USB_UPLLDIV		6
#define	USB_ECLK_MUX		7
#define	USB_ECLK_DIV		8
#define	USB_ECLK_GATE		9
#define	SD_APLLDIV		10
#define	SD_UPLLDIV		11
#define	SD_ECLK_MUX		12
#define	SD_ECLK_DIV		13
#define	SD_ECLK_GATE		14
#define	LCD_APLLDIV		15
#define	LCD_UPLLDIV		16
#define	LCD_ECLK_MUX		17
#define	LCD_ECLK_DIV		18
#define	LCD_ECLK_GATE		19
#define	ADC_APLLDIV		20
#define	ADC_UPLLDIV		21
#define	ADC_ECLK_MUX		22
#define	ADC_ECLK_DIV		23
#define	ADC_ECLK_GATE		24
#define	AUDIO_APLLDIV		25
#define	AUDIO_UPLLDIV		26
#define	AUDIO_ECLK_MUX		27
#define	AUDIO_ECLK_DIV		28
#define	AUDIO_ECLK_GATE		29
#define	CAP_APLLDIV		30
#define	CAP_UPLLDIV		31
#define	CAP_ECLK_MUX		32
#define	CAP_ECLK_DIV		33
#define	CAP_ECLK_GATE		34
#define	SDH_APLLDIV		35
#define	SDH_UPLLDIV		36
#define	SDH_ECLK_MUX		37
#define	SDH_ECLK_DIV		38
#define	SDH_ECLK_GATE		39
#define	EMMC_APLLDIV		40
#define	EMMC_UPLLDIV		41
#define	EMMC_ECLK_MUX		42
#define	EMMC_ECLK_DIV		43
#define	EMMC_ECLK_GATE		44
#define	UART0_APLLDIV		45
#define	UART0_UPLLDIV		46
#define	UART0_ECLK_MUX		47
#define	UART0_ECLK_DIV		48
#define	UART0_ECLK_GATE		49
#define	UART1_APLLDIV		50
#define	UART1_UPLLDIV		51
#define	UART1_ECLK_MUX		52
#define	UART1_ECLK_DIV		53
#define	UART1_ECLK_GATE		54
#define	UART2_APLLDIV		55
#define	UART2_UPLLDIV		56
#define	UART2_ECLK_MUX		57
#define	UART2_ECLK_DIV		58
#define	UART2_ECLK_GATE		59
#define	UART3_APLLDIV		60
#define	UART3_UPLLDIV		61
#define	UART3_ECLK_MUX		62
#define	UART3_ECLK_DIV		63
#define	UART3_ECLK_GATE		64
#define	UART4_APLLDIV		65
#define	UART4_UPLLDIV		66
#define	UART4_ECLK_MUX		67
#define	UART4_ECLK_DIV		68
#define	UART4_ECLK_GATE		69
#define	UART5_APLLDIV		70
#define	UART5_UPLLDIV		71
#define	UART5_ECLK_MUX		72
#define	UART5_ECLK_DIV		73
#define	UART5_ECLK_GATE		74
#define	UART6_APLLDIV		75
#define	UART6_UPLLDIV		76
#define	UART6_ECLK_MUX		77
#define	UART6_ECLK_DIV		78
#define	UART6_ECLK_GATE		79
#define	UART7_APLLDIV		80
#define	UART7_UPLLDIV		81
#define	UART7_ECLK_MUX		82
#define	UART7_ECLK_DIV		83
#define	UART7_ECLK_GATE		84
#define	UART8_APLLDIV		85
#define	UART8_UPLLDIV		86
#define	UART8_ECLK_MUX		87
#define	UART8_ECLK_DIV		88
#define	UART8_ECLK_GATE		89
#define	UART9_APLLDIV		90
#define	UART9_UPLLDIV		91
#define	UART9_ECLK_MUX		92
#define	UART9_ECLK_DIV		93
#define	UART9_ECLK_GATE		94
#define	UART10_APLLDIV		95
#define	UART10_UPLLDIV		96
#define	UART10_ECLK_MUX		97
#define	UART10_ECLK_DIV		98
#define	UART10_ECLK_GATE	99
#define	SYSTEM_APLLDIV		100
#define	SYSTEM_UPLLDIV		101
#define	SYSTEM_ECLK_MUX		102
#define	SYSTEM_ECLK_DIV		103
#define	SYSTEM_ECLK_GATE	104
#define	GPIO_ECLK_MUX		105
#define	GPIO_ECLK_DIV		106
#define	GPIO_ECLK_GATE		107
#define	KPI_ECLK_MUX		108
#define	KPI_ECLK_DIV		109
#define	KPI_ECLK_GATE		110
#define	ETIMER0_ECLK_MUX	111
#define	ETIMER0_ECLK_GATE	12
#define	ETIMER1_ECLK_MUX	113
#define	ETIMER1_ECLK_GATE	114
#define	ETIMER2_ECLK_MUX	115
#define	ETIMER2_ECLK_GATE	116
#define	ETIMER3_ECLK_MUX	117
#define	ETIMER3_ECLK_GATE	118
#define	WWDT_ECLK_MUX		119
#define	WWDT_ECLK_GATE		120
#define	WDT_ECLK_MUX		121
#define	WDT_ECLK_GATE		122
#define	SMC0_ECLK_DIV		123
#define	SMC0_ECLK_GATE		124
#define	SMC0_GATE		125
#define	SMC1_ECLK_DIV		126
#define	SMC1_ECLK_GATE		127
#define	SMC1_GATE		128

/*SYS*/

#define	SYS_MUX			129
#define	SYS_DIV			130
#define	CPU_DIV			131
#define	CPU_GATE		132
#define	DDR_GATE		133

/*HCLK*/

#define	HCLK_GATE		134
#define	HCLK1_DIV		135
#define	GDMA_GATE		136
#define	EBI_GATE		137
#define	TIC_GATE		138
#define	SRAM_GATE		139
#define	HCLKN_DIV		140
#define	DRAM_GATE		141
#define	HCLK234_DIV		142
#define	USBH_GATE		143
#define	EMAC1_GATE		144
#define	EMAC1_ECLK_DIV		145
#define	EMAC1_ECLK_GATE		146
#define	USBD_GATE		147
#define	FMI_GATE		148
#define	NAND_GATE		149
#define	EMMC_GATE		150
#define	CRYPTO_GATE		151
#define	JPEG_GATE		152
#define	JPEG_ECLK_DIV		153
#define	JPEG_ECLK_GATE		154
#define	GE2D_GATE		155
#define	GE2D_ECLK_DIV		156
#define	GE2D_ECLK_GATE		157
#define	EMAC0_GATE		158
#define	EMAC0_ECLK_DIV		159
#define	EMAC0_ECLK_GATE		160
#define	SDH_GATE		161
#define	AUDIO_GATE		162
#define	LCD_GATE		163
#define	CAP_GATE		164
#define	SENSOR_GATE		165

/*PCLK*/

#define	PCLK_DIV		166
#define	PCLK4096_DIV		167
#define	I2C0_GATE		168
#define	I2C1_GATE		169
#define	SPI0_GATE		170
#define	SPI1_GATE		171
#define	UART0_GATE		172
#define	UART1_GATE		173
#define	UART2_GATE		174
#define	UART3_GATE		175
#define	UART4_GATE		176
#define	UART5_GATE		177
#define	UART6_GATE		178
#define	UART7_GATE		179
#define	UART8_GATE		180
#define	UART9_GATE		181
#define	UART10_GATE		182
#define	TIMER0_GATE		183
#define	TIMER1_GATE		184
#define	TIMER2_GATE		185
#define	TIMER3_GATE		186
#define	TIMER4_GATE		187
#define	WDT_GATE		188
#define	RTC_GATE		189
#define	WWDT_GATE		190
#define	GPIO_GATE		191
#define	ADC_GATE		192
#define	KPI_GATE		193
#define	MTPC_GATE		194
#define	PWM_GATE		195
#define	ETIMER0_GATE		196
#define	ETIMER1_GATE		197
#define	ETIMER2_GATE		198
#define	ETIMER3_GATE		199
#define	CAN0_GATE		200
#define	CAN1_GATE		201
#define	NUC970_CLK_MAX		202

#endif
