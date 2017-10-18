/* BCM283x System Timer driver
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

#include "bcm_timer.h"

/* pull in config */
extern bcm_timer_cfg bcm_timer_config;
static bcm_timer_cfg *cfg = &bcm_timer_config;

/**
 * bcm_timer_init - initialize the delay timer
 */
void bcm_timer_init(void)
{
	/* reset the timer */
	bcm_timer_reset();

	return;
}

/**
 * bcm_timer_reset - reset the timer to zero
 */
void bcm_timer_reset(void)
{
	return;
}

/**
 * bcm_timer_usec_count - get the tick value of the timer in microseconds
 * @return: value between 0 and 2^32, counting up.
 */
uint32_t bcm_timer_usec_count(void)
{
	uint32_t value;
	addr_t timer_base = cfg->base;

	value = readl(timer_base + BCM_TMR_CLO);

	return value;
}

/* register this driver */
timer_driver timer_drv = {
	bcm_timer_init,
	bcm_timer_reset,
	bcm_timer_usec_count,
};
