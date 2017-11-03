/*
 * Copyright 2017, Brian McKenzie <mckenzba@gmail.com>
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

#include <prng.h>

#include <boot/aslr.h>

/* TODO: put these a header */
#if defined(__arm__)
    #define PAGE_SIZE 0x1000
#elif defined(__arm64__)
    #ifdef USE_16K_PAGES
        #define PAGE_SIZE 0x4000
    #else
        #define PAGE_SIZE 0x1000
    #endif
#else
    #error "Unsupported architecture"
#endif

#define round_to_page(x) \
    ((((uint32_t)(x)) + (PAGE_SIZE - 1)) & ~(PAGE_SIZE -1))

static uint32_t get_aslr_slide(void)
{
    int8_t data[1];

    prng_get_random_bytes(data, 1);

    return (uint32_t)data[0];
}

uint32_t calc_aslr_virtbase(uint32_t addr)
{
    uint32_t base;

    base = addr;
    base |= get_aslr_slide();
    round_to_page(base);

    return base;
}
