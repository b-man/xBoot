/* ASM helper functions
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

#ifndef ARM_ASM_H
#define ARM_ASM_H

#ifdef __ASSEMBLY__

/*
 * LLVM-AS is a horrible sin upon humanity.
 */
#define LoadConstantToReg(_Data, _Reg)  \
  ldr  _Reg, [pc, #0]   ;               \
  b    1f               ;               \
  .long (_Data)         ;               \
1:

/*
 * Procedure enter and exit for both ARM and thumb states.
 */
#define EnterARM(function)              \
    .code 32                        ;   \
    .globl function                 ;   \
    .globl _ ##function             ;   \
    .align  4                       ;   \
    function:                       ;   \
    _##function:                    ;   \

#define EnterThumb(function)        ;   \
    .code 16                        ;   \
    .thumb_func _ ##function        ;   \
    .globl function                 ;   \
    .globl _ ##function             ;   \
    .align  4                       ;   \
    function:                       ;   \
    _##function:                    ;   \

#define EnterARM_NoAlign(function)      \
    .code 32                        ;   \
    .globl function                 ;   \
    .globl _ ##function             ;   \
    function:                       ;   \
    _##function:                    ;   \

#define EnterThumb_NoAlign(function)    \
    .code 16                        ;   \
    .thumb_func _ ##function        ;   \
    .globl function                 ;   \
    .globl _ ##function             ;   \
    function:                       ;   \
    _##function:                    ;   \

/*
 * ARM atomic function helper macro.
 */
#define AtomicMachine(name, instr)  \
    EnterARM(hw_atomic_ ##name)     \
        mov     r12, r0         ;   \
    try_ ##name:                ;   \
        ldrex   r0, [r12]       ;   \
        instr   r0, r0, r1      ;   \
        strex   r3, r0, [r12]   ;   \
        movs    r3, r3          ;   \
        bxeq    lr              ;   \
        b       try_ ##name     ;

/*
 * Thread stuff.
 */

#define LoadLockHardwareRegister(register) \
    mrc     p15, 0, register, c13, c0, 4

#define LoadThreadRegister(register) \
    mrc     p15, 0, register, c13, c0, 4

#define IncrementPreemptLevel(register, scratch)                      \
    ldr     scratch, [register, MACHINE_THREAD_PREEMPT_COUNT]     ;   \
    add     scratch, scratch, #1                                  ;   \
    str     scratch, [register, MACHINE_THREAD_PREEMPT_COUNT]     ;

#define DecrementPreemptLevel(register, scratch)                      \
    ldr     scratch, [register, MACHINE_THREAD_PREEMPT_COUNT]     ;   \
    sub     scratch, scratch, #1                                  ;   \
    str     scratch, [register, MACHINE_THREAD_PREEMPT_COUNT]     ;

#else
#error You should only include this file if you are building an ASM file.
#endif /* !__ASSEMBLY__ */

#endif /* !ARM_ASM_H */
