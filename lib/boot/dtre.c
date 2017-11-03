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

#include <sys/types.h>

#include <boot/image3.h>
#include <boot/device_tree.h>

extern int parse_jsdt_device_tree(const char *raw);

/**
 * dtre_allocate_memory_range
 *
 * Allocate a memory range into the device tree.
 */
int dtre_allocate_memory_range(char *name, long start, long length, long type)
{
    Node *memory_map;
    char *name_buffer;
    uint32_t *value_buffer;

    memory_map = DT__FindNode("/chosen/memory-map", 1);

    name_buffer = malloc(strlen(name) + 1);
    if (name_buffer == 0)
        return -1;

    strcpy(name_buffer, name);

    value_buffer = malloc(2 * sizeof(uint32_t));
    if (value_buffer == 0)
        return -1;

    value_buffer[0] = start;
    value_buffer[1] = length;

    DT__AddProperty(memory_map, name_buffer, 2 * sizeof(uint32_t), (char *)value_buffer);

    return 0;
}

/**
 * dtre_create_node
 *
 * Puts a node into the device tree.
 */
int dtre_create_node(Node *node, char *name, void* datap, int size)
{
    char *name_bufer;

    name_bufer = malloc(strlen(name) + 1);
    if (name_bufer == NULL)
        return -1;

    strcpy(name_bufer, name);

    DT__AddProperty(node, name_bufer, size, (char *)datap);

    return 0;
}

/**
 * dtre_root_node
 *
 * Returns a pointer to the root node of the device tree.
 */
Node *dtre_root_node(void)
{
    return DT__RootNode();
}

/**
 * dtre_find_node
 *
 * Searches the device tree for the requested node.
 */
Node *dtre_find_node(const char *path, bool create)
{
    return DT__FindNode(path, create);
}

/**
 * dtre_get_size
 *
 * Returns the size of the device tree in bytes.
 */
uint32_t dtre_get_size(void)
{
    uint32_t size;

    DT__GetDeviceTreeSize(&size);

    return size;
}

/**
 * dtre_flatten
 *
 * Flatten the device tree.
 */
void dtre_flatten(void *data, uint32_t length)
{
    return DT__FlattenDeviceTree(data, &length);
}

/**
 * dtre_init
 *
 * Initialize the device tree.
 */
int dtre_init(void)
{
    void *dt_image, *dt_data;

    if ((dt_image = image3_get_image(kImage3TypeJsonDeviceTree)) == NULL) {
        panic("Invalid or missing device tree!\n");
        /* NOREACH */
        return -1;
    }

    image3_get_tag_data(dt_image, kImage3TagData, &dt_data, NULL);

    if (!parse_jsdt_device_tree(dt_data)) {
        panic("Failed to parse device tree!\n");
        /* NOREACH */
        return -1;
    }

    return 0;
}
