PKGS=sdl2
CFLAGS=-Wall -ggdb -mconsole -Iinclude/ $(shell pkg-config --cflags $(PKGS))
LIBS=$(shell pkg-config --libs $(PKGS)) -lm

main: main.c
	gcc $(CFLAGS) main.c src/*.c  -o main.exe $(LIBS)
	./main.exe
 