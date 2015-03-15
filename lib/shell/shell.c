/* xBoot interactive shell.
 *
 * Copyright 2015, Brian McKenzie. <mckenzba@gmail.com>
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

#include <shell.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <interface/serial.h>

#define MAXLINE MAX_INPUT	/* Longest line of user input the shell will accept. */

/**
 * shell_getline
 *
 * Read a line of user input and place it in an input buffer.
 */
static void shell_getline(char *buffer, int minlen, int maxlen)
{
	int ch;
	char *lp;
	int ch_count = 0;

	lp = buffer;

	while (1) {
		ch = serial_getc();

		switch (ch) {
			case '\n':
			case '\r':
			    serial_puts("\n");
			    *lp++ = '\0';
			    return;
			case '\b':
			case '\177':
			    if (lp >= (buffer + minlen - 1)) {
				serial_puts("\b \b");
				lp--;
			    }
			    continue;
			default:
			    if ((ch > '\040' || ch < '\177') && ch_count < maxlen) {
			    	serial_putc(ch);
				*lp++ = ch;
				ch_count++;
			    }
		}
	}

	return;
}

/**
 * shell_parseline
 *
 * Parse an input buffer into an argument array and return the argument count.
 */
static int shell_parseline(char *buffer, char **argv)
{
	int argc, nquote;
	char *delim = " \t";
	char *token, *lpr, *lpw, *save;

	nquote = 0;

	if (*buffer == '\0')
		return 0;

	/* If quoted strings are present in the input buffer, we need to save any spaces within
	 * those quoted strings by replacing the spaces with a tag (-1) before those strings
	 * get tokenized. We will also strip out the quotes around those strings as they aren't
	 * needed when they get placed in the argument array.
	 */
	for (lpr = buffer, lpw = buffer; *lpr != '\0'; lpr++) {
		*lpw = *lpr;
		switch (*lpr) {
			case '\042': /* " */
			case '\047': /* ' */
			    if (++nquote == 2)
				nquote = 0;
			    break;
			default:
			    if ((nquote == 1) && (*lpr == ' '))
				*lpw = (char)-1;
			    lpw++;
		}
	}

	*lpw = '\0';

	/* Throw a syntax error for incomplete quoted strings. */
	if (nquote == 1) {
		printf("? syntax error\n");
		return -1;
	}

	/* Populate the argument array by tokenizing the input buffer. */
	token = strtok_r(buffer, delim, &save);
	for (argc = 0; token != NULL; argc++) {
		/* If a token had spaces, restore them. */
		for (lpr = token, lpw = token; *lpr != '\0'; lpr++, lpw++) {
			if (*lpr == (char)-1)
				*lpw = ' ';
		}
		*argv++ = token;
		token = strtok_r(NULL, delim, &save);
	}

	*argv++ = NULL;

	return argc;
}

/**
 * shell_callcmd
 *
 * Look up a command and call it with given arguments and argument count.
 */
static int shell_callcmd(int argc, char **argv)
{
	cmd_handle_t cmd;

	if (argc < 1)
		return 0;

	cmd = query_command(argv[0]);

	if (cmd.name != NULL)
		return cmd.main(argc, argv);

	printf("Unknown command \'%s\'.\nTry \'help\' for a list of commands.\n\n", argv[0]);

	return -1;
}

/**
 * shell_flushargs
 *
 * Utility function for flushing the argument array.
 */
static void shell_flushargs(char **argv)
{
	while (*argv != NULL) {
		bzero(*argv, strlen(*argv));
		argv++;
	}

	return;
}

/**
 * shell_flushbuffer
 *
 * Utility function for flushing the input buffer.
 */
static void shell_flushbuffer(char *buffer)
{
	bzero(buffer, strlen(buffer));

	return;
}

/**
 * shell_runscript
 *
 * Parse and execute a script.
 */
int shell_runscript(char *buffer)
{
	char *token, *save;
	char *delim = ";\n";
	char *argv[MAXLINE];
	int stat, line, argc;

	if (buffer == NULL)
		return -1;

	token = strtok_r(buffer, delim, &save);
	for (line = 1; token != NULL; line++) {
		if ((argc = shell_parseline(token, argv)) == -1) {
			printf("runscript: syntax error on line %d, aborting.\n", line);
			stat = -2;
			break;
		}
		if ((stat = shell_callcmd(argc, argv)) != 0) {
			printf("runscript: command error on line %d, aborting.\n", line);
			stat = -3;
			break;
		}
		shell_flushargs(argv);
		token = strtok_r(NULL, delim, &save);
	}

	return stat;
}

/**
 * shell_prompt
 *
 * An interactive shell for xBoot.
 */
void shell_prompt(const char *prompt)
{
	int argc, guard;
	char *argv[MAXLINE];
	char buffer[MAXLINE];

	printf("\n\nEntering interactive shell. Run \'help\' for a list of commands.\n\n");

	guard = (strlen(prompt));

	while (1) {
		printf("%s", prompt);
		shell_getline(buffer, guard, MAXLINE);
		argc = shell_parseline(buffer, argv);
		shell_callcmd(argc, argv);
		shell_flushargs(argv);
		shell_flushbuffer(buffer);
	}

	return;
}
