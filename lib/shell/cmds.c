#include <cmd.h>
#include <stdio.h>
#include <string.h>

#include "cmds.h"

command_tab_t lookup_command(const char *name)
{
	int i = 0;
	command_tab_t cmd = { NULL, NULL, NULL };

	while (commands[i].name != NULL) {
		if (strncmp(commands[i].name, name, strlen(name)) == 0)
			cmd = commands[i];
		++i;
	}

	return cmd;
}
