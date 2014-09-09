#include <stdlib.h>
#include "gtwait.h"

#ifndef FALSE
  #define FALSE       0
#endif

#ifndef TRUE
  #define TRUE        1
#endif

int* flag;
int turn;

void gtInit(int threads)
{
  flag = malloc(threads * sizeof(int));
  int i;
  for (i = 0; i < threads; i++)
    flag[i] = FALSE;
  turn = 0;
}

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
      while (turn != tid); //nanosleep((struct timespec[]){{0, SLEEP_NS}}, NULL);
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
