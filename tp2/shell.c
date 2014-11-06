#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "helpers.h"

#define BUFFER_SIZE 512
#define DELIM " \n\0"

char ** tokenize(char* str, int* count)
{
  char** tokens = NULL;
  char* token = strtok (str, DELIM);
  int n = 0;

  while (token) {
    tokens = realloc (tokens, sizeof (char*) * ++n);
    if (tokens == NULL) exit(-1);

    tokens[n - 1] = token;
    token = strtok (NULL, DELIM);
  }

  tokens = realloc (tokens, sizeof (char*) * (n + 1));
  tokens[n] = 0;

  *count = n;
  
  return tokens;
}

int shell()
{
  char buffer[BUFFER_SIZE];
  char ** tokens = NULL;
  int count = 0;

  while(1)
  {
    printf("%sfat%s> ", BLUE, RESET);
    fgets(buffer, BUFFER_SIZE, stdin);

    tokens = tokenize(buffer, &count);

    if (count == 0)
    {
      free(tokens);
      continue;
    }

    if (strcmp(tokens[0], "exit") == 0)
    {
      break;
    }
    else if (strcmp(tokens[0], "ls") == 0)
    {
      printf("ls\n");
    }
    else if (strcmp(tokens[0], "mkdir") == 0)
    {
      printf("mkdir\n");
    }
    else
    {
      printf("unknown command '%s'\n", tokens[0]);
    }
  }

  if (tokens != NULL)
    free(tokens);

  return 0;
}
