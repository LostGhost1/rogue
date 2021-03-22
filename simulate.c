#include "rogue.h"	
void *simulate(void *ptr){
	struct timespec remaining, gamespeed={0,500000000};
	
	struct ncplane *stdplane= notcurses_stdplane(context);
	while(1){
		if(killswitch){
			return NULL;
		}
		if(pthread_mutex_lock(&simulation_mutex)==0)
			pthread_mutex_unlock(&simulation_mutex);
		if(current_state==1){
			struct playing_field_state *state=states[current_state];
			struct ncplane *stdplane= state->plane;
				
			pthread_mutex_lock(&(state->field_mutex));
			int x=0;
			int y=0;
			ncplane_dim_yx(stdplane,&y,&x);
			for(int i=0;i<x;i++)
				for(int j=0;j<y;j++)
					if(rand()%1000==1)state->field[i][j]='a'+rand()%26;
			pthread_mutex_unlock(&(state->field_mutex));
			nanosleep(&gamespeed, &remaining);
		}
	}
}
