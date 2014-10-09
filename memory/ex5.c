#include <stdio.h>

extern end;
extern etext;

main(int argc, char **argv)
{
  int i;

  printf("argc = %d, argc = 0x%x, &argv = 0x%x, &i = 0x%x\n", argc, &argv, &argv, &i);
  if (argc > 0) main(argc - 1, argv);
}
