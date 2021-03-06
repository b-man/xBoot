/* AP9500-specific configuration information
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
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR AND
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>

#include <boot/bsp.h>
#include <device/timer.h>
#include <device/sysctl.h>
#include <device/uart.h>

#include <driver/uart/pl011/pl011.h>
#include <driver/timer/ste_mtu/ste_mtu.h>

#include "ap9500.h"
#include "snowball.h"

/* Platform name */
char *bsp_platform_name = "ST-Ericsson Snowball";

/* Serial port configuration */
pl011_cfg pl011_config = {
	/* UART2 */
	.baud  = 115200,
	.clock = PL011_CLOCK_RATE,
	.base  = (addr_t)(AP9500_PERIPH3_BASE + 0x7000),
};

/* Delay timer configuration */
ste_mtu_cfg mtu_config = {
	.base = (addr_t)(AP9500_PERIPH6_BASE + 0x6000),
};

/* Default environment settings for this platform */
static void bsp_env_init(void)
{
	setenv("bootdelay", "10", 1);
}

int bsp_init(void)
{
	/* Initialize the delay timer */
	timer_init();

	/* Initialize the serial port */
	uart_init();

	/* Initialize console output */
	printf_init(uart_putc);

	/* Initialize the default environment */
	bsp_env_init();

	return 0;
}

void prcmu_reset(void)
{
	return;
}

void prcmu_halt(void)
{
	return;
}

/* Register system control interface */
sysctl_driver sysctl_drv = {
	prcmu_reset,
	prcmu_halt
};
