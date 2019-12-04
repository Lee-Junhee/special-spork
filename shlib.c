#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"shlib.h"
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
	if (0) {
		wait(0);
	}else {
		execvp(args[0], args);
	}
}
