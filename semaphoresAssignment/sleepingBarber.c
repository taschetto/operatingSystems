#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 3
#define CUSTOMERS 15
#define BARBERS 2

sem_t customers;
sem_t barbers;
sem_t mutex;

int waiting = 0;

void *barber(void *arg)
{
  long int i;
  i = (long int)arg;
  while(1)
  {
    printf("Barbeiro %ld dormindo.\n", i);
    sem_wait(&customers);
    sem_wait(&mutex);
    printf("Barbeiro %ld acordou.\n", i);
    waiting--;
    sem_post(&barbers);
    sem_post(&mutex);
    printf("Barbeiro %ld cortou cabelo de um cliente.\n", i);
  }
}

void *customer(void *arg)
{
  long int i;
  i = (long int)arg;

  printf("Cliente %ld chegou ao salao.\n", i);
  sem_wait(&mutex);
  if (waiting < CHAIRS)
  {
    printf("Cliente %ld sentou numa cadeira para esperar.\n", i);
    waiting++;
    sem_post(&customers);
    sem_post(&mutex);
    sem_wait(&barbers);
    printf("Cliente %ld teve o cabelo cortado.\n", i);
  }
  else
  {
    sem_post(&mutex);
    printf("Cliente %ld vai embora sem cortar o cabelo.\n", i);
  }
  return 0;
}

int main(void){
  long int i;
  pthread_t tbarber[BARBERS], tcustomer[CUSTOMERS];
        
  sem_init(&customers, 0, 0);
  sem_init(&barbers, 0, 0);
  sem_init(&mutex, 0, 1);

  for (i = 1; i <= BARBERS; i++)
    pthread_create(&tbarber[i - 1], NULL, barber, (void *)i);

  for (i = 1; i <= CUSTOMERS; i++)
    pthread_create(&tcustomer[i - 1], NULL, customer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

