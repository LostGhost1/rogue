#include <notcurses/notcurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
void *refresh(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, request = {0,16666670};
	//struct timespec remaining, request = {1,0};
	while(1){
		notcurses_render(context);
		nanosleep(&request, &remaining);
	}
}
void *animate(void *ptr){
	struct notcurses *context = ptr;
	struct timespec remaining, request = {0,10000000};
	int x,y;
	struct ncplane *stdplane= notcurses_stdplane(context);
	notcurses_stddim_yx(context,&y,&x);
	while(1){
		for(int n=0;n<y/2;n++){
			for(int i=n;i<x-n;i++){
				ncplane_putchar_yx(stdplane,n,i,'@');
				nanosleep(&request, &remaining);
			}
			for(int j=n;j<y-n;j++){
				ncplane_putchar_yx(stdplane,j,x-1-n,'@');
				nanosleep(&request, &remaining);
			}
			for(int i=x-n-1;i>=n;i--){
				ncplane_putchar_yx(stdplane,y-n-1,i,'@');
				nanosleep(&request, &remaining);
			}
			for(int j=y-n-1;j>=n;j--){
				ncplane_putchar_yx(stdplane,j,n,'@');
				nanosleep(&request, &remaining);
			}
		}
		for(int n=0;n<y/2;n++){
			for(int i=n;i<x-n;i++){
				ncplane_putchar_yx(stdplane,n,i,' ');
				nanosleep(&request, &remaining);
			}
			for(int j=n;j<y-n;j++){
				ncplane_putchar_yx(stdplane,j,x-1-n,' ');
				nanosleep(&request, &remaining);
			}
			for(int i=x-n-1;i>=n;i--){
				ncplane_putchar_yx(stdplane,y-n-1,i,' ');
				nanosleep(&request, &remaining);
			}
			for(int j=y-n-1;j>=n;j--){
				ncplane_putchar_yx(stdplane,j,n,' ');
				nanosleep(&request, &remaining);
			}
		}
	}
	ncplane_printf(stdplane,"Done.");
	request.tv_sec=5;
	request.tv_nsec=0;
	nanosleep(&request, &remaining);
	return NULL;
}
int main(){
	struct notcurses *context= notcurses_core_init(NULL,stdin);
	pthread_t animation_thread, refresh_thread;
	pthread_create(&animation_thread, NULL, animate, context);
	pthread_create(&refresh_thread, NULL, refresh, context);
	pthread_join(animation_thread, NULL);
	notcurses_stop(context);
	return 0;
}
