#include "rogue.h"
void *input(void *ptr){
	struct input_arg *arg=ptr;
	struct notcurses *context = arg->context;
	pthread_t *self=arg->self;
	char32_t key;
	ncinput ni;
	while(1){
		key = notcurses_getc_blocking(context, &ni);
		if(key=='q'){
			killswitch=1;
			CC_ListIter i;
			cc_list_iter_init(&i,thread_list);
			while(i.next!=NULL){
				pthread_t *current;
				cc_list_iter_next(&i,(void*)(&current));
				if((*current)==(*self))continue;
				pthread_join(*current,NULL);
			}
			return NULL;
		}
		if(key=='w')if(player_y!=0)player_y-=1;
		if(key=='a')if(player_x!=0)player_x-=1;
		if(key=='s')if(player_y!=y-1)player_y+=1;
		if(key=='d')if(player_x!=x-1)player_x+=1;
	}
}
