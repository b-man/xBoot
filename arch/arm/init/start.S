/* Start of xBoot
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

.arm
.code 32

.section .init,"ax"
.align 4

/**
 * ARM Vector Table
 *
 * The system will map this to 0x0 during platform initialization and relocate
 * and restart the image if necessary.
 */
.globl  _ExceptionVectorsBase
_ExceptionVectorsBase:
    b       _start              // reset
    ldr     pc, [pc, #24]       // undef
    ldr     pc, [pc, #24]       // swi
    ldr     pc, [pc, #24]       // prefetch
    ldr     pc, [pc, #24]       // data abort
    ldr     pc, [pc, #24]       // dataexc
    ldr     pc, [pc, #24]       // irq
    ldr     pc, [pc, #24]       // fiq

_vectorTable:
    .long   _start
    .long   _bleh_undef
    .long   _bleh_swi
    .long   _bleh_prefabt
    .long   _bleh_dataabt
    .long   _bleh_dataexc
    .long   _bleh_irq
    .long   _bleh_fiq

/**
 * The point of this bootloader is to copy the kernel to the proper
 * physical address and start it.
 */
EnterARM(start)
    /* We are in supervisor, no interrupts. */
    cpsid   if, #0x13

    /* Check sure we're at the right load address */
    ldr    r4, =_ExceptionVectorsBase
    adr    r5, _ExceptionVectorsBase
    cmp    r4, r5
    beq    L_relocated

    /* We're not at the right load address, relocate ourselves to the right address. */
    mov    r6, #(1 * 1024 * 1024)	// HACK
L_relocate_loop:
    ldr    r8, [r5], #4
    str    r8, [r4], #4
    subs   r6, r6, #4
    bgt    L_relocate_loop

    /* We're done relocating, jump back into the image. */
    ldr    r4, =_ExceptionVectorsBase
    bx     r4

L_relocated:

    /* Zero out the bss section. */
    ldr    r4, =__sbss
    ldr    r5, =__ebss
    cmp    r4, r5
    beq    L_initstack
    sub    r6, r5, r4
    mov    r8, #0
L_zero:
    strb   r8, [r4], #1
    subs   r6, r6, #1
    bne    L_zero

L_initstack:

    /* Initialize stack and frame pointers. */
    ldr    sp, =__stack_end
    mov    r7, #0

    /*
     * Go to xboot bootstrap now.
     */
    bl     xboot_init

.text

/**
 * failure
 *
 * This function is called when we fail core initialization. :(
 */
EnterARM(failure)
EnterARM(locore_halt_system)
    cpsid   if
    b       .
