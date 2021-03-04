#include <notcurses/notcurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <collectc/cc_list.h>

extern FILE *debugfile;
extern CC_List *thread_list;

extern int x,y;
extern int player_x,player_y;
extern char **field;
extern pthread_mutex_t field_mutex;

extern char killswitch;

extern pthread_mutex_t stdplane_mutex;


struct input_arg{
	struct notcurses *context;
	pthread_t *self;
};
void *input(void *ptr);
void *prepare_frame(void *ptr);
void *render(void *ptr);
void *simulate(void *ptr);
void init();
int main(int arc, char **argv, char **envp);
