#include<stdio.h>
#include<stdlib.h>
#include"shio.h"
#include"shlib.h"

int main() {
	while (1) {
		char * input = get_input();
		char *** argv = parse_args(input);
		run(argv);
	}
}
