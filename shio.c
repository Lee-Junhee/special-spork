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

char redir(char ** args) {
	char in = 0;
	char out = 0;
	int i = 0;
	int j;
	int fd = 1;
	while (args[i]) {
		if (!strncmp(args[i], ">", 1)) {
			if (!args[i][1]) {
				fd = open(args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0777);
				args[i] = "";
				args[i + 1] = "";
			}else {
				if (!strncmp(args[i], ">>", 2)) {
					if (!args[i][2]) {
						fd = open(args[i + 1], O_CREAT | O_WRONLY | O_APPEND, 0777);
						args[i] = "";
						args[i + 1] = "";
					}else {
						fd = open(args[i] + 2, O_CREAT | O_WRONLY | O_APPEND, 0777);
						args[i] = "";
					}
				}else {
					fd = open(args[i] + 1, O_CREAT | O_WRONLY | O_TRUNC, 0777);
					args[i] = "";
				}
			}
			if (!out) {
				dup2(fd, 1);
			}
			out = 1;
		}
		if (!strncmp(args[i], "<", 1)) {
			if (!in) {
				if (!args[i][1]) {
					args[i] = "";
					fd = open(args[i + 1], O_RDONLY);
					args[i + 1] = "";
				}else {
					fd = open(args[i] + 1, O_RDONLY);
					args[i] = "";
				}
				dup2(fd, 0);
			}else {
				if (!args[i][1]) {
					args[i] = "";
					args[i + 1] = "";
				}else {
					args[i] = "";
				}
			}
			in = 2;
		}
		i++;
	}
	rmempty(args);
	return in + out;
}

void rmempty (char ** args) {
	if (args) {
		int nonempty = 0;
		int len = 0;
		while (args[len]) {
			if (strcmp("", args[len])) {
				nonempty++;
			}
			len++;
		}
		char *full[nonempty + 1];
		full[nonempty] = NULL;
		len = 0;
		nonempty = 0;
		while (args[len]) {
			if (strcmp("", args[len])) {
				full[nonempty] = args[len];
				nonempty++;
			}
			len++;
		}
		args = realloc(args, nonempty);
		len = 0;
		while (full[len]) {
			args[len] = full[len];
			len++;
		}
		args[len] = NULL;
	}
}

char ** nonnull (char ** args) {
	if (args) {
		int nonempty = 0;
		int len = 0;
		while (args[len]) {
			if (strcmp("", args[len])) {
				nonempty++;
			}
			len++;
		}
		char *full[nonempty + 1];
		full[nonempty] = NULL;
		len = 0;
		nonempty = 0;
		while (args[len]) {
			if (strcmp("", args[len])) {
				full[nonempty] = args[len];
				nonempty++;
			}
			len++;
		}
		char ** copy = calloc(sizeof(char *), nonempty + 1);
		len = 0;
		while (full[len]) {
			copy[len] = full[len];
			len++;
		}
		return copy;
	}
	return NULL;
}
