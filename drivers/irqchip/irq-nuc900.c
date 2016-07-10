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

#define	REG_AIC_SCR1	0x00
#define	REG_AIC_SCR2	0x04
#define	REG_AIC_SCR3	0x08
#define	REG_AIC_SCR4	0x0C
#define	REG_AIC_SCR5	0x10
#define	REG_AIC_SCR6	0x14
#define	REG_AIC_SCR7	0x18
#define	REG_AIC_SCR8	0x1C
#define	REG_AIC_SCR9	0x20
#define	REG_AIC_SCR10	0x24
#define	REG_AIC_SCR11	0x28
#define	REG_AIC_SCR12	0x2C
#define	REG_AIC_SCR13	0x30
#define	REG_AIC_SCR14	0x34
#define	REG_AIC_SCR15	0x38
#define	REG_AIC_IRSR	0x100
#define	REG_AIC_IRSRH	0x104
#define	REG_AIC_IASR	0x108
#define	REG_AIC_IASRH	0x10C
#define	REG_AIC_ISR	0x110
#define	REG_AIC_ISRH	0x114
#define	REG_AIC_IPER	0x118
#define	REG_AIC_ISNR	0x120
#define	REG_AIC_OISR	0x124
#define	REG_AIC_IMR	0x128
#define	REG_AIC_IMRH	0x12C
#define	REG_AIC_MECR	0x130
#define	REG_AIC_MECRH	0x134
#define	REG_AIC_MDCR	0x138
#define	REG_AIC_MDCRH	0x13C
#define	REG_AIC_SSCR	0x140
#define	REG_AIC_SSCRH	0x144
#define	REG_AIC_SCCR	0x148
#define	REG_AIC_SCCRH	0x14C
#define	REG_AIC_EOSCR	0x150

static void __iomem *aic_base;
static struct irq_domain *aic_domain;

static void nuc900_irq_mask(struct irq_data *d)
{
	if (d->irq < 32)
		writel(1 << (d->irq), aic_base + REG_AIC_MDCR);
	else
		writel(1 << (d->irq - 32), aic_base + REG_AIC_MDCRH);
}

static void nuc900_irq_ack(struct irq_data *d)
{
	writel(0x01, aic_base + REG_AIC_EOSCR);
}

static void nuc900_irq_unmask(struct irq_data *d)
{
	if (d->irq < 32)
		writel(1 << (d->irq), aic_base + REG_AIC_MECR);
	else
		writel(1 << (d->irq - 32), aic_base + REG_AIC_MECRH);
}

static struct irq_chip nuc900_irq_chip = {
	.irq_ack	= nuc900_irq_ack,
	.irq_mask	= nuc900_irq_mask,
	.irq_unmask	= nuc900_irq_unmask,
};

void __exception_irq_entry aic_handle_irq(struct pt_regs *regs)
{
	u32 hwirq;

	hwirq = readl(aic_base + REG_AIC_IPER);
	hwirq = readl(aic_base + REG_AIC_ISNR);
	if (!hwirq)
		writel(0x01, aic_base + REG_AIC_EOSCR);

	handle_IRQ((irq_find_mapping(aic_domain, hwirq)), regs);
}

static int aic_irq_domain_map(struct irq_domain *d, unsigned int virq,
			      irq_hw_number_t hw)
{
	irq_set_chip_and_handler(virq, &nuc900_irq_chip, handle_level_irq);
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
	int ret;

	aic_base = of_iomap(node, 0);
	if (!aic_base) {
		ret = -ENOMEM;
		pr_err("%s: unable to map registers\n", node->full_name);
		goto err_iomap;
	}

	writel(0xFFFFFFFC, aic_base + REG_AIC_MDCR);
	writel(0xFFFFFFFF, aic_base + REG_AIC_MDCRH);

	aic_domain = irq_domain_add_linear(node, NR_IRQS,
					   &aic_irq_domain_ops, NULL);

	if (!aic_domain) {
		ret = -ENOMEM;
		pr_err("%s: unable to create IRQ domain\n", node->full_name);
		goto err_aic_domain;
	}

	set_handle_irq(aic_handle_irq);
	return 0;

err_aic_domain:
	iounmap(aic_base);
err_iomap:
	return ret;
}

IRQCHIP_DECLARE(nuc900, "nuvoton,nuc900-aic", aic_of_init);
