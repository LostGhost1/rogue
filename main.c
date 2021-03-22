#include "rogue.h"

FILE *debugfile;
CC_List *thread_list;

pthread_t input_thread,simulation_thread, render_thread, prepare_frame_thread;
pthread_mutex_t input_mutex,simulation_mutex, render_mutex, prepare_frame_mutex;

struct notcurses *context=NULL;

char preempt=0;

int nthreads=4;

int current_state;
int nstates=2;
void **states=NULL;

char killswitch=0;

void state_up(int newstate){
	if(newstate==1){
		struct playing_field_state *state=states[newstate];
		int x=0;
		int y=0;
		notcurses_stddim_yx(context,&y,&x);
		state->field=calloc(sizeof(char*),x);
		state->framebuffer=calloc(sizeof(char*),x);
		for(int i=0;i<x;i++){
			state->field[i]=calloc(sizeof(char),y);
			state->framebuffer[i]=calloc(sizeof(char),y);
		}
		for(int i=0;i<x;i++)
			for(int j=0;j<y;j++){
				state->field[i][j]='.';
				state->framebuffer[i][j]='X';
			}
		state->player_x=0;
		state->player_y=0;
		pthread_mutex_init(&(state->field_mutex),NULL);
		pthread_mutex_init(&(state->plane_mutex),NULL);
		ncplane_options *opts=alloca(sizeof(ncplane_options));
		opts->x=0;
		opts->y=0;
		opts->rows=y;
		opts->cols=x;
		opts->userptr=NULL;
		opts->name="Playing field";
		opts->resizecb=NULL;
		opts->flags=0;
		state->plane=ncplane_create(notcurses_stdplane(context),opts);
	}
	current_state=newstate;
}

void init(){
	pthread_mutex_init(&(input_mutex),NULL);
	pthread_mutex_init(&(simulation_mutex),NULL);
	pthread_mutex_init(&(render_mutex),NULL);
	pthread_mutex_init(&(prepare_frame_mutex),NULL);
	int newstate=1;
	states=calloc(sizeof(void*),nstates);
	{
		struct main_menu_state *state=calloc(sizeof(struct main_menu_state),1);
		states[0]=state;
	}
	{
		struct playing_field_data *state=calloc(sizeof(struct playing_field_state),1);
		states[1]=state;
	}
	state_up(newstate);
}
int main(int arc, char **argv, char **envp){
	debugfile=fopen("debug","w");
	setvbuf(debugfile,NULL,_IONBF,0);
	srand(time(NULL));
	context= notcurses_core_init(NULL,stdin);
//	struct ncplane *stdplane= notcurses_stdplane(context);
//	notcurses_stddim_yx(context,&y,&x);
	init();

	struct input_arg arg;
	arg.self = &input_thread;

	pthread_create(&simulation_thread, NULL, simulate, context);
	pthread_create(&input_thread, NULL, input, &arg);
	pthread_create(&render_thread, NULL, render, context);
	pthread_create(&prepare_frame_thread, NULL, prepare_frame, context);

	pthread_join(input_thread, NULL);
	notcurses_stop(context);
	fclose(debugfile);
	pthread_exit(NULL);
}
