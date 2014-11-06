/**
 * @file   io.c
 * @date   02/11/2014
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>

#include "io.h"

#define NO_DESCRIPTOR -1
int current_descriptor = NO_DESCRIPTOR;

int create_new_fs(const char *fs_image_name)
{
	int fd;
	int image_size = CLUSTER_SIZE * NUMBER_OF_CLUSTERS;
	char initial_data[CLUSTER_SIZE] = { 0 };

	fd = open(fs_image_name, O_RDWR | O_CREAT | O_TRUNC, S_IRWXU);
	if (fd < 0)
		return -1;

  if (write(fd, initial_data, image_size) < 0)
  {
    close(fd);
    return -1;
  }

  if (close(fd) < 0)
    return -1;

  return 0;
}

int load_fs(const char *fs_image_name)
{
	int fd;

  if (current_descriptor != NO_DESCRIPTOR)
    return -1;

  fd = open(fs_image_name, O_RDWR);
  if (fd < 0)
    return -1;

  current_descriptor = fd;

  return 0;
}

int unload_fs()
{
	if (current_descriptor == NO_DESCRIPTOR)
    return -1;

	if (close(current_descriptor) < 0)
    return -1;

	current_descriptor = NO_DESCRIPTOR;

	return 0;
}

int read_cluster(unsigned int cluster_index, char *buffer)
{
	if (current_descriptor == NO_DESCRIPTOR)
    return -1;
 
	if (lseek(current_descriptor, cluster_index * CLUSTER_SIZE, SEEK_SET) < 0)
    return -1;

  if (read(current_descriptor, buffer, CLUSTER_SIZE) != CLUSTER_SIZE)
    return -1;

  return 0;
}

int write_cluster(unsigned int cluster_index, const char *buffer)
{
  if (current_descriptor == NO_DESCRIPTOR)
    return -1;

  if (lseek(current_descriptor, cluster_index * CLUSTER_SIZE, SEEK_SET) < 0)
    return -1;

  if (write(current_descriptor, buffer, CLUSTER_SIZE) != CLUSTER_SIZE)
    return -1;

  return 0;
}

