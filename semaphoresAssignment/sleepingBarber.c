#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define CHAIRS 6
#define CUSTOMERS 10

sem_t barberReady;
sem_t accessWRSeats;
sem_t custReady;
int waiting = 0;

void *barber(void *arg)
{
  while(1)
  {
    sem_wait(&custReady);
    sem_wait(&accessWRSeats);
    waiting--;
    sem_post(&barberReady);
    sem_post(&accessWRSeats);
    printf("barbeiro corta cabelo do cliente\n");
  }
}

void *customer(void *arg)
{
  long int i;
  i = (long int)arg;
  while(1)
  {
  sem_wait(&accessWRSeats);
  if (waiting < CHAIRS)
  {
    waiting++;
    sem_post(&custReady);
    sem_post(&accessWRSeats);
    sem_wait(&barberReady);
    printf("cliente %ld corta o cabelo\n", i);
  }
  else
  {
    sem_post(&accessWRSeats);
    printf("cliente %ld vai embora sem cortar o cabelo\n", i);
  }
  }

  return 0;
}

int main(void){
  long int i;
  pthread_t tbarber, tcustomer[CUSTOMERS];
        
  sem_init(&barberReady, 0, 0);
  sem_init(&accessWRSeats, 0, 1);
  sem_init(&custReady, 0, 0);

  pthread_create(&tbarber, NULL, barber, NULL);

  for (i = 1; i <= CUSTOMERS; i++)
    pthread_create(&tcustomer[i-1], NULL, customer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

