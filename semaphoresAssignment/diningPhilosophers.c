#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define N 5
#define LEFT (i+N-1)%N
#define RIGHT (i+1)%N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

int state[N];

sem_t mutex;
sem_t s[N];

void *philosopher(void *arg)
{
  long int i;
  i = (long int)arg;

  while(1)
  {
    printf ("Philosopher %ld is done thinking and now ready to eat.\n", i);
    take_forks(i);
    eat();
    put_forks(i);
  }
}

void take_forks(int i)
{
  sem_wait(&mutex);
  state[i] = HUNGRY;
  test(i);
  sem_post(&mutex);
  sem_wait(&s[i]);
}

void put_forks(int i)
{
  sem_wait(&mutex);
  state[i] = THINKING;
  test(LEFT);
  test(RIGHT);
  sem_post(&mutex);
}

void test(int i)
{
  if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING)
  {
    state[i] = EATING;
    sem_post(&s[i]);
  }
}

int main(void){
  long int i;
  pthread_t tphil[N];
        
  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);

  for(i = 1; i <= READERS; i++)
    pthread_create(&treader[i-1], NULL, reader, (void *)i);

  for (i = 1; i <= WRITERS; i++)
    pthread_create(&twriter[i-1], NULL, writer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

