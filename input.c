#include "rogue.h"
void *input(void *ptr){
	struct input_arg *arg=ptr;
	pthread_t *self=arg->self;
	char32_t key;
	ncinput ni;
	char paused;
	paused=0;
	while(1){
//		pthread_mutex_lock(&preempt_mutex);
//		if(preempt)
//		pthread_mutex_unlock(&preempt_mutex);
		if(pthread_mutex_lock(&input_mutex)==0)
			pthread_mutex_unlock(&input_mutex);
		
		if(current_state==1){
			struct playing_field_state *state=states[current_state];
			struct ncplane *stdplane= state->plane;
			int x=0;			
			int y=0;
			ncplane_dim_yx(stdplane,&y,&x);
			key = notcurses_getc_blocking(context, &ni);
			if(key=='q'){
				killswitch=1;
				pthread_join(simulation_thread,NULL);
				pthread_join(render_thread, NULL);
				pthread_join(prepare_frame_thread,NULL);
				return NULL;
			}
			if(key=='p'){
				if(!paused){
					pthread_mutex_lock(&render_mutex);
					pthread_mutex_lock(&simulation_mutex);
					pthread_mutex_lock(&prepare_frame_mutex);
					paused=1;
				}else{
					pthread_mutex_unlock(&render_mutex);
					pthread_mutex_unlock(&simulation_mutex);
					pthread_mutex_unlock(&prepare_frame_mutex);
					paused=0;
				}
			}
			if(key=='w')if(state->player_y!=0)state->player_y-=1;
			if(key=='a')if(state->player_x!=0)state->player_x-=1;
			if(key=='s')if(state->player_y!=y-1)state->player_y+=1;
			if(key=='d')if(state->player_x!=x-1)state->player_x+=1;
		}
	}
}
