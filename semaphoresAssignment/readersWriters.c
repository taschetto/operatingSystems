#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>

#define READERS 2
#define WRITERS 2

sem_t readAcc;
sem_t writeAcc;

int readCount = 0;
int data = 0;

void* reader(void *arg)
{
  long int i;
  i = (long int)arg;
  while(1)
  {
    sem_wait(&readAcc);
      readCount++;
      if (readCount == 1)
        sem_wait(&writeAcc);
    sem_post(&readAcc);
  
    printf("Reader %ld reads data (%d)\n", i, data);
  
    sem_wait(&readAcc);
      readCount--;
      if (readCount == 0)
        sem_post(&writeAcc);
    sem_post(&readAcc);
  }
}

void* writer(void *arg)
{
  long int i;
  i = (long int)arg;

  while(1)
  {
    sem_wait(&writeAcc);
      data++;
      printf("Writer %ld updates data (%d).\n", i, data);
    sem_post(&writeAcc);
  }
}

int main(void){
  long int i;
  pthread_t treader[READERS], twriter[WRITERS];
        
  sem_init(&readAcc, 0, 1);
  sem_init(&writeAcc, 0, 1);

  for(i = 1; i <= READERS; i++)
    pthread_create(&treader[i - 1], NULL, reader, (void *)i);

  for (i = 1; i <= WRITERS; i++)
    pthread_create(&twriter[i - 1], NULL, writer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

