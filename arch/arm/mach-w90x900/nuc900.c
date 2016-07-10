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

#include <linux/platform_device.h>
#include <linux/clk.h>
#include <linux/clkdev.h>
#include <linux/delay.h>
#include <linux/err.h>
#include <linux/gpio.h>
#include <linux/init.h>
#include <linux/reboot.h>
#include <linux/of_address.h>
#include <linux/of_platform.h>
#include <linux/sys_soc.h>
#include <linux/semaphore.h>

#include <asm/system_misc.h>
#include <asm/mach/arch.h>

static void __init nuc900_machine_init(void)
{
	of_platform_populate(NULL, of_default_bus_match_table, NULL, NULL);
}

static const char *nuc900_dt_compat[] __initconst = {
	"nuvoton,nuc970",
	NULL,
};

DT_MACHINE_START(nuc900_dt, "Nuvoton NUC900 (Device Tree Support)")
	.init_machine	= nuc900_machine_init,
	.dt_compat	= nuc900_dt_compat,
MACHINE_END
