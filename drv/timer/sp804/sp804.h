/* ARM SP804 Dual Timer driver - prototypes and definitions
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

#ifndef SP804_H
#define SP804_H

#include <sys/io.h>
#include <sys/types.h>

/* SP804 timer configuration structure */
typedef struct {
	addr_t base;
} sp804_cfg;

/* SP804 timer driver prototypes */
void sp804_timer_init(void);
void sp804_timer_reset(void);
uint32_t sp804_timer_usec_count(void);

/* SP804 timer1 registers */
#define SP804_TIMER1_LR  0x000 /* timer1 load register */
#define SP804_TIMER1_VAL 0x004 /* timer1 value register */
#define SP804_TIMER1_CR  0x008 /* timer1 control register */
#define SP804_TIMER1_IC  0x00C /* timer1 interrupt clear register */
#define SP804_TIMER1_RIS 0x010 /* timer1 raw interrupt status register */
#define SP804_TIMER1_MIS 0x014 /* timer1 masked interrupt status register */
#define SP804_TIMER1_BGL 0x018 /* timer1 background load register */

/* SP804 timer2 register */
#define SP804_TIMER2_LR  0x020 /* timer2 load register */
#define SP804_TIMER2_VAL 0x024 /* timer2 value register */
#define SP804_TIMER2_CR  0x028 /* timer2 control register */
#define SP804_TIMER2_IC  0x02C /* timer2 interrupt clear register */
#define SP804_TIMER2_RIS 0x030 /* timer2 raw interrupt status register */
#define SP804_TIMER2_MIS 0x034 /* timer2 masked interrupt status register */
#define SP804_TIMER2_BGL 0x038 /* timer2 background load register */

/* SP804 timer control/status bits */
#define SP804_CR_TEN     (1 << 7) /* timer[1-2] enable bit */
#define SP804_CR_TPR     (1 << 6) /* timer[1-2] periodic mode */
#define SP804_CR_IEN     (1 << 5) /* timer[1-2] interrupts enabled */
#define SP804_CR_PRES16  (1 << 2) /* timer[1-2] prescale, divide by 16 */
#define SP804_CR_PRES256 (1 << 3) /* timer[1-2] prescale, divide by 256 */
#define SP804_CR_TSZ_32B (1 << 1) /* timer[1-2] 32bit counter */
#define SP804_CR_OSM     (1 << 0) /* timer[1-2] one-shot mode */

#endif /* !SP804_H */