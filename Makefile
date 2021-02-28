all: demo
demo: main.c
	gcc -o demo -L/usr/lib64 -lnotcurses-core -lpthread main.c
