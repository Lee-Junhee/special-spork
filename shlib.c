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
	char redirected = redir(args);
	int size = 1;
	char * file = args[0] + 1;
	if (redirected & 2) {
		struct stat fileinfo;
		stat(file, &fileinfo);
		size = fileinfo.st_size;
	}
	char content[size + 1];
	int k = 1;
	if (redirected & 2) {
		char raw[size];
		int fd = open(file, O_RDONLY);
		read(fd, raw, size);
		close(fd);
		strcpy(content, raw);
		k = 0;
	}
	char * newargs[64];
	int j = 1;
	if (redirected & 2) {
		char * point = content;
		do {
			newargs[k] = strsep(&point, " \n");
		} while (newargs[k++]);
		j = 0;
		while (args[j]) {
			j++;
		}
		k += j;
	}
	char * oldargs[j];
	if (redirected & 2) {
		while (j) {
			oldargs[j - 1] = args[j];
			j--;
		}
	}
	char * allargs[k];
	if (redirected & 2) {
		j = 0;
		while (oldargs[j]) {
			allargs[j] = oldargs[j];
			j++;
		}
		k = 0;
		while (newargs[k]) {
			allargs[j + k] = newargs[k];
			k++;
		}
		allargs[j + k] = NULL;
		char ** temp = nonnull(allargs);
		j = 0;
		while (temp[j]) {
			allargs[j] = temp[j];
			j++;
		}
		allargs[j] = NULL;
		free(temp);
		args = allargs;
		j = 0;
	}
	if (!(redirected & 1)) {
		j = 0;
		while (args[j]) {
			j++;
		}
	}
	char * coloured[j + 1];
	char * colour = "--color";
	if (!(redirected & 1)) {
		if (!strcmp("ls", args[0])) {
			j = 0;
			while (args[j]) {
				coloured[j] = args[j];
				j++;
			}
			coloured[j] = colour;
			coloured[j + 1] = NULL;
			args = coloured;
		}
	}
	execvp(args[0], args);
	exit(0);
}
