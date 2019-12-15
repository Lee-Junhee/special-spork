#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<unistd.h>
#include"shio.h"
#include"shlib.h"

static void sighandler(int sig) {
	if (sig == SIGINT) {
		printf("\n");
	}
}

int main() {
	signal(SIGINT, sighandler);
	while (1) {
		char * input = get_input();
		char *** argv = parse_args(input);
		run(argv);
		printf("\n");
	}
}
