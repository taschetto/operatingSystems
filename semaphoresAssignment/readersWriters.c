#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>

#define READERS 2
#define WRITERS 1

sem_t mutex, db;
int rc = 0;
int dado = 0;

void *reader(void *arg)
{
  long int i;
  i = (long int)arg;
  int dadoLocal;

  while(1)
  {
    sem_wait(&mutex);
    rc++;
    if (rc == 1) sem_wait(&db);
    sem_post(&mutex);
    dadoLocal = dado;
    printf("leitor %ld le dado=%d\n", i, dado);
    sem_wait(&mutex);
    rc--;
    if (rc == 0) sem_post(&db);
    sem_post(&mutex);
    printf("leitor %ld usa dadoLocal=%d\n", i, dadoLocal);
  }
}

void *writer(void *arg){
  long int i;
  i = (long int)arg;
  int dadoLocal;

  while(1)
  {
    dadoLocal = rand();
    printf("escritor %ld prepara dadoLocal=%d\n", i, dado);
    sem_wait(&db);
    dado = dadoLocal;
    printf("escritor %ld escreve dado=%d\n", i, dado);
    sem_post(&db);
  }
}

int main(void){
  long int i;
  pthread_t treader[READERS], twriter[WRITERS];
        
  sem_init(&mutex, 0, 1);
  sem_init(&db, 0, 1);

  for(i = 1; i <= READERS; i++)
    pthread_create(&treader[i-1], NULL, reader, (void *)i);

  for (i = 1; i <= WRITERS; i++)
    pthread_create(&twriter[i-1], NULL, writer, (void *)i);

  pthread_exit(NULL); 
  return(0);
}

