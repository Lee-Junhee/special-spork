#include<stdlib.h>
#include<string.h>
#include<unistd.h>

void chcwd(char * path) {
	if (path) {
		char home[2] = "~";
		char homedir[64];
		strncpy(homedir, getenv("HOME"), 64);
		char dir[256] = "";
		if (strncmp(path, home, 1)) {
			strncpy(dir, path, 1);
		}else {
			strcpy(dir, homedir);
		}
		strncat(dir, path + 1, 192);
		chdir(dir);
	}
}
