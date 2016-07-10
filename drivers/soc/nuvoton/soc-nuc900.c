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
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/sys_soc.h>
#include <linux/platform_device.h>
#include <linux/mfd/syscon.h>
#include <linux/regmap.h>
#include <linux/of.h>

/* System ID in syscon */
#define GCR_CHIPID		0x00
#define GCR_CHIPID_MASK		0x00ffffff

static const struct of_device_id nuc900_soc_of_match[] = {
	{ .compatible = "nuvoton,nuc900-soc",	},
	{ }
};

static u32 nuc900_chipid;

static ssize_t nuc900_get_chipid(struct device *dev,
				 struct device_attribute *attr,
				 char *buf)
{
	return sprintf(buf, "0x%x\n", nuc900_chipid & GCR_CHIPID_MASK);
}

static struct device_attribute nuc900_chipid_attr =
	__ATTR(manufacturer,  S_IRUGO, nuc900_get_chipid,  NULL);

static ssize_t nuc900_get_versionid(struct device *dev,
			      struct device_attribute *attr,
			      char *buf)
{
	return sprintf(buf, "0x%x\n", (nuc900_chipid >> 24) & 0xff);
}

static struct device_attribute nuc900_version_attr =
	__ATTR(board,  S_IRUGO, nuc900_get_versionid,  NULL);

static int nuc900_soc_probe(struct platform_device *pdev)
{
	static struct regmap *syscon_regmap;
	struct soc_device *soc_dev;
	struct soc_device_attribute *soc_dev_attr;
	struct device_node *np = pdev->dev.of_node;
	int ret;

	syscon_regmap = syscon_regmap_lookup_by_phandle(np, "syscon");
	if (IS_ERR(syscon_regmap))
		return PTR_ERR(syscon_regmap);

	soc_dev_attr = kzalloc(sizeof(*soc_dev_attr), GFP_KERNEL);
	if (!soc_dev_attr)
		return -ENOMEM;

	ret = of_property_read_string(np, "compatible", &soc_dev_attr->soc_id);
	if (ret)
		return -EINVAL;

	soc_dev_attr->machine = "NUC900EVB";
	soc_dev_attr->family = "NUC900";
	soc_dev = soc_device_register(soc_dev_attr);
	if (IS_ERR(soc_dev)) {
		kfree(soc_dev_attr);
		return -ENODEV;
	}

	ret = regmap_read(syscon_regmap, GCR_CHIPID, &nuc900_chipid);
	if (ret)
		return -ENODEV;

	device_create_file(soc_device_to_device(soc_dev), &nuc900_chipid_attr);
	device_create_file(soc_device_to_device(soc_dev), &nuc900_version_attr);

	dev_info(&pdev->dev, "Nuvoton Chip ID: 0x%x, Version ID:0x%x\n",
		 nuc900_chipid & GCR_CHIPID_MASK,
		 (nuc900_chipid >> 24) & 0xff);

	return 0;
}

static struct platform_driver nuc900_soc_driver = {
	.probe = nuc900_soc_probe,
	.driver = {
		.name = "nuc900-soc",
		.of_match_table = nuc900_soc_of_match,
	},
};
builtin_platform_driver(nuc900_soc_driver);
