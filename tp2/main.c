#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "io.h"
#include "fs.h"
	
uint32_t fat[1024];
uint8_t data_block[2048];
struct dir_entry dir[64];

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


void fs_test()
{
	int status;
	const char *path1[] = {""};
	const char *path2[] = {"etc"};
	struct dir_entry entries[64];
	int entries_count;

	load_fs("fat.part");

	format();

	create_dir_entry(path1, 0, "etc", DIR_ENTRY_ATTR_DIRECTORY);
	create_dir_entry(path2, 1, "fstab", DIR_ENTRY_ATTR_FILE);

	list_dir(path1, 0, entries, &entries_count);

	for (int i = 0; i < entries_count; i++) {
		printf ("--->%s     %s\n", entries[i].attributes==DIR_ENTRY_ATTR_DIRECTORY?"D":"F", entries[i].filename);
	}

	list_dir(path2, 1, entries, &entries_count);

	for (int i = 0; i < entries_count; i++) {
		printf ("--->%s     %s\n", entries[i].attributes==DIR_ENTRY_ATTR_DIRECTORY?"D":"F", entries[i].filename);
	}

	unload_fs();
}

int main()
{
  io_test();

  fs_test();

	return 0;
}

