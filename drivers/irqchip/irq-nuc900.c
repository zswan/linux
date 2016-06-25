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

#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/irqchip.h>
#include <linux/irqdomain.h>
#include <linux/io.h>
#include <linux/ioport.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include <asm/exception.h>
#include <asm/hardirq.h>

#include <mach/nuc970-regs-aic.h>

static void __iomem *aic_base;
static struct irq_domain *aic_domain;
#define MAKE_HWIRQ(irqnum)	(irqnum)

static void nuc970_irq_mask(struct irq_data *d)
{
	if (d->irq < 32)
		__raw_writel(1 << (d->irq), aic_base + REG_AIC_MDCR);
	else
		__raw_writel(1 << (d->irq - 32), aic_base + REG_AIC_MDCRH);
}

static void nuc970_irq_ack(struct irq_data *d)
{
	__raw_writel(0x01, aic_base + REG_AIC_EOSCR);
}

static void nuc970_irq_unmask(struct irq_data *d)
{
	if (d->irq < 32)
		__raw_writel(1 << (d->irq), aic_base + REG_AIC_MECR);
	else
		__raw_writel(1 << (d->irq - 32), aic_base + REG_AIC_MECRH);
}

static struct irq_chip nuc970_irq_chip = {
	.irq_ack	= nuc970_irq_ack,
	.irq_mask	= nuc970_irq_mask,
	.irq_unmask	= nuc970_irq_unmask,
};

void __exception_irq_entry aic_handle_irq(struct pt_regs *regs)
{
	u32 hwirq;

	hwirq = __raw_readl(aic_base + REG_AIC_IPER);
	hwirq = __raw_readl(aic_base + REG_AIC_ISNR);
	if (!hwirq)
		__raw_writel(0x01, aic_base + REG_AIC_EOSCR);

	handle_IRQ((irq_find_mapping(aic_domain, hwirq)), regs);
}

static int aic_irq_domain_map(struct irq_domain *d, unsigned int virq,
			      irq_hw_number_t hw)
{
	irq_set_chip_and_handler(virq, &nuc970_irq_chip, handle_level_irq);
	irq_clear_status_flags(virq, IRQ_NOREQUEST);

	return 0;
}

static struct irq_domain_ops aic_irq_domain_ops = {
	.map = aic_irq_domain_map,
	.xlate = irq_domain_xlate_onecell,
};

static int __init aic_of_init(struct device_node *node,
			      struct device_node *parent)
{
	aic_base = of_iomap(node, 0);
	if (!aic_base)
		panic("%s: unable to map IC registers\n", node->full_name);

	__raw_writel(0xFFFFFFFC, aic_base + REG_AIC_MDCR);
	__raw_writel(0xFFFFFFFF, aic_base + REG_AIC_MDCRH);

	aic_domain = irq_domain_add_linear(node, NR_IRQS,
					   &aic_irq_domain_ops, NULL);

	if (!aic_domain)
		panic("%s: unable to create IRQ domain\n", node->full_name);

	set_handle_irq(aic_handle_irq);
	return 0;
}

IRQCHIP_DECLARE(nuc970, "nuvoton,aic", aic_of_init);
