#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include "gtwait.h"

#define THREADS		  4
#define SLEEP_NS    100000000

void *task(void *arg){
	int tid;
	tid = (int)(long int)arg;

	while(1)
  {
		gtWait(tid);
		printf("thread %d entrando na regiao critica.\n", tid);
    nanosleep((struct timespec[]){{0, 5*SLEEP_NS}}, NULL);
		printf("thread %d saindo da regiao critica.\n", tid);
		gtPost(tid);
    nanosleep((struct timespec[]){{0, 5*SLEEP_NS}}, NULL);
	}
}

int main(void){
	long int i;
	pthread_t threads[THREADS];

  gtInit(THREADS);

	for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task, (void *)i);

	pthread_exit(NULL);	
	return(0);
}

