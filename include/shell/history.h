#ifndef SHELL_HISTROY_H
#define SHELL_HISTORY_H

#include <sys/types.h>

#define DEFAULT_HISTORY_DEPTH 10

typedef struct history_entry {
	char *buffer;
	size_t count;
        struct history_entry *next;
} history_entry_t;

typedef struct history_queue {
        size_t depth;
        size_t count;
        history_entry_t *head;
        history_entry_t *tail;
} history_queue_t;

extern int shell_history_init(size_t depth);
extern int shell_history_push(const char *line);
extern int shell_history_print(void);

#endif /* SHELL_HISTORY_H */
