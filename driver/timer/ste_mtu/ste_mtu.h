/* ST-Ericsson Mulit Timer Unit driver - prototypes and definitions
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

#ifndef STE_MTU_H
#define STE_MTU_H

#include <sys/io.h>
#include <sys/types.h>

typedef struct {
	addr_t base;
} ste_mtu_cfg;

/* MTU registers */
#define MTU_IMSC   0x000	/* MTUx interrupt mask/set clear register */
#define MTU_RIS    0x004	/* MTUx raw interrupt status register */
#define MTU_MIS    0x008	/* MTUx masked interrupt status register */
#define MTU_ICR    0x00C	/* MTUx interrupt clear register */

#define MTU_LRT(y) \
	(0x10 + (y * 0x10))		/* MTUx timer y load register */
#define MTU_VALT(y) \
	(0x14 + (y * 0x10))		/* MTUx timer y value register */
#define MTU_CRT(y) \
	(0x18 + (y * 0x10))		/* MTUx timer y control register */
#define MTU_BGLRT(y) \
	(0x1C + (y * 0x10))		/* MTUx timer y background load register */

/* MTU driver prototypes */
void mtu_timer_init(void);
void mtu_timer_reset(void);
uint32_t mtu_timer_usec_count(void);

/* MTU imsc register bits */
#define MTU_IMT0_BIT    (1 << 0)
#define MTU_IMT1_BIT    (1 << 1)
#define MTU_IMT2_BIT    (1 << 2)
#define MTU_IMT3_BIT    (1 << 3)

/* MTU ris register bits */
#define MTU_RIST0_BIT   (1 << 0)
#define MTU_RIST1_BIT   (1 << 1)
#define MTU_RIST2_BIT   (1 << 2)
#define MTU_RIST3_BIT   (1 << 3)

/* MTU mis register bits */
#define MTU_MIST0_BIT   (1 << 0)
#define MTU_MIST1_BIT   (1 << 1)
#define MTU_MIST2_BIT   (1 << 2)
#define MTU_MIST3_BIT   (1 << 3)

/* MTU icr register bits */
#define MTU_ICRT0_BIT   (1 << 0)
#define MTU_ICRT1_BIT   (1 << 1)
#define MTU_ICRT2_BIT   (1 << 2)
#define MTU_ICRT3_BIT   (1 << 3)

/* MTU timer control register bits */
#define MTU_TR_WMODE    (1 << 0)                   /* timer[0..3] one-shot mode */
#define MTU_TSZ_32BIT   (1 << 1)                   /* timer[0..3] 32bit counter size */
#define MTU_TPRE_DIV16  (1 << 2)                   /* timer[0..3] prescailer, divide by 16 */
#define MTU_TS_PEMODE   (1 << 6)                   /* timer[0..3] periodic mode */
#define MTU_TENABLE     (1 << 7)                   /* timer[0..3] enabled */

#endif /* !STE_MTU_H */
