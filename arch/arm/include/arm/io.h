/* ARM-specific register I/O macros, definitions, and operations
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

#ifndef ARM_IO_H
#define ARM_IO_H

#include <sys/types.h>

typedef volatile void* addr_t;

#define writeb(addr, val) __arm_writeb(addr, val)
#define writew(addr, val) __arm_writew(addr, val)
#define writel(addr, val) __arm_writel(addr, val)

#define readb(addr) __arm_readb(addr)
#define readw(addr) __arm_readw(addr)
#define readl(addr) __arm_readl(addr)

static inline void __arm_writeb(addr_t addr, uint8_t val)
{
	asm volatile("strb %1, %0" : "+Qo" (*(volatile uint8_t *)(addr)) : "r" (val));
}

static inline void __arm_writew(addr_t addr, uint16_t val)
{
	asm volatile("strh %1, %0" : "+Qo" (*(volatile uint16_t *)(addr)) : "r" (val));
}

static inline void __arm_writel(addr_t addr, uint32_t val)
{
	asm volatile("str %1, %0" : "+Qo" (*(volatile uint32_t *)(addr)) : "r" (val));
}

static inline uint8_t __arm_readb(const addr_t addr)
{
	uint8_t val;
	asm volatile("ldrb %1, %0" : "+Qo" (*(volatile uint8_t *)(addr)), "=r" (val));

	return val;
}

static inline uint16_t __arm_readw(const addr_t addr)
{
	uint16_t val;
	asm volatile("ldrh %1, %0" : "+Qo" (*(volatile uint16_t *)(addr)), "=r" (val));

	return val;
}

static inline uint32_t __arm_readl(const addr_t addr)
{
	uint32_t val;
	asm volatile("ldr %1, %0" : "+Qo" (*(volatile uint32_t *)(addr)), "=r" (val));

	return val;
}

#endif /* !ARM_IO_H */