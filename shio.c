#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char * get_input() {
	char line[256];
	fgets(line, 256, stdin);
	char * formatted = calloc(256, sizeof(char));
	strncpy(formatted, line, 256);
	return formatted;
}

char *** parse_args(char *line) {
	char *** commands = malloc(sizeof(char **));
	int i = 0;
	char * cmd;
	char ** args;
	do {
		cmd = strsep(&line, ";");
	} while (commands[i++]);
}
