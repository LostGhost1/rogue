#include <notcurses/notcurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
int x,y;
pthread_mutex_t key_mutex = PTHREAD_MUTEX_INITIALIZER;
char32_t key;
void *render(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, renderspeed = {0,16666670};
	while(1){
		notcurses_render(context);
		nanosleep(&renderspeed, &remaining);
	}
}
void *input(void *ptr){
	struct notcurses *context = ptr;
	ncinput ni;
	char32_t localkey;
	while(1){
		localkey = notcurses_getc_blocking(ptr, &ni);
		if(localkey=='q')return NULL;
		pthread_mutex_lock(&key_mutex);
		key=localkey;
		pthread_mutex_unlock(&key_mutex);
	}
}
void *simulate(void *ptr){
	struct timespec remaining, gamespeed={0,50000000};
	char32_t localkey;
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	while(1){
		pthread_mutex_lock(&key_mutex);
		localkey=key;
		pthread_mutex_unlock(&key_mutex);
		ncplane_erase(stdplane);
		ncplane_cursor_move_yx(stdplane,0,0);
		ncplane_printf(stdplane,"%c",key);
		nanosleep(&gamespeed, &remaining);
	}
}
int main(){
	struct notcurses *context= notcurses_core_init(NULL,stdin);
	struct ncplane *stdplane= notcurses_stdplane(context);
	notcurses_stddim_yx(context,&y,&x);
	pthread_t input_thread,simulation_thread, render_thread;
	pthread_create(&simulation_thread, NULL, simulate, context);
	pthread_create(&input_thread, NULL, input, context);
	pthread_create(&render_thread, NULL, render, context);
	pthread_join(input_thread, NULL);
	notcurses_stop(context);
	return 0;
}
