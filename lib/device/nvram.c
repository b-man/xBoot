/* NVRAM api
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
#include <limits.h>
#include <stdbool.h>

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
nvram_variable_node_t *nvram_create_node(const char *name, const char *setting)
{
    nvram_variable_node_t *node = malloc(sizeof(nvram_variable_node_t));
    bzero(node, sizeof(nvram_variable_node_t));

    if ((strlen(name) >= NAME_MAX) || (strlen(setting) >= NAME_MAX)) {
	printf("NVRAM Error: length of variable name or value too large.\n");

	return NULL;
    }

    node->next = NULL;
    strncpy(node->value.name, name, strlen(name));
    strncpy(node->value.setting, setting, strlen(setting));
    node->value.attr = nv_attr_u;

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
            if (nvram_get_attribute(list, name) != nv_attr_p) {
                bzero(current->value.setting, strlen(current->value.setting));
                strncpy(current->value.setting, setting, strlen(setting));
                current->value.attr |= nv_attr_m;

                return;
            } else {
                printf("NVRAM Error: variable \'%s\' is protected.\n", name);
		return;
            }
        }
        current = current->next;
    }

    node = nvram_create_node(name, setting);

    if (node != NULL)
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
            if (nvram_get_attribute(list, name) != nv_attr_p) {
                nvram_remove_node(list, current);
                return 0;
            } else {
                printf("NVRAM Error: variable \'%s\' is protected.\n", name);
                return -1;
            }
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
 * nvram_dump
 *
 * Dump values and states of nvram variables.
 */
void nvram_dump(nvram_variable_list_t *list, const char *name)
{
    int attr;
    bool search_mode = false;
    nvram_variable_node_t *current = list->head;
    nvram_variable_t *var = nvram_read_variable_info(list, name);

    if (var != NULL)
        search_mode = true;

    while (current != NULL) {
        if (search_mode) {
            attr = nvram_get_attribute(list, var->name);
        } else {
            attr = nvram_get_attribute(list, current->value.name);
        }

	switch (attr) {
            case nv_attr_m:
                printf("M ");
                break;
            case nv_attr_p:
                printf("P ");
                break;
            case nv_attr_u:
            default:
                printf("  ");
                break;
	}

        if (search_mode) {
            printf("%s = \"%s\"\n", var->name, var->setting);
            break;
        } else {
            printf("%s = \"%s\"\n", current->value.name, current->value.setting);
        }

        current = current->next;
    }

    return;
}

/**
 * nvram_set_attribute
 *
 * Set attributes for an nvram variable.
 */
int nvram_set_attribute(nvram_variable_list_t *list, const char *name, nvram_attr_t attr)
{
    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        if (strcmp(current->value.name, name) == 0) {
            current->value.attr |= attr;
            return 0;
        }

        current = current->next;
    }

    return -1;
}

/**
 * nvram_get_attribute
 *
 * Get attributes for an nvram variable.
 */
int nvram_get_attribute(nvram_variable_list_t *list, const char *name)
{
    nvram_variable_node_t *current = list->head;

    while (current != NULL) {
        if (strcmp(current->value.name, name) == 0)
            return (int)current->value.attr;

        current = current->next;
    }

    return -1;
}
