#include "rogue.h"
char **framebuffer;
void *render(void *ptr){
	struct timespec remaining, renderspeed = {0,16666670};
	while(1){
		if(killswitch){
			return NULL;
		}
		if(pthread_mutex_lock(&render_mutex)==0)
			pthread_mutex_unlock(&render_mutex);
		if(current_state==1){
			struct playing_field_state *state=states[current_state];
			struct ncplane *stdplane= state->plane;
				
			if(pthread_mutex_trylock(&(state->plane_mutex))==0){
				notcurses_render(context);
				pthread_mutex_unlock(&(state->plane_mutex));
				nanosleep(&renderspeed, &remaining);
			}
		}
	}
}
void *prepare_frame(void *ptr){
	
	
	struct timespec remaining, speed = {0,16666670};
	while(1){
		if(killswitch){
			return NULL;
		}
		if(pthread_mutex_lock(&prepare_frame_mutex)==0)
			pthread_mutex_unlock(&prepare_frame_mutex);
		if(current_state==1){
			struct playing_field_state *state=states[current_state];
			struct ncplane *stdplane= state->plane;
			if(pthread_mutex_trylock(&(state->field_mutex))==0){
				pthread_mutex_lock(&(state->plane_mutex));
				int x=0;
				int y=0;
				ncplane_dim_yx(stdplane,&y,&x);
				for(int i=0;i<x;i++){
					for(int j=0;j<y;j++){
						if((state->player_x)==i)
							if((state->player_y)==j){
								if((state->framebuffer[i][j])=='@')continue;
								(state->framebuffer[i][j])='@';
								ncplane_putchar_yx(stdplane,j,i,'@');
								continue;
							}
						if((state->field[i][j])==(state->framebuffer[i][j]))continue;
						state->framebuffer[i][j]=state->field[i][j];
						ncplane_putchar_yx(stdplane,j,i,state->framebuffer[i][j]);
					}
				}
				pthread_mutex_unlock(&(state->plane_mutex));
				pthread_mutex_unlock(&(state->field_mutex));
				nanosleep(&speed, &remaining);
			}
		}
	}
}
