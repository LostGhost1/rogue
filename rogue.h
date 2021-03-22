#include <notcurses/notcurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <collectc/cc_list.h>

extern struct notcurses *context;

extern char preempt;
extern pthread_mutex_t preempt_mutex;

extern int nthreads;

extern int state;
//0 - main menu
//1 - playing field
extern int nstates;
extern int current_state;
extern void **states;
extern FILE *debugfile;

extern pthread_t input_thread,simulation_thread, render_thread, prepare_frame_thread;
extern pthread_mutex_t input_mutex,simulation_mutex, render_mutex, prepare_frame_mutex;

extern char killswitch;

struct main_menu_state{
};
struct playing_field_state{
	char **framebuffer;
	int x,y;
	int player_x,player_y;
	char **field;
	pthread_mutex_t field_mutex;
	pthread_mutex_t plane_mutex;
	struct ncplane *plane;
};
struct input_arg{
	pthread_t *self;
};
void *input(void *ptr);
void *prepare_frame(void *ptr);
void *render(void *ptr);
void *simulate(void *ptr);
void init();
int main(int arc, char **argv, char **envp);
