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
#include <mach/nuc970-regs-gcr.h>

#define GCR_CHIPID		0x00
#define GCR_CHIPID_MASK		0x00ffffff

int chipid;
int versionid;
static void __iomem *wtcr_addr;

static void __init nuc970_init(void)
{

}

static int  __init *nuc900_get_id(void)
{
	struct device_node *np;
	void __iomem *gcr_base;
	int id;

	np = of_find_compatible_node(NULL, NULL, "nuvoton,gcr");
	gcr_base = of_iomap(np, 0);
	WARN_ON(!gcr_base);

	id = readl(gcr_base + GCR_CHIPID);

	chipid = id & GCR_CHIPID_MASK;
	versionid = (id >> 24) & 0xff;

	iounmap(gcr_base);
	of_node_put(np);

	return 0;
}

static int __init nuc900_restart_init(void)
{
	struct device_node *np;

	np = of_find_compatible_node(NULL, NULL, "nuvoton,gcr");
	wtcr_addr = of_iomap(np, 0);
	if (!wtcr_addr)
		return -ENODEV;

	of_node_put(np);

	return 0;
}

static void __init nuc970_machine_init(void)
{
	struct device_node *root;
	struct device *parent;
	struct soc_device *soc_dev;
	struct soc_device_attribute *soc_dev_attr;
	int ret;

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return;

	root = of_find_node_by_path("/");
	ret = of_property_read_string(root, "model", &soc_dev_attr->machine);
	if (ret)
		return;

	nuc900_get_id();

	soc_dev_attr->family = kasprintf(GFP_KERNEL, "Nuvoton NUC900 MCUs");
	soc_dev_attr->soc_id = kasprintf(GFP_KERNEL, "%x", chipid);
	soc_dev_attr->revision = kasprintf(GFP_KERNEL, "%x", versionid);

	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr->family);
		kfree(soc_dev_attr->soc_id);
		kfree(soc_dev_attr->revision);
		kfree(soc_dev_attr);
		return;
	}

	parent = soc_device_to_device(soc_dev);

	if (of_machine_is_compatible("nuvoton,nuc970evb"))
		nuc970_init();

	of_platform_populate(NULL, of_default_bus_match_table, NULL, parent);

	nuc900_restart_init();

}

static const char *nuc970_dt_compat[] __initconst = {
	"nuvoton,nuc970evb",
	NULL,
};

void nuc970_restart(enum reboot_mode mode, const char *cmd)
{
	if (wtcr_addr) {
		while (__raw_readl(wtcr_addr + REG_WRPRTR) != 1) {
			__raw_writel(0x59, wtcr_addr + REG_WRPRTR);
			__raw_writel(0x16, wtcr_addr + REG_WRPRTR);
			__raw_writel(0x88, wtcr_addr + REG_WRPRTR);
		}

		__raw_writel(1, wtcr_addr + REG_AHBIPRST);
	}

	soft_restart(0);
}

DT_MACHINE_START(nuc970_dt, "Nuvoton nuc970 evb")
	.atag_offset	= 0x100,
	.init_machine	= nuc970_machine_init,
	.restart	= nuc970_restart,
	.dt_compat = nuc970_dt_compat,
MACHINE_END
