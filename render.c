#include "rogue.h"
void *render(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, renderspeed = {0,16666670};
	while(1){
		if(killswitch){
			return NULL;
		}
		if(pthread_mutex_trylock(&stdplane_mutex)==0){
			notcurses_render(context);
			pthread_mutex_unlock(&stdplane_mutex);
			nanosleep(&renderspeed, &remaining);
		}
	}
}
void *prepare_frame(void *ptr){
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	struct timespec remaining, speed = {0,16666670};
	while(1){
		if(killswitch){
			return NULL;
		}
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
