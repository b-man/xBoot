/*
 * Startup routine for xBoot
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

#include <stdlib.h>
#include <string.h>

#include <boot/bsp.h>
#include <boot/aslr.h>
#include <boot/boot.h>
#include <boot/dtre.h>
#include <boot/address.h>
#include <boot/globals.h>

boot_args gBootArgs;

extern const char __xBoot_version[];
extern const char __xBoot_build_version[];

/**
 * init_boot_args
 *
 * Initialize kernel boot-args
 */
static void init_boot_args(void)
{
	bzero((void *)&gBootArgs, sizeof(boot_args));

	gBootArgs.Revision = kBootArgsRevision;
	gBootArgs.Version = kBootArgsVersion2;

	gBootArgs.physBase = DRAM_BASE;
	gBootArgs.memSize = DRAM_SIZE;

	/* Use a random, page-aligned address. */
	gBootArgs.virtBase = calc_aslr_virtbase(KERNEL_VMADDR);
}

/**
 * init_build_info
 *
 * Initialize exported built-time information
 */
static void init_build_info(void)
{
	setenv_protect("build-style", BUILD_STYLE);
	setenv_protect("build-version", __xBoot_version);
}

/**
 * xboot_init
 *
 * Main xBoot initialization routine
 */
void xboot_init(void)
{
	/* Initialize environment */
	env_init();

	/* Initialize build version info */
	init_build_info();

	/* Initialize BSP */
	bsp_init();

	/* Initialize device tree */
	dtre_init();

	/* Initialize boot-args */
	init_boot_args();

	/* Jump to main */
	xboot_main();
}
