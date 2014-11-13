#define _BSD_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libgen.h>
#include "fs_cmd.h"
#include "fs.h"
#include "helpers.h"

extern struct dir_entry dir[64];

char* get_color(uint8_t attr)
{
  return attr == DIR_ENTRY_ATTR_DIRECTORY ? GRAY : YELLOW;
}

char* get_label(uint8_t attr)
{
  return attr == DIR_ENTRY_ATTR_DIRECTORY ? "d" : "f";
}

void fs_init()
{
  if (create_new_fs("fat.part") < 0)
    fail("'init' failed!\n");
}

void fs_load()
{
  if (load_fs("fat.part") < 0)
    fail("'load' failed!\n");
}

void fs_ls(char* dirpath)
{
  int depth = 0;
  char** path = tokenize(dirpath, &depth, "/\0");

  if (list_dir((const char**)path, depth) < 0)
  {
    fail("'ls' failed!\n");
    return;
  }

  for (int i = 0; i < 64; i++) {
    if (dir[i].first_block > 0) {
      printf ("%s%s\t%s\n%s", get_color(dir[i].attributes), get_label(dir[i].attributes), dir[i].filename, RESET);
    }
  }

  free(path);
}

void fs_mkdir(char* dirpath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(dirpath);
  basec = strdup(dirpath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (create_file_or_dir((const char**)path, depth, f, DIR_ENTRY_ATTR_DIRECTORY) < 0)
    fail("'mkdir' failed!\n");

  free(path);
}

void fs_rmdir(char* dirpath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(dirpath);
  basec = strdup(dirpath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (rm_dir((const char**)path, depth, f) < 0)
    fail("'rmdir' failed!\n");

  free(path);
}

void fs_create(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
    depth = 0;
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (create_file_or_dir((const char**)path, depth, f, DIR_ENTRY_ATTR_FILE) < 0)
    fail("'create' failed!\n");

  free(path);
}

void fs_rm(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (rm_file((const char**)path, depth, f) < 0)
    fail("'rm' failed!\n");

  free(path);
}

void fs_write(char content[], char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
  }
  else
    path = tokenize(p, &depth, "/\0");

  if (write_to_file((const char**)path, depth, f, (uint8_t*)content, strlen(content)) < 0)
    fail("'write' failed!\n");

  free(path);
}

void fs_cat(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);

  char** path = NULL;
  int depth = 0;

  if (p[0] == '.')
  {
    path = malloc(sizeof(char));
    path[0] = "";
  }
  else
    path = tokenize(p, &depth, "/\0");

  unsigned int file_size;
  get_file_size((const char**)path, depth, f, &file_size);

  uint8_t *read_data;
  read_data = malloc(file_size + 1);

  if (read_from_file((const char**)path, depth, f, read_data, &file_size) < 0)
  {
    fail("'cat' failed!\n");
    return;
  }

  read_data[file_size] = 0;
  printf("%s\n", read_data);

  free(path);
  free(read_data);
}
