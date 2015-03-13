#include <cmd.h>
#include <stdio.h>
#include <string.h>

extern command_tab_t commands[];

int help_main(int argc, char *argv[])
{
	int i = 0;
	command_tab_t cmd;

	if (argc > 1) {
		cmd = lookup_command(argv[1]);
		if (cmd.help != NULL)
			cmd.help();
		else
			printf("Detailed help for command \'%s\' does not exist.\n", argv[1]);
	} else {
		printf("Supported commands:\n\n");
		while (commands[i].name != NULL) {
			printf("   %-8s\t - %s\n", commands[i].name, commands[i].desc);
			++i;
		}

		printf("\nType \'help <cmd>\' for detailed command help.\n\n");
	}

	return 0;
}
