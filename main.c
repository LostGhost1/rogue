#include <notcurses/notcurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
int x,y;
int player_x,player_y;
char **field;
pthread_mutex_t field_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t stdplane_mutex = PTHREAD_MUTEX_INITIALIZER;
void *render(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, renderspeed = {0,16666670};
	while(1){
		if(pthread_mutex_trylock(&stdplane_mutex)==0){
			notcurses_render(context);
			pthread_mutex_unlock(&stdplane_mutex);
			nanosleep(&renderspeed, &remaining);
		}
		//
	}
}
void *prepare_frame(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, speed = {0,16666670};
	while(1){
		if(pthread_mutex_trylock(&field_mutex)==0){
			pthread_mutex_lock(&stdplane_mutex);
			for(int i=0;i<x;i++){
				for(int j=0;j<y;j++){
					if(player_x==i)
						if(player_y==j){
							ncplane_putchar_yx(stdplane,j,i,'@');
							continue;
						}
					ncplane_putchar_yx(stdplane,j,i,field[i][j]);
				}
			}
			pthread_mutex_unlock(&stdplane_mutex);
			pthread_mutex_unlock(&field_mutex);
			nanosleep(&speed, &remaining);
		}
	}
}
void *input(void *ptr){
	struct notcurses *context = ptr;
	char32_t key;
	ncinput ni;
	while(1){
		key = notcurses_getc_blocking(ptr, &ni);
		if(key=='q')return NULL;
		if(key=='w')if(player_y!=0)player_y-=1;
		if(key=='a')if(player_x!=0)player_x-=1;
		if(key=='s')if(player_y!=y)player_y+=1;
		if(key=='d')if(player_x!=x)player_x+=1;
	}
}
void *simulate(void *ptr){
	struct timespec remaining, gamespeed={0,500000000};
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	while(1){
		pthread_mutex_lock(&field_mutex);
		for(int i=0;i<x;i++)
			for(int j=0;j<y;j++)
				if(rand()%100==1)field[i][j]='a'+rand()%26;
		pthread_mutex_unlock(&field_mutex);
		nanosleep(&gamespeed, &remaining);
	}
}
void init(){
	field=calloc(sizeof(char*),x);
	for(int i=0;i<x;i++)
		field[i]=calloc(sizeof(char),y);
	for(int i=0;i<x;i++)
		for(int j=0;j<y;j++)
			field[i][j]='.';
	player_x=0;
	player_y=0;
}
int main(){
	srand(time(NULL));
	struct notcurses *context= notcurses_core_init(NULL,stdin);
	struct ncplane *stdplane= notcurses_stdplane(context);
	notcurses_stddim_yx(context,&y,&x);
	init();
	pthread_t input_thread,simulation_thread, render_thread, prepare_frame_thread;
	pthread_create(&simulation_thread, NULL, simulate, context);
	pthread_create(&input_thread, NULL, input, context);
	pthread_create(&render_thread, NULL, render, context);
	pthread_create(&prepare_frame_thread, NULL, prepare_frame, context);
	pthread_join(input_thread, NULL);
	notcurses_stop(context);
	return 0;
}
