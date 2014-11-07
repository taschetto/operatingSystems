#define _BSD_SOURCE
#include <stdio.h>
#include <string.h>
#include <libgen.h>
#include "fs_cmd.h"
#include "fs.h"
#include "io.h"
#include "helpers.h"

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

void fs_ls(char* dir)
{
  printf("ls %s\n", dir);
}

void fs_mkdir(char* dir)
{
  printf("mkdir %s\n", dir);
}

void fs_rmdir(char* dir)
{
  printf("rmdir %s\n", dir);
}

void fs_create(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);
  printf("create %s - %s\n", p, f);
}

void fs_rm(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);
  printf("rm %s - %s\n", p, f);
}

void fs_write(char* content, char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);
  printf("write \"%s\" %s - %s\n", content, p, f);
}

void fs_cat(char* filepath)
{
  char *dirc, *basec, *p, *f;
  dirc = strdup(filepath);
  basec = strdup(filepath);
  p = dirname(dirc);
  f = basename(basec);
  printf("cat %s - %s\n", p, f);
}
