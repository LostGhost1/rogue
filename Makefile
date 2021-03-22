all: rogue
edit:
	nano *.h *.c
clean:
ifneq (,$(wildcard ./debug))
	rm debug
endif
ifneq (,$(wildcard ./rogue))
	rm rogue
endif
rogue: main.c input.c render.c simulate.c
	gcc -g -o rogue -L/usr/lib64 -lcollectc -lnotcurses-core -lpthread main.c input.c render.c simulate.c
