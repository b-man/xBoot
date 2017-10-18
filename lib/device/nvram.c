/* NVRAM api interface
 *
 * Copyright 2014, Brian McKenzie <mckenzba@gmail.com>
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

#include <device/nvram.h>

/**
 * nvram_initialize_list
 *
 * Initialize linked list to contain nvram variable data.
 */
nvram_variable_list_t *nvram_initialize_list(void)
{
    nvram_variable_list_t *list = malloc(sizeof(nvram_variable_list_t));
    list->head = NULL;
    list->tail = &list->head;

    return list;
}

/**
 * nvram_create_node
 *
 * Create a node to be inserted into linked list.
 */
nvram_variable_node_t *nvram_create_node(const char *name, const char *setting, int overridden)
{
    nvram_variable_node_t *node = malloc(sizeof(nvram_variable_node_t));

    node->next = NULL;
    strncpy(node->value.name, name, 63);
    strncpy(node->value.setting, setting, 255);
    node->value.overridden = overridden;

    return node;
}

/**
 * nvram_append_node
 *
 * Append a node to the end of the linked list.
 */
void nvram_append_node(nvram_variable_list_t *list, nvram_variable_node_t *node)
{
    *list->tail = node;
    list->tail = &node->next;
    node->next = NULL;

    return;
}

/**
 * nvram_remove_node
 *
 * Remove a node from the linked list.
 */
void nvram_remove_node(nvram_variable_list_t *list, nvram_variable_node_t *node)
{
    nvram_variable_node_t *current;
    nvram_variable_node_t **next = &list->head;

    while ((current = *next) != NULL) {
        if (current == node) {
            *next = node->next;

            if (list->tail == &node->next)
                list->tail = next;

            node->next = NULL;
            break;
        }
        next = &current->next;
    }

    return;
}

/**
 * nvram_variable_set
 *
 * Add/override an vnram variable.
 */
void nvram_variable_set(nvram_variable_list_t *list, const char *name, const char *setting)
{
    nvram_variable_node_t *node;

    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        if (strcmp(current->value.name, name) == 0) {
            bzero(current->value.name, 63);
            bzero(current->value.setting, 255);
            strncpy(current->value.name, name, 63);
            strncpy(current->value.setting, setting, 255);
            current->value.overridden = 1;

            return;
        }
        current = current->next;
    }

    node = nvram_create_node(name, setting, 0);
    nvram_append_node(list, node);

    return;
}

/**
 * nvram_variable_unset
 *
 * Unset/erase an nvram variable.
 */
int nvram_variable_unset(nvram_variable_list_t *list, const char *name)
{
    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        if (strcmp(current->value.name, name) == 0) {
            nvram_remove_node(list, current);
            return 0;
        }

        current = current->next;
    }

    return -1;
}

/**
 * nvram_read_variable_info
 *
 * Retrieve information about an nvram variable (such as it's value or if it's been modified).
 */
nvram_variable_t *nvram_read_variable_info(nvram_variable_list_t *list, const char *name)
{
    nvram_variable_t *value = NULL;

    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        if (strcmp(current->value.name, name) == 0)
            value = &current->value;

        current = current->next;
    }

    return value;
}

/**
 * nvram_dump_list
 *
 * Dump a list of all variables in nvram and their associated values and states.
 */
void nvram_dump_list(nvram_variable_list_t *list)
{
    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        printf("%s %s = %s\n", (current->value.overridden ? "M" : " "), current->value.name, current->value.setting);
        current = current->next;
    }

    return;
}
