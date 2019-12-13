#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/stat.h>
#include"shlib.h"
#include"shio.h"
#include"dir.h"

int run(char *** args) {
	int i = 0;
	int exited = 0;
	int cpid, status;
	while (args[i]) {
		if (!strcmp(args[i][0], "exit")) {
			exited = 1;
		}
		if (!exited) {
			if (!strcmp(args[i][0], "cd")) {
				chcwd(args[i][1]);
			}else {
				cpid = fork();
				if (cpid){
					wait(status);
				}else {
					execute(args[i]);
				}
			}
		}
		i++;
	}
	free(args[0][0]);
	while (i + 1) {
		free(args[i--]);
	}
	free(args);
	if (exited) {
		exit(0);
	}
}

void execute(char ** args) {
	int i = 0;
	int j = 0;
	int fd[2];
	fd[0] = 0;
	fd[1] = 1;
	int temp[2];
	char * entry = NULL;
	int cpid;
	int status;
	char ** margs = malloc(sizeof(char *));
	while (args[i + j]) {
		j += i;
		i = 0;
		while (strncmp(args[i + j], "|", 1)) {
			i++;
			if (!args[i + j]) {
				break;
			}
		}
		if (args[i + j]) {
			if (!strncmp(args[i + j], "|", 1)) {
				if (!args[i + j][1]) {
					entry = NULL;
					args[i + j] = NULL;
					i++;
				}else {
					entry = &args[i + j][1];
					args[i + j] = NULL;
				}
			}
			pipe(temp);
			cpid = fork();
			if (cpid) {
				close(temp[1]);
				fd[0] = temp[0];
				if (entry) {
					args[i + j] = entry;
				}
				wait(status);
			}else {
				fd[1] = temp[1];
				break;
			}
		}
	}
	i = 0;
	do {
		margs[i] = args[i + j];
		margs = realloc(margs, sizeof(char *) * (i + 2));
	} while (args[j + i++]);
	parse_exec(margs, fd[0], fd[1]);
	exit(0);
}

void parse_exec(char ** args, int r, int w) {
	char out = redir(args);

	//redirect input to pipe
	dup2(r, 0);

	//redirect output to pipe
	dup2(w, 1);

	//color ls
	if (!strcmp(args[0], "ls") & !(w - 1)) {
		int i = 0;
		while (args[i]) {
			i++;
		}
		char * newarg[i + 2];
		char * color = "--color";
		newarg[i + 1] = NULL;
		newarg[i--] = color;
		while (i + 1) {
			newarg[i] = args[i];
			i--;
		}
		execvp(newarg[0], newarg);

	}

	//execute
	execvp(args[0], args);
}
