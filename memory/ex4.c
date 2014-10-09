#include <stdio.h>
#include <malloc.h>
#include <sys/types.h>

main()
{
  int *i1, *i2;

  printf("sbrk(0) before malloc(4): 0x%x\n", sbrk(0));
  i1 = (int*)malloc(4);
  printf("sbrk(0) after `i1 = (int*)malloc(4)': 0x%x\n", sbrk(0));
  i2 = (int*)malloc(4);
  printf("sbrk(0) after `i2 = (int*)malloc(4)': 0x%x\n", sbrk(0));
  printf("i1 = 0x%x, i2 = 0x%x\n", i1, i2);
}
