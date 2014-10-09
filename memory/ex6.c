#include <stdio.h>

extern end;
extern etext;

main()
{
  char c;
  char iptr[10000];

  printf("&c = 0x%lx, iprt = 0x%x ... ", &c, iptr);
  fflush(stdout);
  c = iptr[0];
  printf("ok\n");
  main();
}
