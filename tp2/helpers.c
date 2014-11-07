#include "helpers.h"
#include <string.h>
#include <errno.h>

void ok()
{
  printf("%s [ OK ]%s\n", GREEN, RESET);
}

void error()
{
  printf("%s [ ERROR ]%s %s (errno=%d).\n", RED, RESET, strerror(errno), errno);
}

