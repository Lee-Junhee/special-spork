#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"shio.h"
#include"dir.h"

char * get_input() {
	char input[256];
	char path[256];
	getcwd(path, 256);
	homify(path);
	printf("%s$ ", path);
	fgets(input, 256, stdin);
	char * line = input;
	char * formatted = calloc(256, sizeof(char));
	strncpy(formatted, strsep(&line, "\n"), 256);
	return formatted;
}

char *** parse_args(char *line) {
	int i = 0;
	int j;
	char * cmd;
	char ** args;
	char *** commands = NULL;
	do {
		commands = realloc(commands, sizeof(char **) * (i + 1));
		cmd = strsep(&line, ";");
		if (cmd) {
			args = NULL;
			j = 0;
			do {
				args = realloc(args, sizeof(char *) *  (j + 1));
				args[j] = strsep(&cmd, " ");
			} while (args[j++]);
			commands[i] = args;
		}else {
			commands[i] = NULL;
		}
	} while (commands[i++]);
	return commands;
}
