/* Raspberry Pi2-specific configuration information
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

#include <shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/io.h>

#include <boot/bsp.h>
#include <shell/shell.h>
#include <device/timer.h>
#include <device/uart.h>
#include <device/sysctl.h>

#include <driver/uart/pl011/pl011.h>
#include <driver/timer/bcm_timer/bcm_timer.h>

#include "bcm2836.h"

#define xstr(s) #s
#define str(s) xstr(s)

extern void _locore_halt_system();

static inline void delay(uint32_t count)
{
	__asm__ volatile("__delay_%=: subs %[count], %[count], #1; bne __delay_%=\n"
		: : [count]"r"(count) : "cc");
}

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

	/* Turn off the uart */
	writel((addr_t)(BCM2836_UART0_BASE + UART_CR), UART_CR_DISA);

	/* Disable pull up/down for all pins and delay for 150 cycles. */
	writel((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUD), 0x00000000);
	delay(150);

	/* Disable pull up/down for pins 14 and 15 and delay for 150 cycles. */
	writeorl((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUDCLK0), BCM_GPIO_PIN(14));
	writeorl((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUDCLK0), BCM_GPIO_PIN(15));
	delay(150);

	/* Write 0 to GPPUDCLK0 to make it take effect. */
	writel((addr_t)(BCM2836_GPIO_BASE + BCM_GPIO_GPPUDCLK0), 0x00000000);

	/* Initialize the serial port */
	uart_init();

	/* Initialize debug output */
	printf_init(uart_putc);

	printf("I've made it here!\n");

	/* Initialize the environment */
	shell_runscript(init_script);

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
