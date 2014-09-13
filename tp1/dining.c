#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>
#include "lock.h"

#define SLEEP_NS 100000000

#define N 5			        /* number of philosophers */
#define LEFT (i+N-1)%N 	/* number of i’s left neighbor */
#define RIGHT (i+1)%N 	/* number of i’s right neighbor */
#define THINKING 0 		  /* philosopher is thinking */
#define HUNGRY 1 		    /* philosopher is trying to get forks */
#define EATING 2 		    /* philosopher is eating */

int state[N]; 			    /* array to keep track of everyone’s state */
sem_t mutex; 			      /* mutual exclusion for critical regions */
sem_t s[N]; 			      /* one semaphore per philosopher */

void* philosopher(void* arg);
void take_forks(long int i);
void put_forks(long int i);
void test(long int i);

int main()
{
	long int i;
	pthread_t t[N];

	sem_init(&mutex, 0, 1);
  //lockInit(N);

	for (i = 1; i <= N; i++)
		sem_init(&s[i - 1], 0, 1);

	for (i = 1; i <= N; i++)
		pthread_create(&t[i - 1], NULL, philosopher, (void *)i);

	pthread_exit(NULL); 
  return 0;
}

void* philosopher(void* arg) 	/* i: philosopher number, from 0 to N-1 */
{
  long int i;
  i = (long int)arg;
	while (1)		/* repeat forever */
	{
		printf("Philosopher %ld thinking.\n", i); 	/* philosopher is thinking */
		take_forks(i); 	/* acquire two forks or block */
 		printf("Philosopher %ld eating.\n", i); 		/* yum-yum, spaghetti */
		put_forks(i);	/* put both forks back on table */
	}
}

void take_forks(long int i) 		/* i: philosopher number, from 0 to N-1 */
{
  //lock(i);
	sem_wait(&mutex); 	/* enter critical region */
	state[i] = HUNGRY; 	/* record fact that philosopher i is hungry */
	test(i); 		        /* try to acquire 2 forks */
  nanosleep((struct timespec[]){{0, 5 * SLEEP_NS}}, NULL);

  //unlock(i);

	sem_post(&mutex); 	/* exit critical region */
	sem_wait(&s[i]); 		/* block if forks were not acquired */
}

void put_forks(long int i) 	/* i: philosopher number, from 0 to N-1 */
{
  //lock(i);
	sem_wait(&mutex); 	  /* enter critical region */
	state[i] = THINKING; 	/* philosopher has finished eating */
	test(LEFT); 		      /* see if left neighbor can now eat */
	test(RIGHT); 		      /* see if right neighbor can now eat */
  nanosleep((struct timespec[]){{0, 5 * SLEEP_NS}}, NULL);
  //unlock(i);
	sem_post(&mutex);		  /* exit critical region */
}

void test(long int i)        /* i: philosopher number, from 0 to N-1 */
{
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
	{
		state[i] = EATING;
		sem_post(&s[i]);
	}
}

