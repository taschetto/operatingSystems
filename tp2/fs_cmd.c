#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "fs_cmd.h"
#include "fs.h"
#include "helpers.h"

extern struct dir_entry dir[64];

void fs_init()
{
  printf("Initializing file system...");
  if (create_new_fs("fat.part") < 0)
    error();
  else
    ok();
}

void fs_load()
{
  printf("Loading file system from 'fat.part'...");
  if (load_fs("fat.part") < 0)
    error();
  else
    ok();
}

void fs_ls(char* dirpath)
{
  char** path = NULL;
  int depth;
  path = tokenize(dirpath, &depth, "/\0");

  if (list_dir((const char**)path, depth) < 0)
  {
    printf("%s'ls' failed!%s\n", RED, RESET);
    return;
  }

  for (int i = 0; i < 64; i++) {
    if (dir[i].first_block > 0) {
      printf ("%s     %s\n", dir[i].attributes==DIR_ENTRY_ATTR_DIRECTORY?"D":"F", dir[i].filename);
    }
  }
}

void fs_mkdir(char* dirpath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(dirpath);
  basec = strdup(dirpath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (create_file_or_dir((const char**)path, depth, f, DIR_ENTRY_ATTR_DIRECTORY) < 0)
    printf("%s'mkdir' failed!%s\n", RED, RESET);
}

void fs_rmdir(char* dirpath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(dirpath);
  basec = strdup(dirpath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (rm_dir((const char**)path, depth, f) < 0)
    printf("%s'rmdir' failed!%s\n", RED, RESET);
}

void fs_create(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (create_file_or_dir((const char**)path, depth, f, DIR_ENTRY_ATTR_FILE) < 0)
    printf("%s'create' failed!%s\n", RED, RESET);
}

void fs_rm(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (rm_file((const char**)path, depth, f) < 0)
    printf("%s'rm' failed!%s\n", RED, RESET);
}

void fs_write(char* content, char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (write_to_file((const char**)path, depth, f, (uint8_t*)content, (char)strlen(content)) < 0)
    printf("%s'write' failed!%s\n", RED, RESET);
}

void fs_cat(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  char* content = NULL;
  unsigned int content_size = 0;
  if (read_from_file((const char**)path, depth, f, (uint8_t*)content, &content_size) < 0)
  {
    printf("%s'cat' failed!%s\n", RED, RESET);
    return;
  }

  printf("cat com sucesso\n");

  printf("%s\n", (char*)content);
}
