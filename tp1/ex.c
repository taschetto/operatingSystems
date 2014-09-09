#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define THREADS		  10
#define SLEEP_NS    500000000

sem_t mutex;

int r = 0;

void *task(void *arg){
	int tid;

	tid = (int)(long int)arg;
	while(1){
		sem_wait(&mutex);
		printf("thread %d entrando na regiao critica.\n", tid);
    nanosleep((struct timespec[]){{0, SLEEP_NS}}, NULL);
		printf("thread %d saindo da regiao critica.\n", tid);
		sem_post(&mutex);
    nanosleep((struct timespec[]){{0, SLEEP_NS}}, NULL);
	}
}

int main(void){
	long int i;
	pthread_t threads[THREADS];
	
	sem_init(&mutex, 0, 1);

	for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task, (void *)i);

	pthread_exit(NULL);	
	return(0);
}

