#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "io.h"

void io_test()
{
  char *test_cluster;
  int status;

  printf("Allocate memory for 'test_cluster'...");

  test_cluster = malloc(CLUSTER_SIZE);
  if (test_cluster == NULL)
  {
    error();
    return;
  }

  ok();
  printf("Create new file system in 'fat.part'...");

  status = create_new_fs("fat.part");
  if (status != 0)
  {
    error();
    return;
  }

  ok();
  printf("Load file system from 'fat.part'...");

  status = load_fs("fat.part");
  if (status != 0)
  {
    error();
    return;
  }

  ok();
  printf("Write into 'test_cluster'...");

  memset(test_cluster, 0x55, CLUSTER_SIZE);
  status = write_cluster(1, test_cluster);
  if (status != 0)
  {
    error();
    return;
  }

  ok();
  printf("Read from 'test_cluster'...");

  memset(test_cluster, 0x00, CLUSTER_SIZE);
  status = read_cluster(1, test_cluster);
  if (status != 0 || test_cluster[2] != 0x55)
  {
    error();
    return;
  }

  ok();
  printf("Unload file system from 'fat.part'...");

  status = unload_fs();
  if (status != 0)
  {
    error();
    return;
  }
  ok();
}

int main()
{
  io_test();
}

