all: demo
clean:
ifneq (,$(wildcard ./debug))
	rm debug
endif
ifneq (,$(wildcard ./rogue))
	rm rogue
endif
demo: main.c input.c render.c simulate.c
	gcc -o rogue -L/usr/lib64 -lcollectc -lnotcurses-core -lpthread main.c input.c render.c simulate.c
