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
#include <linux/init.h>
#include <linux/err.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <linux/mfd/syscon.h>
#include <linux/module.h>
#include <linux/notifier.h>
#include <linux/of.h>
#include <linux/platform_device.h>
#include <linux/reboot.h>
#include <linux/regmap.h>

#define REG_WRPRTR	0x1fc
#define REG_AHBIPRST	0x060

static struct regmap *syscon;

static int nuc900_restart_handler(struct notifier_block *this,
				  unsigned long mode, void *cmd)
{
	/*0:register protect enable*/
	int write_protect = 0;

	/*register protection disable*/
	do {
		regmap_read(syscon, REG_WRPRTR, &write_protect);

		if (write_protect != 1) {
			regmap_write(syscon, REG_WRPRTR, 0x59);
			regmap_write(syscon, REG_WRPRTR, 0x16);
			regmap_write(syscon, REG_WRPRTR, 0x88);
		}

	} while (write_protect != 1);

	/*trigger reset*/
	regmap_write(syscon, REG_AHBIPRST, 0x01);

	return NOTIFY_DONE;
}

static struct notifier_block nuc900_restart_nb = {
	.notifier_call = nuc900_restart_handler,
	.priority = 128,
};

static int nuc900_reset_probe(struct platform_device *pdev)
{
	struct device *dev = &pdev->dev;
	int err;

	syscon = syscon_regmap_lookup_by_phandle(dev->of_node, "syscon");
	if (IS_ERR(syscon)) {
		pr_err("%s: syscon lookup failed\n", dev->of_node->name);
		return PTR_ERR(syscon);
	}

	err = register_restart_handler(&nuc900_restart_nb);
	if (err)
		dev_err(dev, "cannot register restart handler (err=%d)\n", err);

	return err;
}

static const struct of_device_id of_nuc900_reset_match[] = {
	{ .compatible = "nuvoton,nuc900-reset", },
	{},
};
MODULE_DEVICE_TABLE(of, of_nuc900_reset_match);

static struct platform_driver nuc900_reset_driver = {
	.probe = nuc900_reset_probe,
	.driver = {
		.name = "nuc900-reset",
		.of_match_table = of_match_ptr(of_nuc900_reset_match),
	},
};

static int __init nuc900_reset_init(void)
{
	return platform_driver_register(&nuc900_reset_driver);
}
device_initcall(nuc900_reset_init);
