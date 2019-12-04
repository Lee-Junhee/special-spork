ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o shio.o shlib.o
	$(CC) -o athena main.o shio.o shlib.o

main.o: main.c shio.h shlib.h
	$(CC) -c main.c

shio.o: shio.c shio.h
	$(CC) -c shio.c

shlib.o: shlib.c shlib.h
	$(CC) -c shlib.c

run: all
	./athena

clean:
	touch main.o
	touch a.out
	rm *.o
	rm a.out
