/* ST-Ericsson Mulit Timer Unit driver
 *
 * Copyright (c) 2017, Brian McKenzie <mckenzba@gmail.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  1. Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *
 *  2. Redistributions in binary form must reproduce the above copyright notice,
 *     this list of conditions and the following disclaimer in the documentation
 *     and/or other materials provided with the distribution.
 *
 *  3. Neither the name of the organization nor the names of its contributors may
 *     be used to endorse or promote products derived from this software without
 *     specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <sys/io.h>
#include <sys/types.h>

#include <device/timer.h>

#include "ste_mtu.h"

#define DELAY_TIMER 2	/* MTU0 timer 2 will be the delay timer */

/* pull in config */
extern ste_mtu_cfg mtu_config;

/*
 * The MTU runs at a clock speed of 133Mhz.
 * To get the tick value in microseconds, multiply the tick
 * value by the divisor, and then divide by the clock rate.
 */
#define TICK_TO_USEC(x)	((x) * 16 / 133)

/**
 * mtu_timer_init - initialize the delay timer
 */
void mtu_timer_init(void)
{
	ste_mtu_cfg *cfg = &mtu_config;
	addr_t base = cfg->base;

	/* disable the timer, clear all settings */
	writel((base + MTU_CRT(DELAY_TIMER)), 0x0);

	/* set up a 32bit timer with a prescale divisor of 16 and enable it */
	writel((base + MTU_CRT(DELAY_TIMER)), (MTU_TPRE_DIV16 | MTU_TSZ_32BIT | MTU_TENABLE));

	/* reset the timer */
	mtu_timer_reset();

	return;
}

/**
 * mtu_timer_reset - reset the timer to zero
 */
void mtu_timer_reset(void)
{
	ste_mtu_cfg *cfg = &mtu_config;
	addr_t base = cfg->base;

	writel((base + MTU_LRT(DELAY_TIMER)), 0x0);

	return;
}

/**
 * mtu_timer_usec_count - get the tick value of the timer in microseconds
 * @return: value between 0 and 2^32, counting up.
 */
uint32_t mtu_timer_usec_count(void)
{
	uint32_t value;
	ste_mtu_cfg *cfg = &mtu_config;
	addr_t base = cfg->base;

	value = ~readl(base + MTU_VALT(DELAY_TIMER));

	return TICK_TO_USEC(value);
}

/* register this driver */
timer_driver timer_drv = {
	mtu_timer_init,
	mtu_timer_reset,
	mtu_timer_usec_count,
};
