#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "helpers.h"
#include "shell.h"
#include "fs.h"

// Allowed fs data structures
uint32_t fat[1024];
uint8_t data_block[2048];
struct dir_entry dir[64];

void io_test()
{
  int status;

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

  memset(data_block, 0x55, CLUSTER_SIZE);
  status = write_cluster(1, data_block);
  if (status != 0)
  {
    error();
    return;
  }

//  for (int i = 0; i < 1024; i++) {
//	  memset(data_block, i%255, CLUSTER_SIZE);
//	  status = write_cluster(i, data_block);
//  }

  ok();
  printf("Read from 'test_cluster'...");

  memset(data_block, 0x00, CLUSTER_SIZE);
  status = read_cluster(1, data_block);
  if (status != 0 || data_block[2] != 0x55)
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
	const char *path1[] = {""};
	const char *path2[] = {"y"};
	uint8_t data[] = "0123456789";
	uint8_t *read_data;
	unsigned int file_size;



	load_fs("fat.part");
/*
	format();

	create_file_or_dir(path1, 0, "etc", DIR_ENTRY_ATTR_DIRECTORY);
	create_file_or_dir(path2, 1, "fstab", DIR_ENTRY_ATTR_FILE);
	create_file_or_dir(path2, 1, "init.d", DIR_ENTRY_ATTR_DIRECTORY);

	rm_dir(path2, 1, "init.d");
	list_dir(path1, 0);

	for (int i = 0; i < 64; i++) {
		if (dir[i].first_block > 0) {
			printf ("%s     %s\n", dir[i].attributes==DIR_ENTRY_ATTR_DIRECTORY?"D":"F", dir[i].filename);
		}
	}

//	rm_file(path2, 1, "fstab");
	list_dir(path2, 1);

	for (int i = 0; i < 64; i++) {
		if (dir[i].first_block > 0) {
//			printf ("%s     %s\n", dir[i].attributes==DIR_ENTRY_ATTR_DIRECTORY?"D":"F", dir[i].filename);
		}
	}*/
	// Write to several clusters...sorry for the mess! :)
	for (int i = 0; i < 512; i++) {
		write_to_file(path2, 1, "asd", data, sizeof(data) - 1);
	}


/**	get_file_size(path2, 1, "fstab", &file_size);


	read_data = malloc(file_size + 1);
	printf("file_size1: %d\n", file_size);
	read_from_file(path2, 1, "fstab", read_data, &file_size);
	printf("file_size2: %d\n", file_size);
	read_data[file_size] = 0;
	printf("CONTENT:%s", read_data);

	free(read_data);



	unload_fs();*/
}

int main()
{
  shell();
  unload_fs();
  return 0;
}

