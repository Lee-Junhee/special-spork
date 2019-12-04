#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include"shlib.h"

void run(char *** args) {
	int i = 0;
	int cpid, status;
	while (args[i]) {
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
