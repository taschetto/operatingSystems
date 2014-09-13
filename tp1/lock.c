#include <stdlib.h>
#include <time.h>
#include "lock.h"

#ifndef FALSE
  #define FALSE 0
#endif

#ifndef TRUE
  #define TRUE 1
#endif

int* flag;
int turn;
int threadCount;

void lockInit(int threads)
{
  threadCount = threads;
  flag = malloc(threadCount * sizeof(int));
  int i;
  for (i = 0; i < threadCount; i++)
    flag[i] = FALSE;
  turn = 0;
}

int gtCheck(int tid)
{
  int i = 0;
  for (i = 0; i < threadCount; i++)
  {
    if (i == tid) continue;
    if (flag[i] == TRUE) return TRUE;
  }

  return FALSE;
}

void lock(int tid)
{
  flag[tid] = TRUE;
  while (gtCheck(tid) == TRUE)
  {
    if (turn != tid)
    {
      flag[tid] = FALSE;
      while (turn != tid);
      flag[tid] = TRUE;
    }
  }
}

void unlock(int tid)
{
  srand(time(NULL));
  turn = rand() % threadCount;
  flag[tid] = FALSE;
}
