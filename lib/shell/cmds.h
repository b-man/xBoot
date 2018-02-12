/*
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

#ifndef CMDS_H
#define CMDS_H

#include <stdio.h>

#include <shell/shell.h>

/* Command prototypes */
extern int help_main(int argc, char *argv[]);
extern int reset_main(int argc, char *argv[]);
extern int halt_main(int argc, char *argv[]);
extern int boot_main(int argc, char *argv[]);

extern void getenv_help(void);
extern int getenv_main(int argc, char *argv[]);

extern void setenv_help(void);
extern int setenv_main(int argc, char *argv[]);

extern void printenv_help(void);
extern int printenv_main(int argc, char *argv[]);

extern int history_main(int argc, char *argv[]);

/* Command list */
cmd_handle_t commands[] = {
	{ "help", "Display command help.", NULL, help_main },
	{ "boot", "Boot into Darwin.", NULL, boot_main },
	{ "halt", "Halt the system.", NULL, halt_main },
	{ "reset", "Reset the system.", NULL, reset_main },
	{ "getenv", "Read environment variable.", getenv_help, getenv_main },
	{ "setenv", "Set an environment variable.", setenv_help, setenv_main },
	{ "printenv", "Print one or all environment variables.", printenv_help, printenv_main },
	{ "history", "Print the command history.", NULL, history_main },
	{ NULL, NULL, NULL },
};

#endif /* !CMDS_H */
