#include "helpers.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>

#define DELIM " \n\0"

void ok()
{
  printf("%s [ OK ]%s\n", GREEN, RESET);
}

void error()
{
  printf("%s [ ERROR ]%s\n%s (errno=%d).\n", RED, RESET, strerror(errno), errno);
}

char ** tokenize(char* str, int* count)
{
  char** tokens;
  char* token = strtok(str, DELIM);
  int n = 0;

  while (token) {
    tokens = realloc(tokens, sizeof (char*) * ++n);
    if (tokens == NULL) exit(-1);

    tokens[n - 1] = token;
    token = strtok(NULL, DELIM);
  }

  tokens = realloc(tokens, sizeof(char*) * (n + 1));
  tokens[n] = 0;

  *count = n;
  
  return tokens;
}
