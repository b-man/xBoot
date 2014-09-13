/* ARM SP804 Dual Timer driver
 *
 * Copyright (c) 2014, Brian McKenzie <mckenzba@gmail.com>
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

#include "sp804.h"

#include <sys/io.h>
#include <sys/types.h>

#include <interface/timer.h>

#define DELAY_TIMER 2	/* timer 2 will be the delay timer */

/* pull in config */
extern sp804_cfg sp804_config;

/**
 * sp804_timer_init - initialize the delay timer
 */
void sp804_timer_init(void)
{
	sp804_cfg *cfg = &sp804_config;
	addr_t base = cfg->base;

	/* disable the timer, clear all settings */
	writel((base + SP804_TIMER2_CR), 0x0);

	/* set up a 32bit timer, with a prescale divisor of 1 */
	writel((base + SP804_TIMER2_CR), (SP804_CR_TSZ_32B | SP804_CR_TEN));

	/* reset the timer */
	sp804_timer_reset();

	return;
}

/**
 * sp804_timer_reset - reset the timer to zero
 */
void sp804_timer_reset(void)
{
	sp804_cfg *cfg = &sp804_config;
	addr_t base = cfg->base;

	writel((base + SP804_TIMER2_LR), 0x0);

	return;
}

/**
 * sp804_timer_usec_count - get the tick value of the timer in microseconds
 * @return: value between 0 and 2^32, counting up.
 */
uint32_t sp804_timer_usec_count(void)
{
	uint32_t value;
	sp804_cfg *cfg = &sp804_config;
	addr_t base = cfg->base;

	value = ~readl(base + SP804_TIMER2_VAL);

	return value;
}

/* register this driver */
timer_driver timer_drv = {
	sp804_timer_init,
	sp804_timer_reset,
	sp804_timer_usec_count,
};
