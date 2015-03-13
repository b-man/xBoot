#ifndef COMMANDS_H
#define COMMANDS_H

#include <cmd.h>
#include <stdio.h>

/* Command prototypes */
extern int help_main(int argc, char *argv[]);
extern int reset_main(int argc, char *argv[]);
extern int halt_main(int argc, char *argv[]);

extern void getenv_help(void);
extern int getenv_main(int argc, char *argv[]);

extern void setenv_help(void);
extern int setenv_main(int argc, char *argv[]);

extern void printenv_help(void);
extern int printenv_main(int argc, char *argv[]);

/* Command list */
command_tab_t commands[] = {
	{ "help", "Display command help.", NULL, help_main },
	{ "reset", "Reset the system.", NULL, reset_main },
	{ "halt", "Halt the system.", NULL, halt_main },
	{ "getenv", "Read environment variable.", getenv_help, getenv_main },
	{ "setenv", "Set an environment variable.", setenv_help, setenv_main },
	{ "printenv", "Print one or all environment variables.", printenv_help, printenv_main },
        { NULL, NULL, NULL },
};

#endif /* !COMMANDS_H */
