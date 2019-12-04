#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include"shlib.h"

void run(char *** args) {
	int i = 0;
	int cpid, status;
	while (args[i]) {
		if (!strcmp(args[i][0], "exit")) {
			exit(0);
		}
		if (!strcmp(args[i][0], "cd")) {
			chdir(args[i][1]);
		}
		cpid = fork();
		if (cpid){
			wait(status);
		}else {
			execvp(args[i][0], args[i]);
		}
		i++;
	}
	free(args[0][0]);
	while (i + 1) {
		free(args[i--]);
	}
	free(args);
}
