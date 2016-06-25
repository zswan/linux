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
#include <linux/clk-provider.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/err.h>

#include "clk-ccf.h"

struct clk_apll {
	struct clk_hw	hw;
	void __iomem	*base;
};

#define to_clk_apll(clk) (container_of(clk, struct clk_apll, clk))

static int clk_apll_set_rate(struct clk_hw *hw, unsigned long rate,
			     unsigned long parent_rate)
{
	struct clk_apll *pll = to_clk_apll(hw);
	unsigned long reg;

	reg = readl(pll->base) & ~0x0FFFFFFF;

	switch (rate) {
	/*usbh*/
	case 96000000:
		reg |= 0x8027;
		break;
	/*i2s*/
	case 98400000:
		reg |= 0x8028;
		break;
	/*i2s*/
	case 169500000:
		reg |= 0x21f0;
		break;
	/*system default, 264MHz*/
	case 264000000:
		reg |= 0x15;
		break;
	case 300000000:
		reg |= 0x18;
		break;
	default:
		reg |= 0x15;
		break;
	}

	writel(reg, pll->base);

	return 0;
}

static unsigned long clk_apll_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate)
{
	struct clk_apll *pll = to_clk_apll(hw);
	unsigned long reg = readl(pll->base) & 0x0FFFFFFF;
	unsigned long rate;

	if (parent_rate != 12000000)
		return 0;

	switch (reg) {
	/*system default, 264MHz*/
	case 0x15:
		rate = 264000000;
		break;
	case 0x18:
		rate = 300000000;
		break;
	/*usbh*/
	case 0x8027:
		rate = 96000000;
		break;
	/*i2s*/
	case 0x8028:
		rate = 98400000;
		break;
	/*i2s*/
	case 0x21f0:
		rate = 169500000;
		break;
	default:
		rate = 264000000;
		break;
	}

	return rate;
}

static long clk_apll_round_rate(struct clk_hw *hw, unsigned long rate,
				unsigned long *prate)
{
	return rate;
}

static int clk_apll_enable(struct clk_hw *hw)
{
	struct clk_apll *pll = to_clk_apll(hw);
	unsigned long val;

	val = __raw_readl(pll->base);
	val &= ~0x10000000;
	val |= 0x40000000;
	__raw_writel(val, pll->base);

	return 0;
}

static void clk_apll_disable(struct clk_hw *hw)
{
	struct clk_apll *pll = to_clk_apll(hw);
	unsigned long val;

	val = __raw_readl(pll->base);
	val |= 0x10000000;
	val &= ~0x40000000;
	__raw_writel(val, pll->base);
}

static struct clk_ops clk_apll_ops = {
	.recalc_rate = clk_apll_recalc_rate,
	.enable = clk_apll_enable,
	.disable = clk_apll_disable,
	.set_rate = clk_apll_set_rate,
	.round_rate = clk_apll_round_rate,
};

struct clk *nuc970_clk_apll(const char *name, const char *parent,
			    void __iomem *base)
{
	struct clk_apll *pll;
	struct clk *clk;
	struct clk_init_data init;

	pll = kmalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);

	pll->base = base;

	init.name = name;
	init.ops = &clk_apll_ops;
	init.flags = 0;
	init.parent_names = &parent;
	init.num_parents = 1;

	pll->hw.init = &init;

	clk = clk_register(NULL, &pll->hw);
	if (IS_ERR(clk))
		kfree(pll);

	return clk;
}
