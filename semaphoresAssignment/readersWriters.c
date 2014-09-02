#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define READERS 5
#define WRITERS 1

sem_t mutex;
sem_t db;

int readCount = 0;
int data = 0;

void *reader(void *arg)
{
  long int i;
  i = (long int)arg;

  while(1)
  {
    sem_wait(&mutex);
      readCount++;
      if (readCount == 1)
        sem_wait(&db);
    sem_post(&mutex);

    printf("Reader %ld (%d) reads data (%d)\n", i, readCount, data);

    sem_wait(&mutex);
      readCount--;
      if (readCount == 0)
        sem_post(&db);
    sem_post(&mutex);
  }
}

void *writer(void *arg){
  long int i;
  i = (long int)arg;

  while(1)
  {
    sem_wait(&db);
      data = data + 1;
      printf("Writer %ld updates data (%d).\n", i, data);
    sem_post(&db);
  }
}

int main(void){
  long int i;
  pthread_t treader[READERS], twriter[WRITERS];
        
  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);

  for(i = 1; i <= READERS; i++)
    pthread_create(&treader[i - 1], NULL, reader, (void *)i);

  for (i = 1; i <= WRITERS; i++)
    pthread_create(&twriter[i - 1], NULL, writer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

