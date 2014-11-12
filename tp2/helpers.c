#define _BSD_SOURCE
#include "helpers.h"
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <libgen.h>

void fail(char* message)
{
  printf("%s%s%s", RED, message, RESET);
}

void ok()
{
  printf("%s [ OK ]%s\n", GREEN, RESET);
}

void error()
{
  if (errno != 0) // 0 == SUCCESS
    printf("%s [ ERROR ]%s\n%s (errno=%d).\n", RED, RESET, strerror(errno), errno);
}

char ** tokenize(char* str, int* count, char* delim)
{
  char** tokens= NULL;
  char* token = strtok(str, delim);
  int n = 0;

  while (token) {
    tokens = realloc(tokens, sizeof (char*) * ++n);
    if (tokens == NULL) exit(-1);

    tokens[n - 1] = token;
    token = strtok(NULL, delim);
  }

  tokens = realloc(tokens, sizeof(char*) * (n + 1));
  tokens[n] = 0;

  *count = n;

  return tokens;
}