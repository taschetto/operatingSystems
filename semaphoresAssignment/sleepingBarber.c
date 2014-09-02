#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 6
#define CUSTOMERS 10

sem_t customers;
sem_t barbers;
sem_t mutex;

int waiting = 0;

void *barber(void *arg)
{
  while(1)
  {
    printf("Barbeiro dormindo.\n");
    sem_wait(&customers);
    sem_wait(&mutex);
    printf("Barbeiro acordou.\n");
    waiting--;
    sem_post(&barbers);
    sem_post(&mutex);
    printf("Barbeiro cortou cabelo do cliente.\n");
  }
}

void *customer(void *arg)
{
  long int i;
  i = (long int)arg;

  sem_wait(&mutex);
  if (waiting < CHAIRS)
  {
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
  pthread_t tbarber, tcustomer[CUSTOMERS];
        
  sem_init(&customers, 0, 0);
  sem_init(&barbers, 0, 0);
  sem_init(&mutex, 0, 1);

  pthread_create(&tbarber, NULL, barber, NULL);

  for (i = 1; i <= CUSTOMERS; i++)
    pthread_create(&tcustomer[i - 1], NULL, customer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

