/* Memory allocation routines for xBoot
 *
 * Copyright (c) 2017, Brian McKenzie <mckenzba@gmail.com>
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include <sys/types.h>

#include <tlsf/tlsf.h>

static pool_t pool;

extern int __heap_start[];
extern int __heap_end[];

static bool malloc_inited = false;

static void tlsf_init(void)
{
    pool = tlsf_create_with_pool(__heap_start,
        (size_t)(__heap_end - __heap_start));

    malloc_inited = true;
}

void *malloc(size_t size)
{
    if (!malloc_inited)
        tlsf_init();

    return tlsf_malloc(pool, size);
}

void *calloc(size_t nelem, size_t elsize)
{
    void *ptr;

    ptr = malloc(nelem * elsize);

    if (ptr != NULL)
        memset(ptr, 0, (nelem * elsize));

    return ptr;
}

void *memalign(size_t align, size_t size)
{
    if (!malloc_inited)
        tlsf_init();

    return tlsf_memalign(pool, align, size);
}

void *realloc(void *ptr, size_t size)
{
    if (ptr == NULL)
        return NULL;

    return tlsf_realloc(pool, ptr, size);
}

void free(void *ptr)
{
    if (ptr == NULL)
        return;

    return tlsf_free(pool, ptr);
}
