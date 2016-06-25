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

#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/clk-provider.h>
#include <linux/spinlock.h>
#include <linux/of.h>
#include <linux/of_address.h>

#include <dt-bindings/clock/nuc970-clock.h>
#include "clk-ccf.h"

/* Clock Control Registers  */
static void __iomem *clkctrl;
#define CLK_BA clkctrl

#define REG_CLK_PMCON	(CLK_BA + 0x000)
#define REG_CLK_HCLKEN	(CLK_BA + 0x010)
#define REG_CLK_PCLKEN0	(CLK_BA + 0x018)
#define REG_CLK_PCLKEN1	(CLK_BA + 0x01C)
#define REG_CLK_DIV0	(CLK_BA + 0x020)
#define REG_CLK_DIV1	(CLK_BA + 0x024)
#define REG_CLK_DIV2	(CLK_BA + 0x028)
#define REG_CLK_DIV3	(CLK_BA + 0x02C)
#define REG_CLK_DIV4	(CLK_BA + 0x030)
#define REG_CLK_DIV5	(CLK_BA + 0x034)
#define REG_CLK_DIV6	(CLK_BA + 0x038)
#define REG_CLK_DIV7	(CLK_BA + 0x03C)
#define REG_CLK_DIV8	(CLK_BA + 0x040)
#define REG_CLK_DIV9	(CLK_BA + 0x044)
#define REG_CLK_APLLCON	(CLK_BA + 0x060)
#define REG_CLK_UPLLCON	(CLK_BA + 0x064)

static const char *const sys_sel_clks[] = { "xin",
					    "dummy",
					    "apll",
					    "upll" };

static const char *const lcd_sel_clks[] = { "xin",
					    "dummy",
					    "lcd_aplldiv",
					    "lcd_uplldiv" };

static const char *const audio_sel_clks[] = { "xin",
					      "dummy",
					      "audio_aplldiv",
					      "audio_uplldiv" };

static const char *const usb_sel_clks[] = { "xin",
					    "dummy",
					    "usb_aplldiv",
					    "usb_uplldiv" };

static const char *const adc_sel_clks[] = { "xin",
					    "dummy",
					    "adc_aplldiv",
					    "adc_uplldiv" };

static const char *const cap_sel_clks[] = { "xin",
					    "dummy",
					    "cap_aplldiv",
					    "cap_uplldiv" };

static const char *const sdh_sel_clks[] = { "xin",
					    "dummy",
					    "sdh_aplldiv",
					    "sdh_uplldiv" };

static const char *const emmc_sel_clks[] = { "xin",
					     "dummy",
					     "emmc_aplldiv",
					     "emmc_uplldiv" };

static const char *const uart0_sel_clks[] = { "xin",
					      "dummy",
					      "uart0_aplldiv",
					      "uart0_uplldiv" };

static const char *const uart1_sel_clks[] = { "xin",
					      "dummy",
					      "uart1_aplldiv",
					      "uart1_uplldiv" };

static const char *const uart2_sel_clks[] = { "xin",
					      "dummy",
					      "uart2_aplldiv",
					      "uart2_uplldiv" };

static const char *const uart3_sel_clks[] = { "xin",
					      "dummy",
					      "uart3_aplldiv",
					      "uart3_uplldiv" };

static const char *const uart4_sel_clks[] = { "xin",
					      "dummy",
					      "uart4_aplldiv",
					      "uart4_uplldiv" };

static const char *const uart5_sel_clks[] = { "xin",
					      "dummy",
					      "uart5_aplldiv",
					      "uart5_uplldiv" };

static const char *const uart6_sel_clks[] = { "xin",
					      "dummy",
					      "uart6_aplldiv",
					      "uart6_uplldiv" };

static const char *const uart7_sel_clks[] = { "xin",
					      "dummy",
					      "uart7_aplldiv",
					      "uart7_uplldiv" };

static const char *const uart8_sel_clks[] = { "xin",
					      "dummy",
					      "uart8_aplldiv",
					      "uart8_uplldiv" };

static const char *const uart9_sel_clks[] = { "xin",
					      "dummy",
					      "uart9_aplldiv",
					      "uart9_uplldiv" };

static const char *const uart10_sel_clks[] = { "xin",
					       "dummy",
					       "uart10_aplldiv",
					       "uart10_uplldiv" };

static const char *const system_sel_clks[] = { "xin",
					       "dummy",
					       "system_aplldiv",
					       "system_uplldiv" };

static const char *const gpio_sel_clks[] = { "xin", "xin32k"};
static const char *const kpi_sel_clks[] = { "xin", "xin32k"};
static const char *const etimer_sel_clks[] = { "xin",
					       "pclk_div",
					      "pclk4096_div",
					      "xin32k" };

static const char *const wwdt_sel_clks[] = { "xin",
					     "xin128_div",
					     "pclk4096_div",
					     "xin32k" };

static struct clk *clk[NUC970_CLK_MAX];
static struct clk_onecell_data clk_data;

static void __init nuc970_clocks_init(struct device_node *np)
{
	int i;

	clkctrl = of_iomap(np, 0);
	WARN_ON(!clkctrl);

	/* source */
	clk[XIN]	= nuc970_clk_fixed("xin", 12000000);
	clk[XIN32K]	= nuc970_clk_fixed("xin32k", 32768);
	clk[APLL]	= nuc970_clk_apll("apll", "xin", REG_CLK_APLLCON);
	clk[UPLL]	= nuc970_clk_upll("upll", "xin", REG_CLK_UPLLCON);
	clk[XIN128_DIV]	= nuc970_clk_fixed_factor("xin128_div", "xin", 1, 128);
	clk[SYS_MUX]	= nuc970_clk_mux("sys_mux", REG_CLK_DIV0, 3, 2,
					 sys_sel_clks,
					 ARRAY_SIZE(sys_sel_clks));
	clk[SYS_DIV]	= nuc970_clk_divider("sys_div", "sys_mux",
					     REG_CLK_DIV0, 0, 2);
	clk[DDR_GATE]	= nuc970_clk_gate("ddr_gate", "sys_div",
					  REG_CLK_HCLKEN, 10);
	/* CPU */
	clk[CPU_DIV]  = nuc970_clk_divider("cpu_div", "sys_div",
					   REG_CLK_DIV0, 16, 1);
	clk[CPU_GATE] = nuc970_clk_gate("cpu_gate", "cpu_div",
					REG_CLK_HCLKEN, 0);
	/*HCLK1 & PCLK*/
	clk[HCLK1_DIV]	= nuc970_clk_fixed_factor("hclk1_div", "cpu_div", 1, 2);
	clk[GDMA_GATE]	= nuc970_clk_gate("gdma_hclk_gate", "hclk1_div",
					  REG_CLK_HCLKEN, 12);
	clk[EBI_GATE]	= nuc970_clk_gate("ebi_hclk_gate", "hclk1_div",
					  REG_CLK_HCLKEN, 9);
	clk[TIC_GATE]	= nuc970_clk_gate("tic_hclk_gate", "hclk1_div",
					  REG_CLK_HCLKEN, 7);
	/* HCLK & HCLK234 */
	clk[HCLKN_DIV]		= nuc970_clk_fixed_factor("hclkn_div",
							  "sys_div", 1, 2);
	clk[DRAM_GATE]		= nuc970_clk_gate("dram_gate", "hclkn_div",
						  REG_CLK_HCLKEN, 10);
	clk[HCLK_GATE]		= nuc970_clk_gate("hclk_gate", "hclkn_div",
						  REG_CLK_HCLKEN, 1);
	clk[SRAM_GATE]		= nuc970_clk_gate("sram_gate", "hclk_gate",
						  REG_CLK_HCLKEN, 8);
	clk[HCLK234_DIV]	= nuc970_clk_divider("hclk234_div", "hclkn_div",
						     REG_CLK_DIV0, 20, 4);
	/* HCLK3 */
	clk[USBH_GATE]		= nuc970_clk_gate("usbh_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 18);
	clk[USBD_GATE]		= nuc970_clk_gate("usbd_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 19);
	clk[FMI_GATE]		= nuc970_clk_gate("fmi_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 20);
	clk[NAND_GATE]		= nuc970_clk_gate("nand_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 21);
	clk[EMMC_GATE]		= nuc970_clk_gate("emmc_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 22);
	clk[CRYPTO_GATE]	= nuc970_clk_gate("crypto_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 23);
	clk[EMAC1_GATE]		= nuc970_clk_gate("emac1_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 17);
	clk[EMAC1_ECLK_DIV]	= nuc970_clk_divider("emac1_eclk_div",
						     "hclk234_div",
						     REG_CLK_DIV8, 0, 8);
	clk[EMAC1_ECLK_GATE]	= nuc970_clk_gate("emac1_eclk_gate",
						  "emac1_eclk_div",
						  REG_CLK_HCLKEN, 17);
	clk[JPEG_GATE]		= nuc970_clk_gate("jpeg_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 29);
	clk[JPEG_ECLK_DIV]	= nuc970_clk_divider("jpeg_eclk_div",
						     "hclk234_div",
						     REG_CLK_DIV3, 28, 3);
	clk[JPEG_ECLK_GATE]	= nuc970_clk_gate("jpeg_eclk_gate",
						  "jpeg_eclk_div",
						  REG_CLK_HCLKEN, 29);
	clk[GE2D_GATE]		= nuc970_clk_gate("ge2d_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 28);
	clk[GE2D_ECLK_DIV]	= nuc970_clk_divider("ge2d_eclk_div",
						     "hclk234_div",
						     REG_CLK_DIV2, 28, 2);
	clk[GE2D_ECLK_GATE]	= nuc970_clk_gate("ge2d_eclk_gate",
						  "ge2d_eclk_div",
						  REG_CLK_HCLKEN, 28);
	/* HCLK4 */
	clk[SDH_GATE]		= nuc970_clk_gate("sdh_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 30);
	clk[AUDIO_GATE]		= nuc970_clk_gate("audio_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 24);
	clk[LCD_GATE]		= nuc970_clk_gate("lcd_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 25);
	clk[CAP_GATE]		= nuc970_clk_gate("cap_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 26);
	clk[SENSOR_GATE]	= nuc970_clk_gate("sensor_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 27);
	clk[EMAC0_GATE]		= nuc970_clk_gate("emac0_hclk_gate",
						  "hclk234_div",
						  REG_CLK_HCLKEN, 16);
	clk[EMAC0_ECLK_DIV]	= nuc970_clk_divider("emac0_eclk_div",
						     "hclk234_div",
						     REG_CLK_DIV8, 0, 8);
	clk[EMAC0_ECLK_GATE]	= nuc970_clk_gate("emac0_eclk_gate",
						  "emac0_eclk_div",
						  REG_CLK_HCLKEN, 16);
	/* ECLK */
	/* USB */
	clk[USB_APLLDIV]	= nuc970_clk_divider("usb_aplldiv", "apll",
						     REG_CLK_DIV2, 0, 3);
	clk[USB_UPLLDIV]	= nuc970_clk_divider("usb_uplldiv", "upll",
						     REG_CLK_DIV2, 0, 3);
	clk[USB_ECLK_MUX]	= nuc970_clk_mux("usb_eclk_mux", REG_CLK_DIV2,
						 3, 2, usb_sel_clks,
						 ARRAY_SIZE(usb_sel_clks));
	clk[USB_ECLK_DIV]	= nuc970_clk_divider("usb_eclk_div",
						     "usb_eclk_mux",
						     REG_CLK_DIV2, 8, 4);
	clk[USB_ECLK_GATE]	= nuc970_clk_gate("usb_eclk_gate",
						  "usb_eclk_div",
						  REG_CLK_HCLKEN, 18);
	/* SDH */
	clk[SDH_APLLDIV]	= nuc970_clk_divider("sdh_aplldiv", "apll",
						     REG_CLK_DIV9, 0, 3);
	clk[SDH_UPLLDIV]	= nuc970_clk_divider("sdh_uplldiv", "upll",
						     REG_CLK_DIV9, 0, 3);
	clk[SDH_ECLK_MUX]	= nuc970_clk_mux("sdh_eclk_mux", REG_CLK_DIV9,
						 3, 2, sdh_sel_clks,
						 ARRAY_SIZE(sdh_sel_clks));
	clk[SDH_ECLK_DIV]	= nuc970_clk_divider("sdh_eclk_div",
						     "sdh_eclk_mux",
						     REG_CLK_DIV9, 8, 8);
	clk[SDH_ECLK_GATE]	= nuc970_clk_gate("sdh_eclk_gate",
						  "sdh_eclk_div",
						  REG_CLK_HCLKEN, 30);
	/* EMMC */
	clk[EMMC_APLLDIV]	= nuc970_clk_divider("emmc_aplldiv", "apll",
						     REG_CLK_DIV3, 0, 3);
	clk[EMMC_UPLLDIV]	= nuc970_clk_divider("emmc_uplldiv", "upll",
						     REG_CLK_DIV3, 0, 3);
	clk[EMMC_ECLK_MUX]	= nuc970_clk_mux("emmc_eclk_mux", REG_CLK_DIV3,
						 3, 2, emmc_sel_clks,
						 ARRAY_SIZE(emmc_sel_clks));
	clk[EMMC_ECLK_DIV]	= nuc970_clk_divider("emmc_eclk_div",
						     "emmc_eclk_mux",
						     REG_CLK_DIV3, 8, 8);
	clk[EMMC_ECLK_GATE]	= nuc970_clk_gate("emmc_eclk_gate",
						  "emmc_eclk_div",
						  REG_CLK_HCLKEN, 22);
	/* ADC */
	clk[ADC_APLLDIV]	= nuc970_clk_divider("adc_aplldiv", "apll",
						     REG_CLK_DIV7, 16, 3);
	clk[ADC_UPLLDIV]	= nuc970_clk_divider("adc_uplldiv", "upll",
						     REG_CLK_DIV7, 16, 3);
	clk[ADC_ECLK_MUX]	= nuc970_clk_mux("adc_eclk_mux", REG_CLK_DIV7,
						 19, 2, adc_sel_clks,
						 ARRAY_SIZE(adc_sel_clks));
	clk[ADC_ECLK_DIV]	= nuc970_clk_divider("adc_eclk_div",
						     "adc_eclk_mux",
						     REG_CLK_DIV7, 24, 8);
	clk[ADC_ECLK_GATE]	= nuc970_clk_gate("adc_eclk_gate",
						  "adc_eclk_div",
						  REG_CLK_PCLKEN1, 24);
	/* LCD */
	clk[LCD_APLLDIV]	= nuc970_clk_divider("lcd_aplldiv", "apll",
						     REG_CLK_DIV1, 0, 3);
	clk[LCD_UPLLDIV]	= nuc970_clk_divider("lcd_uplldiv", "upll",
					     REG_CLK_DIV1, 0, 3);
	clk[LCD_ECLK_MUX]	= nuc970_clk_mux("lcd_eclk_mux", REG_CLK_DIV1,
						 3, 2, lcd_sel_clks,
						 ARRAY_SIZE(lcd_sel_clks));
	clk[LCD_ECLK_DIV]	= nuc970_clk_divider("lcd_eclk_div",
						     "lcd_eclk_mux",
						     REG_CLK_DIV1, 8, 8);
	clk[LCD_ECLK_GATE]	= nuc970_clk_gate("lcd_eclk_gate",
						  "lcd_eclk_div",
						  REG_CLK_HCLKEN, 25);
	/* AUDIO */
	clk[AUDIO_APLLDIV]	= nuc970_clk_divider("audio_aplldiv", "apll",
						     REG_CLK_DIV1, 16, 3);
	clk[AUDIO_UPLLDIV]	= nuc970_clk_divider("audio_uplldiv", "upll",
						     REG_CLK_DIV1, 16, 3);
	clk[AUDIO_ECLK_MUX]	= nuc970_clk_mux("audio_eclk_mux", REG_CLK_DIV1,
						 19, 2, audio_sel_clks,
						 ARRAY_SIZE(audio_sel_clks));
	clk[AUDIO_ECLK_DIV]	= nuc970_clk_divider("audio_eclk_div",
						     "audio_eclk_mux",
						     REG_CLK_DIV1, 24, 8);
	clk[AUDIO_ECLK_GATE]	= nuc970_clk_gate("audio_eclk_gate",
						  "audio_eclk_div",
						  REG_CLK_HCLKEN, 24);
	/* CAP */
	clk[CAP_APLLDIV]	= nuc970_clk_divider("cap_aplldiv", "apll",
						     REG_CLK_DIV3, 16, 3);
	clk[CAP_UPLLDIV]	= nuc970_clk_divider("cap_uplldiv", "upll",
						     REG_CLK_DIV3, 16, 3);
	clk[CAP_ECLK_MUX]	= nuc970_clk_mux("cap_eclk_mux", REG_CLK_DIV3,
						 19, 2, cap_sel_clks,
						 ARRAY_SIZE(cap_sel_clks));
	clk[CAP_ECLK_DIV]	= nuc970_clk_divider("cap_eclk_div",
						     "cap_eclk_mux",
						     REG_CLK_DIV3, 24, 4);
	clk[CAP_ECLK_GATE]	= nuc970_clk_gate("cap_eclk_gate",
						  "cap_eclk_div",
						  REG_CLK_HCLKEN, 26);
	/* UART0 */
	clk[UART0_APLLDIV]	= nuc970_clk_divider("uart0_aplldiv",
						     "apll", REG_CLK_DIV4,
						     0, 3);
	clk[UART0_UPLLDIV]	= nuc970_clk_divider("uart0_uplldiv", "upll",
						     REG_CLK_DIV4, 0, 3);
	clk[UART0_ECLK_MUX]	= nuc970_clk_mux("uart0_eclk_mux", REG_CLK_DIV4,
						 3, 2, uart0_sel_clks,
						 ARRAY_SIZE(uart0_sel_clks));
	clk[UART0_ECLK_DIV]	= nuc970_clk_divider("uart0_eclk_div",
						     "uart0_eclk_mux",
						     REG_CLK_DIV4, 5, 3);
	clk[UART0_ECLK_GATE]	= nuc970_clk_gate("uart0_eclk_gate",
						  "uart0_eclk_div",
						  REG_CLK_PCLKEN0, 16);
	/* UART1 */
	clk[UART1_APLLDIV]	= nuc970_clk_divider("uart1_aplldiv", "apll",
						     REG_CLK_DIV4, 8, 3);
	clk[UART1_UPLLDIV]	= nuc970_clk_divider("uart1_uplldiv", "upll",
						     REG_CLK_DIV4, 8, 3);
	clk[UART1_ECLK_MUX]	= nuc970_clk_mux("uart1_eclk_mux", REG_CLK_DIV4,
						 11, 2, uart1_sel_clks,
						 ARRAY_SIZE(uart1_sel_clks));
	clk[UART1_ECLK_DIV]	= nuc970_clk_divider("uart1_eclk_div",
						     "uart1_eclk_mux",
						     REG_CLK_DIV4, 13, 3);
	clk[UART1_ECLK_GATE]	= nuc970_clk_gate("uart1_eclk_gate",
						  "uart1_eclk_div",
						  REG_CLK_PCLKEN0, 17);
	/* UART2 */
	clk[UART2_APLLDIV]	= nuc970_clk_divider("uart2_aplldiv", "apll",
						     REG_CLK_DIV4, 16, 3);
	clk[UART2_UPLLDIV]	= nuc970_clk_divider("uart2_uplldiv", "upll",
						     REG_CLK_DIV4, 16, 3);
	clk[UART2_ECLK_MUX]	= nuc970_clk_mux("uart2_eclk_mux", REG_CLK_DIV4,
						 19, 2, uart2_sel_clks,
						 ARRAY_SIZE(uart2_sel_clks));
	clk[UART2_ECLK_DIV]	= nuc970_clk_divider("uart2_eclk_div",
						     "uart2_eclk_mux",
						     REG_CLK_DIV4, 21, 3);
	clk[UART2_ECLK_GATE]	= nuc970_clk_gate("uart2_eclk_gate",
						  "uart2_eclk_div",
						  REG_CLK_PCLKEN0, 18);
	/* UART3 */
	clk[UART3_APLLDIV]	= nuc970_clk_divider("uart3_aplldiv", "apll",
						     REG_CLK_DIV4, 24, 3);
	clk[UART3_UPLLDIV]	= nuc970_clk_divider("uart3_uplldiv", "upll",
						     REG_CLK_DIV4, 24, 3);
	clk[UART3_ECLK_MUX]	= nuc970_clk_mux("uart3_eclk_mux", REG_CLK_DIV4,
						 27, 2, uart3_sel_clks,
						 ARRAY_SIZE(uart3_sel_clks));
	clk[UART3_ECLK_DIV]	= nuc970_clk_divider("uart3_eclk_div",
						     "uart3_eclk_mux",
						     REG_CLK_DIV4, 29, 3);
	clk[UART3_ECLK_GATE]	= nuc970_clk_gate("uart3_eclk_gate",
						  "uart3_eclk_div",
						  REG_CLK_PCLKEN0, 19);
	/* UART4 */
	clk[UART4_APLLDIV]	= nuc970_clk_divider("uart4_aplldiv", "apll",
						     REG_CLK_DIV5, 0, 3);
	clk[UART4_UPLLDIV]	= nuc970_clk_divider("uart4_uplldiv", "upll",
						     REG_CLK_DIV5, 0, 3);
	clk[UART4_ECLK_MUX]	= nuc970_clk_mux("uart4_eclk_mux", REG_CLK_DIV5,
						 3, 2, uart4_sel_clks,
						 ARRAY_SIZE(uart4_sel_clks));
	clk[UART4_ECLK_DIV]	= nuc970_clk_divider("uart4_eclk_div",
						     "uart4_eclk_mux",
						     REG_CLK_DIV5, 5, 3);
	clk[UART4_ECLK_GATE]	= nuc970_clk_gate("uart4_eclk_gate",
						  "uart4_eclk_div",
						  REG_CLK_PCLKEN0, 20);
	/* UART5 */
	clk[UART5_APLLDIV]	= nuc970_clk_divider("uart5_aplldiv", "apll",
						     REG_CLK_DIV5, 8, 3);
	clk[UART5_UPLLDIV]	= nuc970_clk_divider("uart5_uplldiv", "upll",
						     REG_CLK_DIV5, 8, 3);
	clk[UART5_ECLK_MUX]	= nuc970_clk_mux("uart5_eclk_mux", REG_CLK_DIV5,
						 11, 2, uart5_sel_clks,
						 ARRAY_SIZE(uart5_sel_clks));
	clk[UART5_ECLK_DIV]	= nuc970_clk_divider("uart5_eclk_div",
						     "uart5_eclk_mux",
						     REG_CLK_DIV5, 13, 3);
	clk[UART5_ECLK_GATE]	= nuc970_clk_gate("uart5_eclk_gate",
						  "uart5_eclk_div",
						  REG_CLK_PCLKEN0, 21);
	/* UART6 */
	clk[UART6_APLLDIV]	= nuc970_clk_divider("uart6_aplldiv", "apll",
						     REG_CLK_DIV5, 16, 3);
	clk[UART6_UPLLDIV]	= nuc970_clk_divider("uart6_uplldiv", "upll",
						     REG_CLK_DIV5, 16, 3);
	clk[UART6_ECLK_MUX]	= nuc970_clk_mux("uart6_eclk_mux", REG_CLK_DIV5,
						 19, 2, uart6_sel_clks,
						 ARRAY_SIZE(uart6_sel_clks));
	clk[UART6_ECLK_DIV]	= nuc970_clk_divider("uart6_eclk_div",
						     "uart6_eclk_mux",
						     REG_CLK_DIV5, 21, 3);
	clk[UART6_ECLK_GATE]	= nuc970_clk_gate("uart6_eclk_gate",
						  "uart6_eclk_div",
						  REG_CLK_PCLKEN0, 22);
	/* UART7 */
	clk[UART7_APLLDIV]	= nuc970_clk_divider("uart7_aplldiv", "apll",
						     REG_CLK_DIV5, 24, 3);
	clk[UART7_UPLLDIV]	= nuc970_clk_divider("uart7_uplldiv", "upll",
						     REG_CLK_DIV5, 24, 3);
	clk[UART7_ECLK_MUX]	= nuc970_clk_mux("uart7_eclk_mux", REG_CLK_DIV5,
						 27, 2, uart7_sel_clks,
						 ARRAY_SIZE(uart7_sel_clks));
	clk[UART7_ECLK_DIV]	= nuc970_clk_divider("uart7_eclk_div",
						     "uart7_eclk_mux",
						     REG_CLK_DIV5, 29, 3);
	clk[UART7_ECLK_GATE]	= nuc970_clk_gate("uart7_eclk_gate",
						  "uart7_eclk_div",
						  REG_CLK_PCLKEN0, 23);
	/* UART8 */
	clk[UART8_APLLDIV]	= nuc970_clk_divider("uart8_aplldiv", "apll",
						     REG_CLK_DIV6, 0, 3);
	clk[UART8_UPLLDIV]	= nuc970_clk_divider("uart8_uplldiv", "upll",
						     REG_CLK_DIV6, 0, 3);
	clk[UART8_ECLK_MUX]	= nuc970_clk_mux("uart8_eclk_mux", REG_CLK_DIV6,
						 3, 2, uart8_sel_clks,
						 ARRAY_SIZE(uart8_sel_clks));
	clk[UART8_ECLK_DIV]	= nuc970_clk_divider("uart8_eclk_div",
						     "uart8_eclk_mux",
						     REG_CLK_DIV6, 5, 3);
	clk[UART8_ECLK_GATE]	= nuc970_clk_gate("uart8_eclk_gate",
						  "uart8_eclk_div",
						  REG_CLK_PCLKEN0, 24);
	/* UART9 */
	clk[UART9_APLLDIV]	= nuc970_clk_divider("uart9_aplldiv", "apll",
						     REG_CLK_DIV6, 8, 3);
	clk[UART9_UPLLDIV]	= nuc970_clk_divider("uart9_uplldiv", "upll",
						     REG_CLK_DIV6, 8, 3);
	clk[UART9_ECLK_MUX]	= nuc970_clk_mux("uart9_eclk_mux", REG_CLK_DIV6,
						 11, 2, uart9_sel_clks,
						 ARRAY_SIZE(uart9_sel_clks));
	clk[UART9_ECLK_DIV]	= nuc970_clk_divider("uart9_eclk_div",
						     "uart9_eclk_mux",
						     REG_CLK_DIV6, 13, 3);
	clk[UART9_ECLK_GATE]	= nuc970_clk_gate("uart9_eclk_gate",
						  "uart9_eclk_div",
						  REG_CLK_PCLKEN0, 25);
	/* UART10 */
	clk[UART10_APLLDIV]	 = nuc970_clk_divider("uart10_aplldiv", "apll",
						      REG_CLK_DIV6, 16, 3);
	clk[UART10_UPLLDIV]	 = nuc970_clk_divider("uart10_uplldiv", "upll",
						      REG_CLK_DIV6, 16, 3);
	clk[UART10_ECLK_MUX]	= nuc970_clk_mux("uart10_eclk_mux",
						 REG_CLK_DIV6, 19, 2,
						 uart10_sel_clks,
						 ARRAY_SIZE(uart10_sel_clks));
	clk[UART10_ECLK_DIV]	= nuc970_clk_divider("uart10_eclk_div",
						     "uart10_eclk_mux",
						     REG_CLK_DIV6, 21, 3);
	clk[UART10_ECLK_GATE]	= nuc970_clk_gate("uart10_eclk_gate",
						  "uart10_eclk_div",
						  REG_CLK_PCLKEN0, 26);
	/* SYSTEM */
	clk[SYSTEM_APLLDIV]	= nuc970_clk_divider("system_aplldiv", "apll",
						     REG_CLK_DIV0, 0, 3);
	clk[SYSTEM_UPLLDIV]	= nuc970_clk_divider("system_uplldiv", "upll",
						     REG_CLK_DIV0, 0, 3);
	clk[SYSTEM_ECLK_MUX]	= nuc970_clk_mux("system_eclk_mux",
						 REG_CLK_DIV0, 3, 2,
						 system_sel_clks,
						 ARRAY_SIZE(system_sel_clks));
	clk[SYSTEM_ECLK_DIV]	= nuc970_clk_divider("system_eclk_div",
						     "system_eclk_mux",
						     REG_CLK_DIV0, 8, 4);
	/* GPIO */
	clk[GPIO_ECLK_MUX]	= nuc970_clk_mux("gpio_eclk_mux", REG_CLK_DIV7,
						 7, 1, gpio_sel_clks,
						 ARRAY_SIZE(gpio_sel_clks));
	clk[GPIO_ECLK_DIV]	= nuc970_clk_divider("gpio_eclk_div",
						     "gpio_eclk_mux",
						     REG_CLK_DIV7, 0, 7);
	clk[GPIO_ECLK_GATE]	= nuc970_clk_gate("gpio_eclk_gate",
						  "gpio_eclk_div",
						  REG_CLK_PCLKEN0, 3);
	/* KPI */
	clk[KPI_ECLK_MUX]	= nuc970_clk_mux("kpi_eclk_mux", REG_CLK_DIV7,
						 15, 1, kpi_sel_clks,
						 ARRAY_SIZE(kpi_sel_clks));
	clk[KPI_ECLK_DIV]	= nuc970_clk_divider("kpi_eclk_div",
						     "kpi_eclk_mux",
						     REG_CLK_DIV7, 8, 7);
	clk[KPI_ECLK_GATE]	= nuc970_clk_gate("kpi_eclk_gate",
						  "kpi_eclk_div",
						  REG_CLK_PCLKEN1, 25);
	/* ETIMER0 */
	clk[ETIMER0_ECLK_MUX]	= nuc970_clk_mux("etimer0_eclk_mux",
						 REG_CLK_DIV8, 16, 2,
						 etimer_sel_clks,
						 ARRAY_SIZE(etimer_sel_clks));
	clk[ETIMER0_ECLK_GATE]	= nuc970_clk_gate("etimer0_eclk_gate",
						  "etimer0_eclk_mux",
						  REG_CLK_PCLKEN0, 4);
	/* ETIMER1 */
	clk[ETIMER1_ECLK_MUX]	= nuc970_clk_mux("etimer1_eclk_mux",
						 REG_CLK_DIV8, 18, 2,
						 etimer_sel_clks,
						 ARRAY_SIZE(etimer_sel_clks));
	clk[ETIMER1_ECLK_GATE]	= nuc970_clk_gate("etimer1_eclk_gate",
						  "etimer1_eclk_mux",
						  REG_CLK_PCLKEN0, 5);
	/* ETIMER2 */
	clk[ETIMER2_ECLK_MUX]	= nuc970_clk_mux("etimer2_eclk_mux",
						 REG_CLK_DIV8, 20, 2,
						 etimer_sel_clks,
						 ARRAY_SIZE(etimer_sel_clks));
	clk[ETIMER2_ECLK_GATE]	= nuc970_clk_gate("etimer2_eclk_gate",
						  "etimer2_eclk_mux",
						  REG_CLK_PCLKEN0, 6);
	/* ETIMER3 */
	clk[ETIMER3_ECLK_MUX]	= nuc970_clk_mux("etimer3_eclk_mux",
						 REG_CLK_DIV8, 22, 2,
						 etimer_sel_clks,
						 ARRAY_SIZE(etimer_sel_clks));
	clk[ETIMER3_ECLK_GATE]	= nuc970_clk_gate("etimer3_eclk_gate",
						  "etimer3_eclk_mux",
						  REG_CLK_PCLKEN0, 7);
	/* WWDT */
	clk[WWDT_ECLK_MUX]	= nuc970_clk_mux("wwdt_eclk_mux", REG_CLK_DIV8,
						 10, 2, wwdt_sel_clks,
						 ARRAY_SIZE(wwdt_sel_clks));
	clk[WWDT_ECLK_GATE]	= nuc970_clk_gate("wwdt_eclk_gate",
						  "wwdt_eclk_mux",
						  REG_CLK_PCLKEN0, 1);
	/* WDT */
	clk[WDT_ECLK_MUX]	= nuc970_clk_mux("wdt_eclk_mux", REG_CLK_DIV8,
						 8, 2, wwdt_sel_clks,
						 ARRAY_SIZE(wwdt_sel_clks));
	clk[WDT_ECLK_GATE]	= nuc970_clk_gate("wdt_eclk_gate",
						  "wdt_eclk_mux",
						  REG_CLK_PCLKEN0, 0);
	/* SMARTCARD */
	clk[SMC0_ECLK_DIV]	= nuc970_clk_divider("smc0_eclk_div", "xin",
						     REG_CLK_DIV6, 24, 4);
	clk[SMC0_ECLK_GATE]	= nuc970_clk_gate("smc0_eclk_gate",
						  "smc0_eclk_div",
						  REG_CLK_PCLKEN1, 12);
	clk[SMC1_ECLK_DIV]	= nuc970_clk_divider("smc1_eclk_div", "xin",
						     REG_CLK_DIV6, 28, 4);
	clk[SMC1_ECLK_GATE]	= nuc970_clk_gate("smc1_eclk_gate",
						  "smc1_eclk_div",
						  REG_CLK_PCLKEN1, 13);
	/* PCLK */
	clk[PCLK_DIV]		= nuc970_clk_divider("pclk_div", "hclk1_div",
						     REG_CLK_DIV0, 24, 4);
	clk[PCLK4096_DIV]	= nuc970_clk_fixed_factor("pclk4096_div",
							  "pclk_div", 1, 4096);
	clk[I2C0_GATE]		= nuc970_clk_gate("i2c0_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 0);
	clk[I2C1_GATE]		= nuc970_clk_gate("i2c1_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 1);
	clk[SPI0_GATE]		= nuc970_clk_gate("spi0_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 4);
	clk[SPI1_GATE]		= nuc970_clk_gate("spi1_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 5);
	clk[UART0_GATE]		= nuc970_clk_gate("uart0_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 16);
	clk[UART1_GATE]		= nuc970_clk_gate("uart1_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 17);
	clk[UART2_GATE]		= nuc970_clk_gate("uart2_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 18);
	clk[UART3_GATE]		= nuc970_clk_gate("uart3_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 19);
	clk[UART4_GATE]		= nuc970_clk_gate("uart4_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 20);
	clk[UART5_GATE]		= nuc970_clk_gate("uart5_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 21);
	clk[UART6_GATE]		= nuc970_clk_gate("uart6_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 22);
	clk[UART7_GATE]		= nuc970_clk_gate("uart7_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 23);
	clk[UART8_GATE]		= nuc970_clk_gate("uart8_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 24);
	clk[UART9_GATE]		= nuc970_clk_gate("uart9_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 25);
	clk[UART10_GATE]	= nuc970_clk_gate("uart10_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 26);
	clk[WDT_GATE]		= nuc970_clk_gate("wdt_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 0);
	clk[WWDT_GATE]		= nuc970_clk_gate("wwdt_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 1);
	clk[RTC_GATE]		= nuc970_clk_gate("rtc_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 2);
	clk[GPIO_GATE]		= nuc970_clk_gate("gpio_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 3);
	clk[ADC_GATE]		= nuc970_clk_gate("adc_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 24);
	clk[KPI_GATE]		= nuc970_clk_gate("kpi_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 25);
	clk[MTPC_GATE]		= nuc970_clk_gate("mtpc_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 26);
	clk[PWM_GATE]		= nuc970_clk_gate("pwm_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 27);
	clk[ETIMER0_GATE]	= nuc970_clk_gate("etimer0_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 4);
	clk[ETIMER1_GATE]	= nuc970_clk_gate("etimer1_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 5);
	clk[ETIMER2_GATE]	= nuc970_clk_gate("etimer2_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 6);
	clk[ETIMER3_GATE]	= nuc970_clk_gate("etimer3_gate", "pclk_div",
						  REG_CLK_PCLKEN0, 7);
	clk[CAN0_GATE]		= nuc970_clk_gate("can0_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 8);
	clk[CAN1_GATE]		= nuc970_clk_gate("can1_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 9);
	clk[TIMER0_GATE]	= nuc970_clk_gate("timer0_gate", "xin",
						  REG_CLK_PCLKEN0, 8);
	clk[TIMER1_GATE]	= nuc970_clk_gate("timer1_gate", "xin",
						  REG_CLK_PCLKEN0, 9);
	clk[TIMER2_GATE]	= nuc970_clk_gate("timer2_gate", "xin",
						  REG_CLK_PCLKEN0, 10);
	clk[TIMER3_GATE]	= nuc970_clk_gate("timer3_gate", "xin",
						  REG_CLK_PCLKEN0, 11);
	clk[TIMER4_GATE]	= nuc970_clk_gate("timer4_gate", "xin",
						  REG_CLK_PCLKEN0, 12);
	clk[SMC0_GATE]		= nuc970_clk_gate("smc0_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 12);
	clk[SMC1_GATE]		= nuc970_clk_gate("smc1_gate", "pclk_div",
						  REG_CLK_PCLKEN1, 13);

	for (i = 0; i < ARRAY_SIZE(clk); i++)
		if (IS_ERR(clk[i]))
			pr_err("nuc970 clk %d: register failed with %ld\n",
				i, PTR_ERR(clk[i]));

	clk_data.clks = clk;
	clk_data.clk_num = ARRAY_SIZE(clk);
	of_clk_add_provider(np, of_clk_src_onecell_get, &clk_data);

	/* Register clock device */
	clk_register_clkdev(clk[TIMER0_GATE], "timer0", NULL);
	clk_register_clkdev(clk[TIMER1_GATE], "timer1", NULL);
	clk_register_clkdev(clk[PCLK4096_DIV], "pclk4096_div", NULL);
	clk_register_clkdev(clk[XIN], "xin", NULL);
	clk_register_clkdev(clk[XIN32K], "xin32k", NULL);
	clk_register_clkdev(clk[APLL], "apll", NULL);
	clk_register_clkdev(clk[UPLL], "upll", NULL);
	clk_register_clkdev(clk[SYS_MUX], "sysmux", NULL);
	clk_register_clkdev(clk[SYS_DIV], "sysdiv", NULL);
	clk_register_clkdev(clk[XIN128_DIV], "xin128div", NULL);
	/* CPU */
	clk_register_clkdev(clk[CPU_DIV], "cpudiv", NULL);
	clk_register_clkdev(clk[CPU_GATE], "cpu", NULL);
	/* HCLK1 */
	clk_register_clkdev(clk[HCLK_GATE], "hclk", NULL);
	clk_register_clkdev(clk[SRAM_GATE], "sram", NULL);
	clk_register_clkdev(clk[HCLK1_DIV], "hclk1div", NULL);
	clk_register_clkdev(clk[DDR_GATE], "ddr_hclk", NULL);
	clk_register_clkdev(clk[GDMA_GATE], "gdma_hclk", NULL);
	clk_register_clkdev(clk[EBI_GATE], "ebi_hclk", NULL);
	clk_register_clkdev(clk[TIC_GATE], "tic_hclk", NULL);
	/* HCLK234 */
	clk_register_clkdev(clk[HCLKN_DIV], "hclkndiv", NULL);
	clk_register_clkdev(clk[DRAM_GATE], "dram", NULL);
	clk_register_clkdev(clk[HCLK234_DIV], "hclk234div", NULL);
	/* HCLK3 */
	clk_register_clkdev(clk[USBH_GATE], "usbh_hclk", NULL);
	clk_register_clkdev(clk[EMAC1_GATE], "emac1_hclk", NULL);
	clk_register_clkdev(clk[EMAC1_ECLK_DIV], "emac1_eclk_div", NULL);
	clk_register_clkdev(clk[EMAC1_ECLK_GATE], "emac1_eclk", NULL);
	clk_register_clkdev(clk[USBD_GATE], "usbd_hclk", NULL);
	clk_register_clkdev(clk[FMI_GATE], "fmi_hclk", NULL);
	clk_register_clkdev(clk[NAND_GATE], "nand_hclk", NULL);
	clk_register_clkdev(clk[EMMC_GATE], "emmc_hclk", NULL);
	clk_register_clkdev(clk[CRYPTO_GATE], "crypto_hclk", NULL);
	clk_register_clkdev(clk[JPEG_GATE], "jpeg_hclk", NULL);
	clk_register_clkdev(clk[JPEG_ECLK_DIV], "jpeg_eclk_div", NULL);
	clk_register_clkdev(clk[JPEG_ECLK_GATE], "jpeg_eclk", NULL);
	clk_register_clkdev(clk[GE2D_GATE], "ge2d_hclk", NULL);
	clk_register_clkdev(clk[GE2D_ECLK_DIV], "ge2d_eclk_div", NULL);
	clk_register_clkdev(clk[GE2D_ECLK_GATE], "ge2d_eclk", NULL);
	/* HCLK4 */
	clk_register_clkdev(clk[EMAC0_GATE], "emac0_hclk", NULL);
	clk_register_clkdev(clk[EMAC0_ECLK_DIV], "emac0_eclk_div", NULL);
	clk_register_clkdev(clk[EMAC0_ECLK_GATE], "emac0_eclk", NULL);
	clk_register_clkdev(clk[SDH_GATE], "sdh_hclk", NULL);
	clk_register_clkdev(clk[AUDIO_GATE], "audio_hclk", NULL);
	clk_register_clkdev(clk[LCD_GATE], "lcd_hclk", NULL);
	clk_register_clkdev(clk[SENSOR_GATE], "sensor_hclk", NULL);
	clk_register_clkdev(clk[CAP_GATE], "cap_hclk", NULL);
	/* ECLK */
	clk_register_clkdev(clk[LCD_APLLDIV], "lcd_aplldiv", NULL);
	clk_register_clkdev(clk[LCD_UPLLDIV], "lcd_uplldiv", NULL);
	clk_register_clkdev(clk[LCD_ECLK_MUX], "lcd_eclk_mux", NULL);
	clk_register_clkdev(clk[LCD_ECLK_DIV], "lcd_eclk_div", NULL);
	clk_register_clkdev(clk[LCD_ECLK_GATE], "lcd_eclk", NULL);
	clk_register_clkdev(clk[AUDIO_APLLDIV], "audio_aplldiv", NULL);
	clk_register_clkdev(clk[AUDIO_UPLLDIV], "audio_uplldiv", NULL);
	clk_register_clkdev(clk[AUDIO_ECLK_MUX], "audio_eclk_mux", NULL);
	clk_register_clkdev(clk[AUDIO_ECLK_DIV], "audio_eclk_div", NULL);
	clk_register_clkdev(clk[AUDIO_ECLK_GATE], "audio_eclk", NULL);
	clk_register_clkdev(clk[USB_APLLDIV], "usb_aplldiv", NULL);
	clk_register_clkdev(clk[USB_UPLLDIV], "usb_uplldiv", NULL);
	clk_register_clkdev(clk[USB_ECLK_MUX], "usb_eclk_mux", NULL);
	clk_register_clkdev(clk[USB_ECLK_DIV], "usb_eclk_div", NULL);
	clk_register_clkdev(clk[USB_ECLK_GATE], "usb_eclk", NULL);
	clk_register_clkdev(clk[SDH_APLLDIV], "sdh_aplldiv", NULL);
	clk_register_clkdev(clk[SDH_UPLLDIV], "sdh_uplldiv", NULL);
	clk_register_clkdev(clk[SDH_ECLK_MUX], "sdh_eclk_mux", NULL);
	clk_register_clkdev(clk[SDH_ECLK_DIV], "sdh_eclk_div", NULL);
	clk_register_clkdev(clk[SDH_ECLK_GATE], "sdh_eclk", NULL);
	clk_register_clkdev(clk[EMMC_APLLDIV], "emmc_aplldiv", NULL);
	clk_register_clkdev(clk[EMMC_UPLLDIV], "emmc_uplldiv", NULL);
	clk_register_clkdev(clk[EMMC_ECLK_MUX], "emmc_eclk_mux", NULL);
	clk_register_clkdev(clk[EMMC_ECLK_DIV], "emmc_eclk_div", NULL);
	clk_register_clkdev(clk[EMMC_ECLK_GATE], "emmc_eclk", NULL);
	clk_register_clkdev(clk[ADC_APLLDIV], "adc_aplldiv", NULL);
	clk_register_clkdev(clk[ADC_UPLLDIV], "adc_uplldiv", NULL);
	clk_register_clkdev(clk[ADC_ECLK_MUX], "adc_eclk_mux", NULL);
	clk_register_clkdev(clk[ADC_ECLK_DIV], "adc_eclk_div", NULL);
	clk_register_clkdev(clk[ADC_ECLK_GATE], "adc_eclk", NULL);
	clk_register_clkdev(clk[CAP_APLLDIV], "cap_aplldiv", NULL);
	clk_register_clkdev(clk[CAP_UPLLDIV], "cap_uplldiv", NULL);
	clk_register_clkdev(clk[CAP_ECLK_MUX], "cap_eclk_mux", NULL);
	clk_register_clkdev(clk[CAP_ECLK_DIV], "cap_eclk_div", NULL);
	clk_register_clkdev(clk[CAP_ECLK_GATE], "cap_eclk", NULL);
	clk_register_clkdev(clk[UART0_APLLDIV], "uart0_aplldiv", NULL);
	clk_register_clkdev(clk[UART0_UPLLDIV], "uart0_uplldiv", NULL);
	clk_register_clkdev(clk[UART0_ECLK_MUX], "uart0_eclk_mux", NULL);
	clk_register_clkdev(clk[UART0_ECLK_DIV], "uart0_eclk_div", NULL);
	clk_register_clkdev(clk[UART0_ECLK_GATE], "uart0_eclk", NULL);
	clk_register_clkdev(clk[UART1_APLLDIV], "uart1_aplldiv", NULL);
	clk_register_clkdev(clk[UART1_UPLLDIV], "uart1_uplldiv", NULL);
	clk_register_clkdev(clk[UART1_ECLK_MUX], "uart1_eclk_mux", NULL);
	clk_register_clkdev(clk[UART1_ECLK_DIV], "uart1_eclk_div", NULL);
	clk_register_clkdev(clk[UART1_ECLK_GATE], "uart1_eclk", NULL);
	clk_register_clkdev(clk[UART2_APLLDIV], "uart2_aplldiv", NULL);
	clk_register_clkdev(clk[UART2_UPLLDIV], "uart2_uplldiv", NULL);
	clk_register_clkdev(clk[UART2_ECLK_MUX], "uart2_eclk_mux", NULL);
	clk_register_clkdev(clk[UART2_ECLK_DIV], "uart2_eclk_div", NULL);
	clk_register_clkdev(clk[UART2_ECLK_GATE], "uart2_eclk", NULL);
	clk_register_clkdev(clk[UART3_APLLDIV], "uart3_aplldiv", NULL);
	clk_register_clkdev(clk[UART3_UPLLDIV], "uart3_uplldiv", NULL);
	clk_register_clkdev(clk[UART3_ECLK_MUX], "uart3_eclk_mux", NULL);
	clk_register_clkdev(clk[UART3_ECLK_DIV], "uart3_eclk_div", NULL);
	clk_register_clkdev(clk[UART3_ECLK_GATE], "uart3_eclk", NULL);
	clk_register_clkdev(clk[UART4_APLLDIV], "uart4_aplldiv", NULL);
	clk_register_clkdev(clk[UART4_UPLLDIV], "uart4_uplldiv", NULL);
	clk_register_clkdev(clk[UART4_ECLK_MUX], "uart4_eclk_mux", NULL);
	clk_register_clkdev(clk[UART4_ECLK_DIV], "uart4_eclk_div", NULL);
	clk_register_clkdev(clk[UART4_ECLK_GATE], "uart4_eclk", NULL);
	clk_register_clkdev(clk[UART5_APLLDIV], "uart5_aplldiv", NULL);
	clk_register_clkdev(clk[UART5_UPLLDIV], "uart5_uplldiv", NULL);
	clk_register_clkdev(clk[UART5_ECLK_MUX], "uart5_eclk_mux", NULL);
	clk_register_clkdev(clk[UART5_ECLK_DIV], "uart5_eclk_div", NULL);
	clk_register_clkdev(clk[UART5_ECLK_GATE], "uart5_eclk", NULL);
	clk_register_clkdev(clk[UART6_APLLDIV], "uart6_aplldiv", NULL);
	clk_register_clkdev(clk[UART6_UPLLDIV], "uart6_uplldiv", NULL);
	clk_register_clkdev(clk[UART6_ECLK_MUX], "uart6_eclk_mux", NULL);
	clk_register_clkdev(clk[UART6_ECLK_DIV], "uart6_eclk_div", NULL);
	clk_register_clkdev(clk[UART6_ECLK_GATE], "uart6_eclk", NULL);
	clk_register_clkdev(clk[UART7_APLLDIV], "uart7_aplldiv", NULL);
	clk_register_clkdev(clk[UART7_UPLLDIV], "uart7_uplldiv", NULL);
	clk_register_clkdev(clk[UART7_ECLK_MUX], "uart7_eclk_mux", NULL);
	clk_register_clkdev(clk[UART7_ECLK_DIV], "uart7_eclk_div", NULL);
	clk_register_clkdev(clk[UART7_ECLK_GATE], "uart7_eclk", NULL);
	clk_register_clkdev(clk[UART8_APLLDIV], "uart8_aplldiv", NULL);
	clk_register_clkdev(clk[UART8_UPLLDIV], "uart8_uplldiv", NULL);
	clk_register_clkdev(clk[UART8_ECLK_MUX], "uart8_eclk_mux", NULL);
	clk_register_clkdev(clk[UART8_ECLK_DIV], "uart8_eclk_div", NULL);
	clk_register_clkdev(clk[UART8_ECLK_GATE], "uart8_eclk", NULL);
	clk_register_clkdev(clk[UART9_APLLDIV], "uart9_aplldiv", NULL);
	clk_register_clkdev(clk[UART9_UPLLDIV], "uart9_uplldiv", NULL);
	clk_register_clkdev(clk[UART9_ECLK_MUX], "uart9_eclk_mux", NULL);
	clk_register_clkdev(clk[UART9_ECLK_DIV], "uart9_eclk_div", NULL);
	clk_register_clkdev(clk[UART9_ECLK_GATE], "uart9_eclk", NULL);
	clk_register_clkdev(clk[UART10_APLLDIV], "uart10_aplldiv", NULL);
	clk_register_clkdev(clk[UART10_UPLLDIV], "uart10_uplldiv", NULL);
	clk_register_clkdev(clk[UART10_ECLK_MUX], "uart10_eclk_mux", NULL);
	clk_register_clkdev(clk[UART10_ECLK_DIV], "uart10_eclk_div", NULL);
	clk_register_clkdev(clk[UART10_ECLK_GATE], "uart10_eclk", NULL);
	clk_register_clkdev(clk[SYSTEM_APLLDIV], "system_aplldiv", NULL);
	clk_register_clkdev(clk[SYSTEM_UPLLDIV], "system_uplldiv", NULL);
	clk_register_clkdev(clk[SYSTEM_ECLK_MUX], "system_eclk_mux", NULL);
	clk_register_clkdev(clk[SYSTEM_ECLK_DIV], "system_eclk_div", NULL);
	clk_register_clkdev(clk[SYSTEM_ECLK_GATE], "system_eclk", NULL);
	clk_register_clkdev(clk[GPIO_ECLK_MUX], "gpio_eclk_mux", NULL);
	clk_register_clkdev(clk[GPIO_ECLK_DIV], "gpio_eclk_div", NULL);
	clk_register_clkdev(clk[GPIO_ECLK_GATE], "gpio_eclk", NULL);
	clk_register_clkdev(clk[KPI_ECLK_MUX], "kpi_eclk_mux", NULL);
	clk_register_clkdev(clk[KPI_ECLK_DIV], "kpi_eclk_div", NULL);
	clk_register_clkdev(clk[KPI_ECLK_GATE], "kpi_eclk", NULL);
	clk_register_clkdev(clk[ETIMER0_ECLK_MUX], "etmr0_eclk_mux", NULL);
	clk_register_clkdev(clk[ETIMER0_ECLK_GATE], "etmr0_eclk", NULL);
	clk_register_clkdev(clk[ETIMER1_ECLK_MUX], "etmr1_eclk_mux", NULL);
	clk_register_clkdev(clk[ETIMER1_ECLK_GATE], "etmr1_eclk", NULL);
	clk_register_clkdev(clk[ETIMER2_ECLK_MUX], "etmr2_eclk_mux", NULL);
	clk_register_clkdev(clk[ETIMER2_ECLK_GATE], "etmr2_eclk", NULL);
	clk_register_clkdev(clk[ETIMER3_ECLK_MUX], "etmr3_eclk_mux", NULL);
	clk_register_clkdev(clk[ETIMER3_ECLK_GATE], "etmr3_eclk", NULL);
	clk_register_clkdev(clk[WWDT_ECLK_MUX], "wwdt_eclk_mux", NULL);
	clk_register_clkdev(clk[WWDT_ECLK_GATE], "wwdt_eclk", NULL);
	clk_register_clkdev(clk[WDT_ECLK_MUX], "wdt_eclk_mux", NULL);
	clk_register_clkdev(clk[WDT_ECLK_GATE], "wdt_eclk", NULL);
	clk_register_clkdev(clk[SMC0_ECLK_DIV], "smc0_eclk_div", NULL);
	clk_register_clkdev(clk[SMC0_ECLK_GATE], "smc0_eclk", NULL);
	clk_register_clkdev(clk[SMC1_ECLK_DIV], "smc1_eclk_div", NULL);
	clk_register_clkdev(clk[SMC1_ECLK_GATE], "smc1_eclk", NULL);
	/* PCLK */
	clk_register_clkdev(clk[PCLK_DIV], "pclkdiv", NULL);
	clk_register_clkdev(clk[RTC_GATE], "rtc", NULL);
	clk_register_clkdev(clk[I2C0_GATE], "i2c0", NULL);
	clk_register_clkdev(clk[I2C1_GATE], "i2c1", NULL);
	clk_register_clkdev(clk[SPI0_GATE], "spi0", NULL);
	clk_register_clkdev(clk[SPI1_GATE], "spi1", NULL);
	clk_register_clkdev(clk[UART0_GATE], "uart0", NULL);
	clk_register_clkdev(clk[UART1_GATE], "uart1", NULL);
	clk_register_clkdev(clk[UART2_GATE], "uart2", NULL);
	clk_register_clkdev(clk[UART3_GATE], "uart3", NULL);
	clk_register_clkdev(clk[UART4_GATE], "uart4", NULL);
	clk_register_clkdev(clk[UART5_GATE], "uart5", NULL);
	clk_register_clkdev(clk[UART6_GATE], "uart6", NULL);
	clk_register_clkdev(clk[UART7_GATE], "uart7", NULL);
	clk_register_clkdev(clk[UART8_GATE], "uart8", NULL);
	clk_register_clkdev(clk[UART9_GATE], "uart9", NULL);
	clk_register_clkdev(clk[UART10_GATE], "uart10", NULL);
	clk_register_clkdev(clk[WDT_GATE], "wdt", NULL);
	clk_register_clkdev(clk[WWDT_GATE], "wwdt", NULL);
	clk_register_clkdev(clk[GPIO_GATE], "gpio", NULL);
	clk_register_clkdev(clk[SMC0_GATE], "smc0", NULL);
	clk_register_clkdev(clk[SMC1_GATE], "smc1", NULL);
	clk_register_clkdev(clk[ADC_GATE], "adc", NULL);
	clk_register_clkdev(clk[KPI_GATE], "kpi", NULL);
	clk_register_clkdev(clk[MTPC_GATE], "mtpc", NULL);
	clk_register_clkdev(clk[PWM_GATE], "pwm", NULL);
	clk_register_clkdev(clk[ETIMER0_GATE], "etimer0", NULL);
	clk_register_clkdev(clk[ETIMER1_GATE], "etimer1", NULL);
	clk_register_clkdev(clk[ETIMER2_GATE], "etimer2", NULL);
	clk_register_clkdev(clk[ETIMER3_GATE], "etimer3", NULL);
	clk_register_clkdev(clk[TIMER2_GATE], "timer2", NULL);
	clk_register_clkdev(clk[TIMER3_GATE], "timer3", NULL);
	clk_register_clkdev(clk[TIMER4_GATE], "timer4", NULL);
	clk_register_clkdev(clk[CAN0_GATE], "can0", NULL);
	clk_register_clkdev(clk[CAN1_GATE], "can1", NULL);

	/* enable some important clocks */
	clk_prepare_enable(clk_get(NULL, "cpu"));
	clk_prepare_enable(clk_get(NULL, "hclk"));
	clk_prepare_enable(clk_get(NULL, "sram"));
	clk_prepare_enable(clk_get(NULL, "dram"));
	clk_prepare_enable(clk_get(NULL, "ddr_hclk"));
}

CLK_OF_DECLARE(nuc970_clk, "nuvoton,clk", nuc970_clocks_init);
