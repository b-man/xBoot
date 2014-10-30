/* ARM divide support
 *
 * Copyright (c) 2008 - 2009, Apple Inc. All rights reserved.
 *
 * This program and the accompanying materials
 * are licensed and made available under the terms and conditions of the BSD License
 * which accompanies this distribution.  The full text of the license may be found at
 * http://opensource.org/licenses/bsd-license.php
 *
 * THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
 * WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.
 *
 * This code is borrowed from the Tianocore Project
 *
 */

#include <arm/asm.h>

EnterARM(_aeabi_uidiv)
EnterARM(_aeabi_uidivmod)
    rsbs    r12, r1, r0, lsr #4
    mov     r2, #0
    bcc     __arm_div4
    rsbs    r12, r1, r0, lsr #8
    bcc     __arm_div8
    mov     r3, #0
    B       __arm_div_large

EnterARM(_aeabi_idiv)
EnterARM(_aeabi_idivmod)
    orrs    r12, r0, r1
    bmi     __arm_div_negative
    rsbs    r12, r1, r0, lsr #1
    mov     r2, #0
    bcc     __arm_div1
    rsbs    r12, r1, r0, lsr #4
    bcc     __arm_div4
    rsbs    r12, r1, r0, lsr #8
    bcc     __arm_div8
    mov     r3, #0
    b       __arm_div_large
__arm_div8:
    rsbs    r12, r1, r0, lsr #7
    subcs   r0, r0, r1, lsl #7
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #6
    subcs   r0, r0, r1, lsl #6
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #5
    subcs   r0, r0, r1, lsl #5
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #4
    subcs   r0, r0, r1, lsl #4
    adc     r2, r2, r2
__arm_div4:
    rsbs    r12, r1, r0, lsr #3
    subcs   r0, r0, r1, lsl #3
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #2
    subcs   r0, r0, r1, lsl #2
    adcs    r2, r2, r2
    rsbs    r12, r1, r0, lsr #1
    subcs   r0, r0, r1, lsl #1
    adc     r2, r2, r2
__arm_div1:
    subs    r1, r0, r1
    movcc   r1, r0
    adc     r0, r2, r2
    bx      r14
__arm_div_negative:
    ands    r2, r1, #0x80000000
    rsbmi   r1, r1, #0
    eors    r3, r2, r0, asr #32
    rsbcs   r0, r0, #0
    rsbs    r12, r1, r0, lsr #4
    bcc     label1
    rsbs    r12, r1, r0, lsr #8
    bcc     label2
__arm_div_large:
    lsl     r1, r1, #6
    rsbs    r12, r1, r0, lsr #8
    orr     r2, r2, #0xfc000000
    bcc     label2
    lsl     r1, r1, #6
    rsbs    r12, r1, r0, lsr #8
    orr     r2, r2, #0x3f00000
    bcc     label2
    lsl     r1, r1, #6
    rsbs    r12, r1, r0, lsr #8
    orr     r2, r2, #0xfc000
    orrcs   r2, r2, #0x3f00
    lslcs   r1, r1, #6
    rsbs    r12, r1, #0
    bcs     __aeabi_idiv0
label3:
    lsrcs   r1, r1, #6
label2:
    rsbs    r12, r1, r0, lsr #7
    subcs   r0, r0, r1, lsl #7
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #6
    subcs   r0, r0, r1, lsl #6
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #5
    subcs   r0, r0, r1, lsl #5
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #4
    subcs   r0, r0, r1, lsl #4
    adc     r2, r2, r2
label1:
    rsbs    r12, r1, r0, lsr #3
    subcs   r0, r0, r1, lsl #3
    adc     r2, r2, r2
    rsbs    r12, r1, r0, lsr #2
    subcs   r0, r0, r1, lsl #2
    adcs    r2, r2, r2
    bcs     label3
    rsbs    r12, r1, r0, lsr #1
    subcs   r0, r0, r1, lsl #1
    adc     r2, r2, r2
    subs    r1, r0, r1
    movcc   r1, r0
    adc     r0, r2, r2
    asrs    r3, r3, #31
    rsbmi   r0, r0, #0
    rsbcs   r1, r1, #0
    bx      r14

    /* What to do about division by zero?  For now, just return. */
__aeabi_idiv0:
    bx      r14
