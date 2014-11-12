#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helpers.h"
#include "fs_cmd.h"

#define BUFFER_SIZE 512

enum command
{
  INIT = 0,
  LOAD,
  LS,
  MKDIR,
  RMDIR,
  CREATE,
  RM,
  WRITE,
  CAT,
  EXIT,
  UNKNOWN
};

int arguments[10] = {1, 1, 2, 2, 2, 2, 2, 3, 2, 1};

enum command map(char* str)
{
  if (strcmp(str, "init")   == 0) return INIT;
  if (strcmp(str, "load")   == 0) return LOAD;
  if (strcmp(str, "ls")     == 0) return LS;
  if (strcmp(str, "mkdir")  == 0) return MKDIR;
  if (strcmp(str, "rmdir")  == 0) return RMDIR;
  if (strcmp(str, "create") == 0) return CREATE;
  if (strcmp(str, "rm")     == 0) return RM;
  if (strcmp(str, "write")  == 0) return WRITE;
  if (strcmp(str, "cat")    == 0) return CAT;
  if (strcmp(str, "exit")   == 0) return EXIT;

  return UNKNOWN;
}

int validate(enum command cmd, int actual)
{
  int expected = arguments[cmd];
  if (expected != actual)
    return -1;

  return 0;
}

int shell()
{
  char buffer[BUFFER_SIZE];
  char ** tokens = NULL;
  int count = 0;

  while(1)
  {
    if (tokens != NULL)
      free(tokens);

    printf("%sfat%s> ", BLUE, RESET);
    fgets(buffer, BUFFER_SIZE, stdin);

    tokens = tokenize(buffer, &count, " \n\0");
    if (count == 0) continue;

    enum command cmd = map(tokens[0]);
    if (cmd == UNKNOWN)
    {
      printf("%sShell error: unknown command '%s'.\n%s", RED, tokens[0], RESET);
      continue;
    }

    if (validate(cmd, count) < 0)
    {
      printf("%sShell error: invalid arguments for '%s'.\n%s", RED, tokens[0], RESET);
      continue;
    }

    if (cmd == EXIT) break;

    switch (cmd)
    {
      case INIT:
        fs_init();
        break;
      case LOAD:
        fs_load();
        break;
      case LS:
        fs_ls(tokens[1]);
        break;
      case MKDIR:
        fs_mkdir(tokens[1]);
        break;
      case RMDIR:
        fs_rmdir(tokens[1]);
        break;
      case CREATE:
        fs_create(tokens[1]);
        break;
      case RM:
        fs_rm(tokens[1]);
        break;
      case WRITE:
        fs_write(tokens[1], tokens[2]);
        break;
      case CAT:
        fs_cat(tokens[1]);
        break;
      case EXIT:
      case UNKNOWN:
        // do nothing
        break;
    }
  }

  if (tokens != NULL)
    free(tokens);

  return 0;
}
