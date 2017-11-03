/*
 * Copyright 2013, winocm. <winocm@icloud.com>
 * Copyright 2017, Brian McKenzie. <mckenzba@gmail.com>
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
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <panic.h>
#include <assert.h>

#include <sys/types.h>
#include <sys/align.h>

#include <boot/dtre.h>
#include <boot/image3.h>
#include <boot/globals.h>
#include <boot/boot_args.h>
#include <boot/device_tree.h>
#include <boot/memory_region.h>

static memory_region_t kernel_region;

/**
 * publish_device_tree
 *
 * Publish the device tree.
 */
void publish_device_tree(void)
{
    void *dtre_data;
    uint32_t dtre_length;
    Node *root, *chosen, *options;

    root = dtre_root_node();
    chosen = dtre_find_node("/chosen", 1);
    options = dtre_find_node("/options", 1);

    assert(root != NULL);
    assert(chosen != NULL);
    assert(options != NULL);

    /* Insert fake iBoot data
     * TODO: Use real values.
     */
    uint32_t one = 1;
    uint32_t chip = 0x8900;
    uint64_t ecid = 0xEFBEEFBEEFBEEFBE;
    char *region_info = "JP/A";
    char *platform_name = "ARM Hexley";
    char *model_number = "ARMHXL123";
    char *serial_number = "ARMHXL456";
    char *mlb_serial_number = "MB123";
    char *secure_root_prefix = "md";
    char *firmware_version = "xBoot-0.1~1";

    dtre_create_node(chosen, "debug-enabled", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "production-cert", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "system-trusted", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "secure-boot", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "gid-aes-key", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "uid-aes-key", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "development-cert", &one, sizeof(uint32_t));
    dtre_create_node(chosen, "chip-id", &chip, sizeof(uint32_t));
    dtre_create_node(chosen, "die-id", &ecid, sizeof(uint64_t));
    dtre_create_node(chosen, "unique-chip-id", &ecid, sizeof(uint64_t)); 

    dtre_create_node(root, region_info, &region_info, sizeof(region_info));
    dtre_create_node(root, platform_name, &platform_name, sizeof(platform_name));
    dtre_create_node(root, model_number, &model_number, sizeof(model_number));
    dtre_create_node(root, serial_number, &serial_number, sizeof(serial_number));
    dtre_create_node(root, mlb_serial_number, &mlb_serial_number, sizeof(mlb_serial_number));
    dtre_create_node(root, secure_root_prefix, &secure_root_prefix, sizeof(secure_root_prefix));
    dtre_create_node(root, firmware_version, &firmware_version, sizeof(firmware_version));

    /* Flatten the device tree. */
    dtre_length = dtre_get_size();
    dtre_data = memory_region_reserve(&kernel_region, dtre_length, 0);
    dtre_flatten(dtre_data, dtre_length);

    /* Add boot-args entry. */
    gBootArgs.deviceTreeLength = dtre_length;
    gBootArgs.deviceTreeP = dtre_data;

    return;
}

/**
 * publish_boot_args
 *
 * Publish boot-args.
 */
boot_args *publish_boot_args(void)
{
    char *cmdline;
    boot_args *args;

    gBootArgs.topOfKernelData = align_up(kernel_region.pos, 0x100000);

    cmdline = getenv("bootargs");
    strncpy(gBootArgs.commandLine, cmdline, (BOOT_LINE_LENGTH - 1));

    printf("gBootArgs.commandLine = [%s]\n", gBootArgs.commandLine);

    /* Allocate kernel memory for this. */
    args = (boot_args *)memory_region_reserve(&kernel_region,
                                            sizeof(boot_args), 1024);
    bcopy(&gBootArgs, args, sizeof(boot_args));

    return args;
}

/**
 * start_darwin
 *
 * Start the Darwin kernel.
 */
void start_darwin(void)
{
    boot_args *args;

    publish_device_tree();

    /*
     * Initialize the kernel memory region.
     *
     * The kernel is always mapped to eDRAM + PAGE_SIZE. This is done
     * for sleep support. (eventually.)
     */
    kernel_region.pos = kernel_region.base = (gBootArgs.physBase + 0x1000);

    /* Zero out first 64MB of RAM. */
    printf("- Preparing system...\n");
    bzero((void *)gBootArgs.physBase, (64 * 1024 * 1024));

    args = publish_boot_args();
    assert(args != NULL);

    panic("TODO: load kernel, ramdisk, and boot.\n");

    return;
}
