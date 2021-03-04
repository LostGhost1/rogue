#include "rogue.h"

FILE *debugfile;
CC_List *thread_list;

int x,y;
int player_x,player_y;
char **field;
pthread_mutex_t field_mutex = PTHREAD_MUTEX_INITIALIZER;

char killswitch=0;

pthread_mutex_t stdplane_mutex = PTHREAD_MUTEX_INITIALIZER;


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
int main(int arc, char **argv, char **envp){
	debugfile=fopen("debug","w");
	setvbuf(debugfile,NULL,_IONBF,0);
	srand(time(NULL));
	struct notcurses *context= notcurses_core_init(NULL,stdin);
	struct ncplane *stdplane= notcurses_stdplane(context);
	notcurses_stddim_yx(context,&y,&x);
	init();
	pthread_t input_thread,simulation_thread, render_thread, prepare_frame_thread;

	cc_list_new(&thread_list);
	cc_list_add(thread_list,&input_thread);
	cc_list_add(thread_list,&render_thread);
	cc_list_add(thread_list,&simulation_thread);
	cc_list_add(thread_list,&prepare_frame_thread);

	struct input_arg arg;
	arg.self = &input_thread;		
	arg.context = context;

	pthread_create(&simulation_thread, NULL, simulate, context);
	pthread_create(&input_thread, NULL, input, &arg);
	pthread_create(&render_thread, NULL, render, context);
	pthread_create(&prepare_frame_thread, NULL, prepare_frame, context);

	pthread_join(input_thread, NULL);
	notcurses_stop(context);
	fclose(debugfile);
	return 0;
}
