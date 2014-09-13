/* ARM exception trap handlers
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

#include <stdio.h>
#include <assert.h>
#include <panic.h>
#include <arm/traps.h>

/**
 * sleh_abort
 *
 * Handle prefetch and data aborts.
 */
void sleh_abort(abort_information_context_t* abort_context, int reason)
{
	  assert(abort_context);

    switch(reason) {
        case SLEH_ABORT_TYPE_PREFETCH_ABORT: {
            /* Print out the long long nice prefetch abort. */
            printf("sleh_abort: prefetch abort type %d: fault_addr=0x%x\n"
                          "r0: 0x%08x  r1: 0x%08x  r2: 0x%08x  r3: 0x%08x\n"
                          "r4: 0x%08x  r5: 0x%08x  r6: 0x%08x  r7: 0x%08x\n"
                          "r8: 0x%08x  r9: 0x%08x r10: 0x%08x r11: 0x%08x\n"
                          "12: 0x%08x  sp: 0x%08x  lr: 0x%08x  pc: 0x%08x\n"
                          "cpsr: 0x%08x fsr: 0x%08x far: 0x%08x\n",
                          reason, abort_context->far,
                          abort_context->gprs[0], abort_context->gprs[1], abort_context->gprs[2], abort_context->gprs[3],
                          abort_context->gprs[4], abort_context->gprs[5], abort_context->gprs[6], abort_context->gprs[7],
                          abort_context->gprs[8], abort_context->gprs[9], abort_context->gprs[10], abort_context->gprs[11],
                          abort_context->gprs[12], abort_context->sp, abort_context->lr, abort_context->pc,
                          abort_context->cpsr, abort_context->fsr, abort_context->far
                          );
 			      break;
        }
        case SLEH_ABORT_TYPE_DATA_ABORT: {
            /* Print out the long long nice data abort. */
            printf("sleh_abort: data abort type %d: fault_addr=0x%x\n"
                       "r0: 0x%08x  r1: 0x%08x  r2: 0x%08x  r3: 0x%08x\n"
                       "r4: 0x%08x  r5: 0x%08x  r6: 0x%08x  r7: 0x%08x\n"
                       "r8: 0x%08x  r9: 0x%08x r10: 0x%08x r11: 0x%08x\n"
                       "12: 0x%08x  sp: 0x%08x  lr: 0x%08x  pc: 0x%08x\n"
                       "cpsr: 0x%08x fsr: 0x%08x far: 0x%08x\n",
                       reason, abort_context->far,
                       abort_context->gprs[0], abort_context->gprs[1], abort_context->gprs[2], abort_context->gprs[3],
                       abort_context->gprs[4], abort_context->gprs[5], abort_context->gprs[6], abort_context->gprs[7],
                       abort_context->gprs[8], abort_context->gprs[9], abort_context->gprs[10], abort_context->gprs[11],
                       abort_context->gprs[12], abort_context->sp, abort_context->lr, abort_context->pc,
                       abort_context->cpsr, abort_context->fsr, abort_context->far
                       );
 			      break;
        }
        default:
            panic("sleh_abort: unknown abort called (context: 0x%08x, reason: %d)!\n", abort_context, reason);
    }

	  panic("System Abort\n");
}

/**
 * sleh_undef
 *
 * Handle undefined instructions and VFP usage.
 */
void sleh_undef(abort_information_context_t* state)
{
	  assert(state);
    printf("sleh_undef: undefined instruction\n"
                  "r0: 0x%08x  r1: 0x%08x  r2: 0x%08x  r3: 0x%08x\n"
                  "r4: 0x%08x  r5: 0x%08x  r6: 0x%08x  r7: 0x%08x\n"
                  "r8: 0x%08x  r9: 0x%08x r10: 0x%08x r11: 0x%08x\n"
                  "12: 0x%08x  sp: 0x%08x  lr: 0x%08x  pc: 0x%08x\n"
                  "cpsr: 0x%08x\n",
                  state->gprs[0], state->gprs[1], state->gprs[2], state->gprs[3],
                  state->gprs[4], state->gprs[5], state->gprs[6], state->gprs[7],
                  state->gprs[8], state->gprs[9], state->gprs[10], state->gprs[11],
                  state->gprs[12], state->sp, state->lr, state->pc, state->cpsr);
	  panic("Undefined Instruction\n");
}
