/* Raspberry Pi2-specific configuration information
 *
 * Copyright (c) 2013, Brian McKenzie <mckenzba@gmail.com>
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

#include "bcm2836.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>
#include <boot/bsp.h>
#include <interface/timer.h>
#include <interface/serial.h>
#include <interface/sysctl.h>
#include <drv/serial/pl011/pl011.h>
#include <drv/timer/bcm_timer/bcm_timer.h>

#define xstr(s) #s
#define str(s) xstr(s)

extern void _locore_halt_system();

/* Platform name */
char *bsp_platform_name = "Broadcom BCM2836";

/* Serial port configuration */
pl011_cfg pl011_config = {
	/* UART0 */
	.baud  = 115200,
	.clock = PL011_CLOCK_RATE,
	.base  = (addr_t)BCM2836_UART0_BASE,
};

/* Delay timer configuration */
bcm_timer_cfg bcm_timer_config = {
	.base  = (addr_t)BCM2836_TIMER_BASE,
};

/* Default environment settings for this platform */
env_init_list_t env_list[] = {
	{ "bootdelay", "10" },
	{ "boot-args", "-v" },
	{ "auto-boot", "false" },
	{ "boot-device", "nand0a" },
	{ "boot-partition", "0" },
	{ "loadaddr", str(DRAM_BASE) },
};

/* Board initialization routine */
int bsp_init(void)
{
       /* Initialize the delay timer */
        timer_init();

	/* Wait for RX fifo to empty. */
	while (readl((addr_t)(BCM2836_UART0_BASE + UART_FR)) & UART_FR_TXFF_BIT)
		;

	/* Turn off the uart */
	writel((addr_t)(BCM2836_UART0_BASE + UART_CR), UART_CR_DISA);

	/* Disable pull up/down for all pins and delay for 150 us. */
	writel((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), 0x00000000);
	usleep(150);

	/* Disable pull up/down for pins 14 and 15 and delay for 150 us. */
	writel((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), (BCM_GPIO_PIN(14) | BCM_GPIO_PIN(15)));
	usleep(150);

	/* Write 0 to GPPUDCLK0 to make it take effect. */
	writel((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUDCLK0), 0x00000000);

	/* Initialize the serial port */
	serial_init();

	/* Initialize debug output */
	printf_init(serial_putc);

	/* Initialize the environment */
	env_init(env_list, 6);

	return 0;
}

/* Power management routines */
void bcm2836_reset(void)
{
	/* Halt */
	_locore_halt_system();

	return;
}

void bcm2836_halt(void)
{
	/* Halt the board */
	_locore_halt_system();

	return;
}

/* Register system control interface */
sysctl_driver sysctl_drv = {
	bcm2836_reset,
	bcm2836_halt
};
