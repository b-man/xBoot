/* ARM RealView-specific configuration information
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

#include <shell.h>

#include <boot/bsp.h>

#include <device/timer.h>
#include <device/uart.h>
#include <device/sysctl.h>

#include <driver/uart/pl011/pl011.h>
#include <driver/timer/sp804/sp804.h>

#include "rvpba8.h"
#include "realview-pba8.h"

#define xstr(s) #s
#define str(s) xstr(s)

extern void _locore_halt_system();

/* Platform name */
char *bsp_platform_name = "ARM RealView Platform Baseboard (Cortex-A8)";

/* Serial port configuration */
pl011_cfg pl011_config = {
	/* UART0 */
	.baud  = 115200,
	.clock = PL011_CLOCK_RATE,
	.base  = (addr_t)PL011_UART0_BASE,
};

/* Delay timer configuration */
sp804_cfg sp804_config = {
	.base  = (addr_t)SP804_TIMER01_BASE,
};

/* Default environment settings for this platform */
char init_script[512] = \
	"setenv bootdelay 10;"
	"setenv bootargs \"rd=md0 debug=0x16e serial=3 -v symbolicate_panics=1\";"
	"setenv autoboot false;"
	"setenv bootdev md0;"
	"setenv bootpart 0;"
	"setenv loadaddr " str(DRAM_BASE);

/* Board initialization routine */
int bsp_init(void)
{
	/* Initialize the delay timer */
	timer_init();

	/* Initialize the serial port */
	uart_init();

	/* Initialize debug output */
	printf_init(uart_putc);

	/* Use REFCLK (1Mhz) for timer2 */
	writel((addr_t)SYS_CTRL0_BASE, (1 << 19));

	/* Initialize the environment */
	shell_runscript(init_script);

	return 0;
}

/* Power management routines */
void rvpba8_reset(void)
{
	/* Prepare for sw reset */
	writel((addr_t)SYS_CTL_LOCK, SYS_CTL_KEY);

	/* Reset the board */
	writel((addr_t)SYS_RESETCTL, 0x0);
	writel((addr_t)SYS_RESETCTL, SYS_PORESET_BIT);

	/* Halt on failure */
	_locore_halt_system();

	return;
}

void rvpba8_halt(void)
{

	/* Halt the board */
	_locore_halt_system();

	return;
}

/* Register system control interface */
sysctl_driver sysctl_drv = {
	rvpba8_reset,
	rvpba8_halt
};
