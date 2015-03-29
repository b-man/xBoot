/* BCM283x System Timer driver - prototypes and definitions
 *
 * Copyright (c) 2015, Brian McKenzie <mckenzba@gmail.com>
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

#ifndef BCM_TIMER_H
#define BCM_TIMER_H

#include <sys/io.h>
#include <sys/types.h>

/* BCM timer configuration structure */
typedef struct {
	addr_t base;
} bcm_timer_cfg;

/* BCM timer driver prototypes */
void bcm_timer_init(void);
void bcm_timer_reset(void);
uint32_t bcm_timer_usec_count(void);

/* BCM Timer registers */
#define BCM_TMR_CS	0x00			/* System timer control/status */
#define BCM_TMR_CLO	0x04			/* System timer counter (lower 32 bits) */
#define BCM_TMR_CHI	0x08			/* System timer counter (higher 32 bits) */
#define BCM_TMR_C(x)	(0x0C + (0x04 * x))	/* System timer compare 0..3 */

#endif /* !BCM_TIMER_H */
