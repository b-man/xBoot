/* xBoot interactive shell.
 *
 * Copyright 2018, Brian McKenzie. <mckenzba@gmail.com>
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
#include <limits.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>

#include <panic.h>
#include <shell/history.h>


static history_entry_t *recall;
static history_queue_t *history;

/**
 * history_create_entry
 *
 * Create an entry to be added to the history queue.
 */
static history_entry_t *history_create_entry(const char *line, size_t size)
{
	size_t line_size;
	history_entry_t *entry;

	if (line == NULL)
		return NULL;

	if ((size < 1) || (size > LINE_MAX))
		return NULL;

	/* Exclude newline from line length */
	line_size = size;
	for (int i = 0; i < line_size; i++) {
		if (*(line + i) == '\n') {
			line_size = i;
			break;
		}
	}

	/* Skip empty lines */
	if (line_size == 0)
		return NULL;

	if ((entry = malloc(sizeof(history_entry_t))) == NULL) {
		panic("SHELL Error: failed to allocate space for history entry.\n");

		/* NOREACH */
		return NULL;
	}

	entry->count = 0;
	entry->next = NULL;
	entry->last = NULL;
	entry->size = line_size;

	if ((entry->buffer = malloc(line_size + 1)) == NULL) {
		panic("SHELL Error: failed to allocate space for history buffer.\n");

		/* NOREACH */
		return NULL;
	}

	strncpy(entry->buffer, line, line_size);
	*(entry->buffer + line_size) = '\0';

	return entry;
}

/**
 * history_lookup_prev_entry
 *
 * Walk up the history queue and return the last entry.
 */
static history_entry_t *history_lookup_last_entry(history_entry_t *current)
{
	if (current == NULL)
		return history->tail;

	if (current == history->head)
		return current;

	return current->last;
}

/**
 * history_lookup_next_entry
 *
 * Walk down the history queue and return the next entry.
 */
static history_entry_t *history_lookup_next_entry(history_entry_t *current)
{
	if (current == NULL)
		return NULL;

	if (current->next == NULL)
		return current;

	return current->next;
}

/**
 * history_push_entry
 *
 * Push an entry into the history queue.
 */
static void history_push_entry(history_queue_t *queue, history_entry_t *entry)
{
	history_entry_t *last = NULL;

	if ((queue == NULL) || (entry == NULL))
		return;

	/* Drop consecutive duplicate enteries */
	last = history_lookup_last_entry(last);
	if (strcmp(last->buffer, entry->buffer) == 0) {
		free(entry->buffer);
		free(entry);

		return;
	}

	queue->count++;
	entry->count = queue->count;

	if ((queue->head == NULL) && (queue->tail == NULL)) {
		queue->head = queue->tail = entry;

		return;
	}

	entry->last = queue->tail;
	queue->tail->next = entry;
	queue->tail = entry;

	return;
}

/**
 * history_pop_entry
 *
 * Pop an entry off of the start of the history queue.
 */
static void history_pop_entry(history_queue_t *queue)
{
	history_entry_t *old_head;
	history_entry_t *new_head;

	if (queue == NULL)
		return;

	if ((queue->head == NULL) && (queue->tail == NULL))
		return;

	old_head = queue->head;
	new_head = old_head->next;
	queue->head = new_head;

	free(old_head->buffer);
	free(old_head);

	if (queue->head == NULL)
		queue->tail = queue->head;

	return;
}

/**
 * shell_history_init
 *
 * Initialize the shell history queue.
 */
int shell_history_init(size_t depth)
{
	if (depth == 0)
		return -1;

	if ((history = malloc(sizeof(history_queue_t))) == NULL) {
		panic("SHELL Error: failed to allocate history queue.\n");

		/* NOREACH */
		return -1;
	}

	history->count = 0;
	history->depth = depth;
	history->head = history->tail = NULL;

	return 0;
}

/**
 * shell_history_push
 *
 * Push line input into the history queue.
 */
int shell_history_push(const char *line, size_t size)
{
	history_entry_t *entry;

	if (line == NULL)
		return -1;

	if ((size < 1) || (size > LINE_MAX))
		return -1;

	if (history->count >= history->depth)
		history_pop_entry(history);

	entry = history_create_entry(line, size);
	history_push_entry(history, entry);

	/* Reset history recall context */
	recall = NULL;

	return 0;
}

/**
 * shell_history_print
 *
 * Print out the input line history in chronological order.
 */
int shell_history_print(void)
{
	history_entry_t *entry;

	if (history == NULL)
		return 1;

	for (entry = history->head; entry != NULL; entry = entry->next)
		printf("  %2d %s\n", entry->count, entry->buffer);

	return 0;
}

/**
 * shell_history_lookup_last_line
 *
 * Look up the last entry and update the current line.
 */
char *shell_history_last_line(char *lp, size_t *size)
{
	size_t line_size = *size;

	if ((line_size < 0) || (line_size > LINE_MAX))
		return lp;

	while (line_size-- > 0) {
		printf("\b \b");
		lp--;
	}

	recall = history_lookup_last_entry(recall);
	if (recall == NULL)
		return lp;

	for (int i = 0; i < recall->size; i++)
		*lp++ = *(recall->buffer + i);

	*size = recall->size;
	printf("%s", recall->buffer);

	return lp;
}

/**
 * shell_history_lookup_last_line
 *
 * Look up the next entry and update the current line.
 */
char *shell_history_next_line(char *lp, size_t *size)
{
	size_t line_size = *size;

	if ((line_size < 0) || (line_size > LINE_MAX))
		return lp;

	while (line_size-- > 0) {
		printf("\b \b");
		lp--;
	}

	/* Return a blank line after stepping over the latest entry. */
	if (recall == history->tail) {
		*size = 0;
		recall = NULL;

		return lp;
	}

	recall = history_lookup_next_entry(recall);
	if (recall == NULL)
		return lp;

	for (int i = 0; i < recall->size; i++)
		*lp++ = *(recall->buffer + i);

	*size = recall->size;
	printf("%s", recall->buffer);

	return lp;
}
