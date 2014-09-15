//
// Os filosofos glutoes.
//

#define _BSD_SOURCE
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#include "lock.h"

#define NUM_OF_PHILOSOPHERS 5
#define LEFT(x) ((x+NUM_OF_PHILOSOPHERS-1)%NUM_OF_PHILOSOPHERS)
#define RIGHT(x) ((x+1)%NUM_OF_PHILOSOPHERS)

#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[NUM_OF_PHILOSOPHERS];
sem_t s[NUM_OF_PHILOSOPHERS];

void eat(unsigned long i)
{
	printf("C   %ld comendo...\n", i);
	sleep(1);
}

void think(unsigned long i)
{
	printf("P   %ld pensando...\n", i);
	sleep(1);
}

void test(unsigned long i)
{
	if (state[i] == HUNGRY && state[LEFT(i)] != EATING && state[RIGHT(i)] != EATING) {
		state[i] = EATING;
		sem_post(&s[i]);
	}
}

void take_forks(unsigned long i)
{
	lock((int)i);
	state[i] = HUNGRY;
	test(i);
	unlock((int)i);
	sem_wait(&s[i]);
}

void put_forks(unsigned long i)
{
	lock((int)i);
	state[i] = THINKING;
	test(LEFT(i));
	test(RIGHT(i));
	unlock((int)i);
}

void *philosopher(void *thread_id)
{
	while (1) {
		think((unsigned long)thread_id);
		take_forks((unsigned long)thread_id);
		eat((unsigned long)thread_id);
		put_forks((unsigned long)thread_id);
	}
	
	pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
	pthread_t philosophers[NUM_OF_PHILOSOPHERS];
	int ret;

	lockInit(NUM_OF_PHILOSOPHERS);
	
	for (int i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
		sem_init(&s[i], 0, 0);
	}

	for (unsigned long i = 0; i < NUM_OF_PHILOSOPHERS; i++) {
		ret = pthread_create(&philosophers[i], NULL, philosopher, (void *) i);
		if (ret < 0) {
			printf("pthread_create falhou\n");
			return -1;
		}
	}

	// Ja que nenhum filosofo vai terminar, fica preso aqui, esperando CTRL-C
	pthread_join(philosophers[0], NULL);

	return 0;
}
