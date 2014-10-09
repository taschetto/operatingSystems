#include <stdio.h>
#include <malloc.h>

extern end;
extern etext;

main()
{
  char *s;
  char c;
  int i = 357;
  int *ii;

  ii = (int *) malloc(sizeof(int));

  printf("&i = 0x%lx\n", &i);
  printf("&ii = 0x%lx\n", &ii);
  printf("ii = 0x%lx\n", ii);
  printf("&etext = 0x%lx\n", &etext);
  printf("&end = 0x%lx\n", &end);

  printf("\n");

  printf("Enter memory location in hex (start with 0x): ");
  fflush(stdout);

  scanf("0x%lx", &s);

  printf("Reading 0x%lx: ", s);
  fflush(stdout);
  c = *s;
  printf("%d\n", c);
  printf("Writing %d back to 0x%lx: ", c, s);
  fflush(stdout);
  *s = c;
  printf("ok\n");
}
