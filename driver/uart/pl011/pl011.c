/* PL011 UART driver
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

#include <stdbool.h>

#include <sys/io.h>
#include <sys/types.h>

#include <device/uart.h>

#include "pl011.h"

/* pull in uart configuration */
extern pl011_cfg pl011_config;

/**
 * pl011_init - initialize PL011 uart
 */
void pl011_init(void)
{
	int div, frac, rem;

	pl011_cfg *config = &pl011_config;

	addr_t base = config->base;
	uint32_t baud = config->baud;
	uint32_t clock = config->clock;

	/* wait until the uart tx fifo is empty before proceeding */
	while (readl(base + UART_FR) & UART_FR_TXFF_BIT)
		;

	/* disable interrupts and turn off the uart */
	writel((base + UART_ICR), UART_ICR_DISA);
	writel((base + UART_CR), UART_CR_DISA);

	/* compute the integer and fraction divisor */
	if (readl(base + UART_CR) & UART_CR_OSVFACT) {
		div = clock / (8 * baud);
		rem = clock % (8 * baud);
		frac = ((8 * rem / baud) >> 1);
		frac += ((8 * rem / baud) & 1);
	} else {
		div = clock / (16 * baud);
		rem = clock % (16 * baud);
		frac = ((8 * rem / baud) >> 1);
		frac += ((8 * rem / baud) & 1);
	}

	/* set the integer and fraction divisor */
	writel((base + UART_IBRD), div);
	writel((base + UART_FBRD), frac);

	/* turn on the uart */
	writel((base + UART_LCRH_RX), (UART_LCRH_8WL | UART_LCRH_RXFE));
	writel((base + UART_LCRH_TX), (UART_LCRH_8WL | UART_LCRH_TXFE));
	writel((base + UART_CR), (UART_CR_UEN | UART_CR_RXE | UART_CR_TXE));

	/* set rx and tx interrupt masks */
	pl011_rx_interrupt_mask(TRUE);
	pl011_tx_interrupt_mask(TRUE);

	return;
}

/**
 * pl011_rx_interrupt_mask - mask uart rx interrupts
 */
void pl011_rx_interrupt_mask(bool state)
{
	pl011_cfg *config = &pl011_config;
	addr_t base = config->base;

	if (state == true)
		writel((base + UART_IMSC), (UART_IMSC_RXMS));
	else
		writel((base + UART_IMSC), (UART_IMSC_RXMS & ~UART_IMSC_RXMS));

	return;
}

/**
 * pl011_rx_interrupt_mask - mask uart tx interrupts
 */
void pl011_tx_interrupt_mask(bool state)
{
	pl011_cfg *config = &pl011_config;
	addr_t base = config->base;

	if (state == true)
		writel((base + UART_IMSC), (UART_IMSC_TXMS));
	else
		writel((base + UART_IMSC), (UART_IMSC_TXMS & ~UART_IMSC_TXMS));

	return;
}

/**
 * pl011_getc - read in character data from the rx fifo
 */
uint32_t pl011_getc(void)
{
	uint32_t c;
	pl011_cfg *config = &pl011_config;
	addr_t base = config->base;

	/* wait for incomming data */
	while (readl(base + UART_FR) & UART_FR_RXFE_BIT)
		;

	c = readl(base);

	/* check for errors */
	if (c & 0xF00) {
		writel((base + UART_RSR_ECR), 0x0);
		return -1;
	}

	return c;
}

/**
 * pl011_poll - poll the rx fifo for data
 *
 * @return: 1 for incomming data, 0 for none
 */
int pl011_poll(void)
{
	pl011_cfg *config = &pl011_config;
	addr_t base = config->base;

	if (readl(base + UART_FR) & UART_FR_RXFE_BIT)
		return 0;
	else
		return 1;
}

/**
 * pl011_putc - send out character data on the tx fifo
 *
 * @arg: c: character to send out
 */
void pl011_putc(uint32_t c)
{
	pl011_cfg *config = &pl011_config;
	addr_t base = config->base;

	/* wait until the uart is done transmitting */
	while (readl(base + UART_FR) & UART_FR_TXFF_BIT)
		;

	/* send it out */
	writeb(base, c);

	return;
}

/**
 * pl011_puts - send out a string on the tx fifo
 */
void pl011_puts(const char *str)
{
	while (*str != '\0') {
		if (*str == '\n')
			pl011_putc('\r');

		pl011_putc(*str++);
	}

	return;
}

/* register this driver */
serial_driver serial_drv = {
	pl011_init,
	pl011_poll,
	pl011_getc,
	pl011_putc,
	pl011_puts,
};
