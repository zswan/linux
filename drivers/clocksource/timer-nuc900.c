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
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <linux/jiffies.h>
#include <linux/clockchips.h>
#include <linux/types.h>
#include <linux/clk.h>

#include <linux/io.h>
#include <asm/mach/time.h>

#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_irq.h>

#include <mach/mfp.h>
#include <mach/map.h>
#include <mach/nuc970-regs-timer.h>
#include <mach/hardware.h>
#include <mach/regs-clock.h>

#define RESETINT	0x1f
#define PERIOD		(0x01 << 27)
#define ONESHOT		(0x00 << 27)
#define COUNTEN		(0x01 << 30)
#define INTEN		(0x01 << 29)

#define TICKS_PER_SEC	100
/* Divider = prescale + 1 */
#define PRESCALE	0x63

#define	TDR_SHIFT	24
#define	TDR_MASK	((1 << TDR_SHIFT) - 1)

static unsigned int timer0_load;
static void __iomem *tmr_base;

static int nuc970_clockevent_set_oneshot(struct clock_event_device *evt)
{
	unsigned int val;

	val = __raw_readl(tmr_base + REG_TMR_TCSR0);
	val &= ~(0x03 << 27);

	val |= (ONESHOT | COUNTEN | INTEN | PRESCALE);

	__raw_writel(val, tmr_base + REG_TMR_TCSR0);
	return 0;
}

static int nuc970_clockevent_set_periodic(struct clock_event_device *evt)
{
	unsigned int val;

	val = __raw_readl(tmr_base + REG_TMR_TCSR0);
	val &= ~(0x03 << 27);

	__raw_writel(timer0_load, tmr_base + REG_TMR_TICR0);
	val |= (PERIOD | COUNTEN | INTEN | PRESCALE);

	__raw_writel(val, tmr_base + REG_TMR_TCSR0);

	return 0;
}

static int nuc970_clockevent_setnextevent(unsigned long evt,
		struct clock_event_device *clk)
{
	unsigned int tcsr, tdelta;

	tcsr = __raw_readl(tmr_base + REG_TMR_TCSR0);
	tdelta = __raw_readl(tmr_base + REG_TMR_TICR0) -
			     __raw_readl(tmr_base + REG_TMR_TDR0);

	__raw_writel(evt, tmr_base + REG_TMR_TICR0);

	if (!(tcsr & COUNTEN) && ((tdelta > 2) || (tdelta == 0)))
		__raw_writel(__raw_readl(tmr_base + REG_TMR_TCSR0) | COUNTEN,
			     tmr_base + REG_TMR_TCSR0);

	return 0;
}

static int nuc970_clockevent_shutdown(struct clock_event_device *evt)
{
	unsigned int val = __raw_readl(tmr_base + REG_TMR_TCSR0) &
				       ~(0x03 << 27);

	__raw_writel(val, tmr_base + REG_TMR_TCSR0);

	return 0;
}

static struct clock_event_device nuc970_clockevent_device = {
	.name			= "nuc970-timer0",
	.shift			= 32,
	.features		= CLOCK_EVT_FEAT_PERIODIC |
				  CLOCK_EVT_FEAT_ONESHOT,
	.set_state_shutdown	= nuc970_clockevent_shutdown,
	.set_state_periodic	= nuc970_clockevent_set_periodic,
	.set_state_oneshot	= nuc970_clockevent_set_oneshot,
	.set_next_event		= nuc970_clockevent_setnextevent,
	.tick_resume		= nuc970_clockevent_shutdown,
	.rating			= 300,
};

/*IRQ handler for the timer*/
static irqreturn_t nuc970_timer0_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = &nuc970_clockevent_device;

	__raw_writel(0x01, tmr_base + REG_TMR_TISR);

	evt->event_handler(evt);

	return IRQ_HANDLED;
}

static struct irqaction nuc970_timer0_irq = {
	.name		= "nuc970-timer0",
	.flags		= IRQF_TIMER | IRQF_IRQPOLL,
	.handler	= nuc970_timer0_interrupt,
};

static void __init nuc970_clock_source_event_init(void __iomem *base, int irq,
						  struct clk *clk_timer0,
						  struct clk *clk_timer1)
{
	unsigned int val;
	unsigned int rate = 0;

	/* Get the timer base address */
	tmr_base = base;

	/*Clocksource init*/
	WARN_ON(clk_prepare_enable(clk_timer1));

	__raw_writel(0x00, tmr_base + REG_TMR_TCSR1);

	rate = clk_get_rate(clk_timer1) / (PRESCALE + 1);

	__raw_writel(0xffffffff, tmr_base + REG_TMR_TICR1);

	val = __raw_readl(tmr_base + REG_TMR_TCSR1);
	val |= (COUNTEN | PERIOD | PRESCALE);
	__raw_writel(val, tmr_base + REG_TMR_TCSR1);

	clocksource_mmio_init(tmr_base + REG_TMR_TDR1, "nuc970-timer1",
			      rate, 200, TDR_SHIFT,
			      clocksource_mmio_readl_down);

	/*Clockevents init*/
	WARN_ON(clk_prepare_enable(clk_timer0));

	__raw_writel(0x00, tmr_base + REG_TMR_TCSR0);

	rate = clk_get_rate(clk_timer0) / (PRESCALE + 1);

	timer0_load = (rate / TICKS_PER_SEC);

	__raw_writel(RESETINT, tmr_base + REG_TMR_TISR);

	setup_irq(irq, &nuc970_timer0_irq);

	nuc970_clockevent_device.cpumask = cpumask_of(0);

	clockevents_config_and_register(&nuc970_clockevent_device, rate,
					0xf, 0xffffffff);
}

static void __init nuc970_timer_of_init(struct device_node *node)
{
	struct clk *clk_timer0, *clk_timer1;
	void __iomem *base;
	int irq;

	base = of_iomap(node, 0);
	if (!base)
		panic("%s: Unable to map timer base\n", node->full_name);

	clk_timer0 = of_clk_get_by_name(node, "timer0");
	if (IS_ERR(clk_timer0))
		panic("%s: Unable to get clk_timer0\n", node->full_name);

	clk_timer1 = of_clk_get_by_name(node, "timer1");
	if (IS_ERR(clk_timer1))
		panic("%s: Unable to get clk_timer1\n", node->full_name);

	irq = irq_of_parse_and_map(node, 0);
	if (irq <= 0)
		panic("%s: Unable to get irq\n", node->full_name);

	nuc970_clock_source_event_init(base, irq, clk_timer0, clk_timer1);
}

CLOCKSOURCE_OF_DECLARE(nuc970, "nuvoton,tmr", nuc970_timer_of_init);
