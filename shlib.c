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
		j = i;
		i = 0;
		if (entry) {
			args[i + j] = entry;
		}
		while (strncmp(args[i + j], "|", 1)) {
			i++;
			if (!args[i + j]) {
				break;
			}
		}
		if (args[i + j]) {
			if (args[i + j] + 1) {
				entry = NULL;
				args[i + j] = NULL;
				i++;
			}else {
				args[i + j] = args[i + j] + 1;
				entry = args[i + j];
				args[i + j] = NULL;
			}
			pipe(temp);
			cpid = fork();
			if (cpid) {
				fd[0] = temp[0];
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
	margs[j + i] = NULL;
	parse_exec(margs, fd[0], fd[1]);
}

void parse_exec(char ** args, int r, int w) {
	char redirected = redir(args);

	//redirecting stdin
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
			newargs[k] = strsep(&point, " \n\r");
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
	char * allargs[k + 1];
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

	//inserts color into ls
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

	//redirect input from pipe
	char rawpipein[256];
	char pipein[255];
	char * pargs[64];
	char ** pnonnull;
	if (r) {
		read(r, pipein, 255);
		strcpy(pipein, rawpipein);
		char * ppoint = pipein;
		j = 0;
		do {
			pargs[j] = strsep(&ppoint, " \n\r");
		} while (pargs[j++]);
		pnonnull = nonnull(pargs);
		j = 0;
		while (pnonnull[j]) {
			pargs[j] = pnonnull[j];
			j++;
		}
		pargs[j] = NULL;
		free(pnonnull);
		k = 0;
		while (args[k]) {
			k++;
		}
	}
	char * paargs[j + k + 1];
	if (r) {
		k = 0;
		while (args[k]) {
			paargs[k] = args[k];
			k++;
		}
		j = 0;
		while (pargs[j]) {
			paargs[k + j] = pargs[j];
			j++;
		}
		paargs[j + k] = NULL;
		args = paargs;
	}

	//redirect output to pipe
	dup2(w, 1);
	
	//execute
	execvp(args[0], args);
	exit(0);
}
