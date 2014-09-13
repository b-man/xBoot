/* ARM exception handler code.
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
 * bleh_undef
 *
 * Undefined instruction handler.
 */
EnterARM(bleh_undef)
    /* Supervisor. */
    cpsid   i, #0x13
    sub     sp, sp, #0x50
    stmea   sp, {r0-r12}
    str     lr, [sp, #0x38]
    mov     r0, sp

    /* Undefined. */
    cpsid   i, #0x1b
    mrs     r2, spsr
    str     lr, [r0, #0x3C]
    str     r2, [r0, #0x40]

    /* Supervisor again. */
    cpsid   i, #0x13
    add     r3, sp, #0x50
    str     r3, [r0, #0x34]
    blx     sleh_undef

    bl      _locore_halt_system

/**
 * bleh_swi
 *
 * System call handler.
 */
EnterARM(bleh_swi)
    bl      _locore_halt_system

/**
 * bleh_dataexc
 *
 * ????
 */
EnterARM(bleh_dataexc)
    bl      _locore_halt_system

/**
 * bleh_fiq
 *
 * FIQ handler.
 */
EnterARM(bleh_fiq)
    bl      _locore_halt_system

/**
 * bleh_irq
 *
 * IRQ handler.
 */
EnterARM(bleh_irq)
    bl      _locore_halt_system

/**
 * bleh_prefabt
 *
 * Prefetch abort handler.
 */
EnterARM(bleh_prefabt)
    /* Supervisor. */
    cpsid   i, #0x13

    /* Save registers. */
    sub     sp, sp, #0x50
    stmea   sp, {r0-r12}
    str     lr, [sp, #0x38]
    mov     r12, sp

    /* Abort. */
    cpsid   i, #0x17

    /* Save more. */
    str     lr, [r12, #0x3C]
    str     lr, [r12, #0x48]

    /* Save DFSR. */
    mrc     p15, 0, r5, c5, c0, 0
    str     r5, [r12, #0x44]

    /* Save SPSR. */
    mrs     r4, spsr
    str     r4, [r12, #0x40]

    /* supervisor */
    cpsid	i, #0x13

    add     r12, r12, #0x50
    str     r12, [sp, #0x34]
    sub     r12, r12, #0x50
    mov     r0, sp
    mov     r1, #3      /* Prefetch Abort */
    bl      sleh_abort

    bl      _locore_halt_system

/**
 * bleh_dataabt
 *
 * Data abort handler.
 */
EnterARM(bleh_dataabt)
    /* supervisor */
    cpsid	i, #0x13

    /* make space on the stack for the registers. */
    sub     sp, sp, #0x50
    stmea   sp, {r0-r12}

    /* Save the remaining registers. */
    str     lr, [sp, #0x38]
    mov     r12, sp

    /* abort mode */
    cpsid	i, #0x17

    /* Save more registers */
    str     lr, [r12, #0x3C]
    mrs     r4, spsr
    str     r4, [r12, #0x40]

    /* supervisor */
    cpsid	i, #0x13

    /* Get DFSR */
    mrc     p15, 0, r5, c5, c0, 0

    /* Get FAR */
    mrc     p15, 0, r6, c6, c0, 0

    /* Save them on the stack */
    str     r5, [sp, #0x44]
    str     r6, [sp, #0x48]

    /* Go to abort handler */
    add     r12, r12, #0x50
    str     r12, [sp, #0x34]
    sub     r12, r12, #0x50
    mov     r0, sp
    mov     r1, #4      // Data Abort
    bl      sleh_abort

    bl      _locore_halt_system
