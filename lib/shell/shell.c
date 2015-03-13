#include <cmd.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <interface/serial.h>

#define MAXLINE MAX_INPUT

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
				if (lp >= (buffer + minlen)) {
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

static int shell_parseline(char *buffer, char **argv)
{
	int argc;
	char *token;
	char *delim = " \t";

	if (*buffer == '\0')
		return 0;

	token = strtok(buffer, delim);

	for (argc = 0; token != NULL; argc++) {
		*argv++ = token;
		token = strtok(NULL, delim);
	}

	*argv++ = NULL;

	return argc;
}

static int shell_callcmd(int argc, char **argv)
{
	command_tab_t cmd;

	if (argc < 1)
		return 0;

	cmd = lookup_command(argv[0]);

	if (cmd.name != NULL)
		return cmd.main(argc, argv);

	printf("Error: \'%s\' is not a valid command.\nEnter \'help\' for a list of supported commands.\n\n", argv[0]);

	return -1;
}

static void shell_flushargs(char **argv)
{
	while (*argv != NULL) {
		bzero(*argv, strlen(*argv));
		argv++;
	}

	return;
}

static void shell_flushbuffer(char *buffer)
{
	bzero(buffer, strlen(buffer));

	return;
}

void shell_prompt(const char *prompt)
{
	int argc, guard;
	char *argv[MAXLINE];
	char buffer[MAXLINE];

	printf("\n\nEntering interactive shell. Enter \'help\' for a list of commands.\n\n");

	guard = (strlen(prompt) - 1);

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
