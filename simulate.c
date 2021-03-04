#include "rogue.h"	
void *simulate(void *ptr){
	struct timespec remaining, gamespeed={0,500000000};
	struct notcurses *context = ptr;
	struct ncplane *stdplane= notcurses_stdplane(context);
	while(1){
		if(killswitch){
			return NULL;
		}
		
		pthread_mutex_lock(&field_mutex);
		for(int i=0;i<x;i++)
			for(int j=0;j<y;j++)
				if(rand()%1000==1)field[i][j]='a'+rand()%26;
		pthread_mutex_unlock(&field_mutex);
		nanosleep(&gamespeed, &remaining);
	}
}
