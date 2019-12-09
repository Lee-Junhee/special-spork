#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include"shio.h"
#include"dir.h"

char * get_input() {
	char input[256];
	print_home();
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

void print_home() {
	char path[256];
	getcwd(path, 256);
	homify(path);
	printf("\033[1;36m");
	printf("%s$ ", path);
	printf("\033[0m");
}

void redir(char ** args) {
	char * arg;
	int i = 0;
	char in = 0;
	char out = 0;
	int fd;
	do {
		arg = args[i];
		if (!strncmp(">", arg, 1)) {
			if (out) {
				if (!args[i][1]) {
					fd = open(args[i + 1], O_WRONLY | O_TRUNC, 0777);
				}else {
					if (!strncmp(arg + 1, ">", 1)) {
						if (!args[i][2]) {
							fd = open(args[i + 1], O_WRONLY | O_APPEND, 0777);
						}else {
							fd = open()
						}
					}else {
						fd = open(args[i] + 1, )
					}
				}
				out = 1;
			}else {
				if (!args[i][1]) {
					strcpy(args[i + 1], "");
				}else {
					if (!strncmp(arg + 1, ">", 1) & !args[i][2]) {
						strcpy(args[i + 1], "");
					}
				}
				strcpy(args[i], "");
			}
		}
		if (!strncmp("<", arg, 1)) {
			if (in) {
				if (!args[i][1]) {
					fd = open(args[i + 1], O_RDONLY);
				}else {
					fd = open(&args[i][1], O_RDONLY);
				}
				//add args to end of args 
				in = 1;
			}else {
				if (!args[i][1]) {
					strcpy(args[i + 1], "");
				}
				strcpy(args[i], "");
			}
		}
	} while (args[i++]);
}

void rmempty (char ** args) {
}
