/* CPU functions for xBoot
 *
 * Copyright 2013, winocm. <winocm@icloud.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *   Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 *   Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 *   If you are going to use this software in any form that does not involve
 *   releasing the source to this project or improving it, let me know beforehand.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <arm/asm.h>

/**
 * locore_jump_to
 *
 * Shut down the bootloader and start the new OS image.
 */
EnterARM(locore_jump_to)
    cpsid   if, #0x13

#ifdef CORTEX_A8
    /* Disable L2 cache */
    mov     r9, r0
    mov     r10, r1

    mrc     p15, 0, r0, c1, c0, 1   /* read Auxiliary Control Register */
    bic     r0, r0, #0x00000002     /* disable L2 cache */
    mcr     p15, 0, r0, c1, c0, 1   /* store Auxiliary Control Register */
#endif

    /* Disable caching entirely. */
    mrc     p15, 0, r0, c1, c0, 0
    bic     r0, r0, #0x00002300
    bic     r0, r0, #0x00000005
    bic     r0, r0, #0x00001000
    bic     r0, r0, #(1 << 2)
    mcr     p15, 0, r0, c1, c0, 0

    /* Disable VFP/SIMD */
    mov     r0, #0x00000000
    mcr     p10, #0x7, r0, c8, c0, #0

    /* Disable I-cache */
    mrc     p15, 0, r0, c1, c0, 2
    bic     r0, r0, #0x00f00000
    mcr     p15, 0, r0, c1, c0, 2

    /* Clear caches. */
    mov     r0, #0
    mcr     p15, 0, r0, c7, c5, 0
    mov     r0, #0
    mcr     p15, 0, r0, c7, c5, 4

    /* Disable MMU */
    mrc     p15, 0, r0, c1, c0, 0
    bic     r0, r0, #1
    mcr     p15, 0, r0, c1, c0, 0

    /* Clear prefetch buffer */
    mov     r0, #0
    mcr     p15, 0, r0, c7, c5, 0
    mov     r0, #0
    mcr     p15, 0, r0, c7, c5, 4

    isb     sy
    dsb     sy

    /* Point of no return */
    mov     lr, r9
    mov     r0, r1
    bx      lr
