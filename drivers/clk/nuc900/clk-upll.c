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

struct clk_upll {
	struct clk_hw	hw;
	void __iomem	*base;
};

#define to_clk_upll(clk) (container_of(clk, struct clk_upll, clk))

static unsigned long clk_upll_recalc_rate(struct clk_hw *hw,
					  unsigned long parent_rate)
{
	struct clk_upll *pll = to_clk_upll(hw);
	unsigned long rate;
	unsigned long reg = readl(pll->base) & 0x0FFFFFFF;

	if (parent_rate != 12000000)
		return 0;

	switch (reg) {
	case 0x15:
		rate = 264000000;
		break;
	case 0x18:
		rate = 300000000;
		break;
	default:
		rate = 264000000;
		break;
	}

	return rate;
}

static struct clk_ops clk_upll_ops = {
	.recalc_rate = clk_upll_recalc_rate,
};

struct clk *nuc970_clk_upll(const char *name, const char *parent,
			    void __iomem *base)
{
	struct clk_upll *pll;
	struct clk *clk;
	struct clk_init_data init;

	pll = kmalloc(sizeof(*pll), GFP_KERNEL);
	if (!pll)
		return ERR_PTR(-ENOMEM);

	pll->base = base;

	init.name = name;
	init.ops = &clk_upll_ops;
	init.flags = 0;
	init.parent_names = &parent;
	init.num_parents = 1;

	pll->hw.init = &init;

	clk = clk_register(NULL, &pll->hw);
	if (IS_ERR(clk))
		kfree(pll);

	return clk;
}
