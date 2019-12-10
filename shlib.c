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
	int redirected = redir(args);
	if (redirected & 2) {
		char * file = args[0];
		file++;
		struct stat fileinfo;
		stat(file, &fileinfo);
		int fd;
		fd = open(file, O_RDONLY);
		char raw[fileinfo.st_size];
		char contents[fileinfo.st_size];
		read(fd, raw, fileinfo.st_size);
		strncpy(contents, raw, fileinfo.st_size);
		char * content = contents;
		char ** newargs = NULL;
		int a = 0;
		do {
			newargs = realloc(newargs, sizeof(char *) * (a + 1));
			newargs[a] = strsep(&content, " \n");
		} while (newargs[a++]);
		rmempty(newargs);
		int b = 0;
		while (args[b]) {
			b++;
		}
		char * oldargs[b];
		while (b) {
			oldargs[b - 1] = args[b];
			b--;
		}
		char * allargs[a + b];
		b = 0;
		while (oldargs[b]) {
			allargs[b] = oldargs[b];
			b++;
		}
		a = 0;
		while (newargs[a]) {
			allargs[a + b] = newargs[a];
			a++;
		}
		allargs[a + b] = NULL;
		args = allargs;
	}
	if (redirected & 1) {
		execvp(args[0], args);
		exit(0);
	}else {
		if (!strcmp(args[0], "ls")) {
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
		execvp(args[0], args);
		exit(0);
	}
}
