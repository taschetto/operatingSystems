#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define THREADS		  4
#define SLEEP_NS    100000000

#define FALSE       0
#define TRUE        1

int flag[THREADS];
int turn = 0;

int checkOthers(int tid, int threads)
{
  int i = 0;
  for (i = 0; i < threads; i++)
  {
    if (i == tid) continue;
    if (flag[i] == TRUE) return TRUE;
  }

  return FALSE;
}

void gtwait(int tid, int threads)
{
  flag[tid] = TRUE;
  while (checkOthers(tid, threads) == TRUE)
  {
    if (turn != tid)
    {
      flag[tid] = FALSE;
      while (turn != tid) nanosleep((struct timespec[]){{0, SLEEP_NS}}, NULL);
      flag[tid] = TRUE;
    }
  }
}

void gtpost(int tid, int threads)
{
  turn++;
  turn %= threads;
  flag[tid] = FALSE;
}

void *task(void *arg){
	int tid;
	tid = (int)(long int)arg;

  printf("hello from thread %d!\n", tid);

	while(1)
  {
		gtwait(tid, THREADS);
		printf("thread %d entrando na regiao critica.\n", tid);
    nanosleep((struct timespec[]){{0, 5*SLEEP_NS}}, NULL);
		printf("thread %d saindo da regiao critica.\n", tid);
		gtpost(tid, THREADS);
    nanosleep((struct timespec[]){{0, 5*SLEEP_NS}}, NULL);
	}
}

int main(void){
	long int i;
	pthread_t threads[THREADS];

  for(i = 0; i < THREADS; i++)
    flag[i] = FALSE;
	
	for(i = 0; i < THREADS; i++)
		pthread_create(&threads[i], NULL, task, (void *)i);

	pthread_exit(NULL);	
	return(0);
}

