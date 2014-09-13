/* PL011 serial driver - prototypes and definitions
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
 * IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef PL011_H
#define PL011_H

#include <sys/io.h>
#include <sys/types.h>
#include <stdbool.h>

/* PL011 uart configuration structure */
typedef struct {
	uint32_t baud;  /* baud rate */
	uint32_t clock; /* clock speed in Hz */
	addr_t   base;  /* uart base address */
} pl011_cfg;

/* PL011 driver prototypes */
void pl011_init(void);
int pl011_rx_poll(void);
void pl011_putc(uint32_t c);
void pl011_puts(const char *str);
void pl011_rx_interrupt_mask(bool state);
void pl011_tx_interrupt_mask(bool state);

/* PL011 registers */
#define UART_DR         0x000
#define UART_RSR_ECR    0x004
#define UART_DMAWM      0x008
#define UART_TIMEOUT    0x00C
#define UART_FR         0x018
#define UART_LCRH_RX    0x01C
#define UART_ILPR       0x020
#define UART_IBRD       0x024
#define UART_FBRD       0x028
#define UART_LCRH_TX    0x02C
#define UART_CR         0x030
#define UART_IFLS       0x034
#define UART_IMSC       0x038
#define UART_RIS        0x03C
#define UART_MIS        0x040
#define UART_ICR        0x044
#define UART_DMACR      0x048
#define UART_XFCR       0x050
#define UART_XON1       0x054
#define UART_XON2       0x058
#define UART_XOFF1      0x05C
#define UART_XOFF2      0x060
#define UART_ITCR       0x080
#define UART_ITIP       0x084
#define UART_ITOP       0x088
#define UART_YDR        0x08C
#define UART_ABCR       0x100
#define UART_ABSR       0x104
#define UART_ABFMT      0x108
#define UART_ABDR       0x150
#define UART_ABDFR      0x154
#define UART_ABMR       0x158
#define UART_ABIMSC     0x15C
#define UART_ABRIS      0x160
#define UART_ABMIS      0x164
#define UART_ABICR      0x168
#define UART_IDPROD     0xFD8
#define UART_IDPROV     0xFDC
#define UART_PERIPH0    0xFE0
#define UART_PERIPH1    0xFE4
#define UART_PERIPH2    0xFE8
#define UART_PERIPH3    0xFEC
#define UART_PCELLID0   0xFF0
#define UART_PCELLID1   0xFF4
#define UART_PCELLID2   0xFF8
#define UART_PCELLID3   0xFFC

/* PL011 status bits */
#define UART_FR_TXFE_BIT 0x080
#define UART_FR_RXFF_BIT 0x040
#define UART_FR_TXFF_BIT 0x020
#define UART_FR_RXFE_BIT 0x010
#define UART_ICR_DISA    0xFFF
#define UART_CR_DISA     0x000

/* PL011 control bits */
#define UART_LCRH_RXFE  0x010
#define UART_LCRH_TXFE  0x010
#define UART_CR_RXE     0x100
#define UART_CR_TXE     0x080
#define UART_CR_UEN     0x001
#define UART_CR_RTS     0x800
#define UART_CR_OSVFACT 0x008
#define UART_IMSC_RXMS  0x010
#define UART_IMSC_TXMS  0x020

/* PL011 LCRH word lengths */
#define UART_LCRH_8WL   0x060
#define UART_LCRH_7WL   0x040
#define UART_LCRH_6WL   0x020
#define UART_LCRH_5WL   0x000

#endif /* !PL011_H */
