/**
 * @file   io.c
 * @date   02/11/2014
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include "io.h"

#define NO_DESCRIPTOR -1
int current_descriptor = NO_DESCRIPTOR;


int create_new_fs(const char *fs_image_name)
{
	int fd;
	int status;
	int image_size = CLUSTER_SIZE * NUMBER_OF_CLUSERS;
	char initial_data[CLUSTER_SIZE];

	// create image file
	fd = open(fs_image_name, O_RDWR|O_CREAT|O_TRUNC, S_IRWXU);
	if (fd > 0) {
		memset(initial_data, 0, image_size);
		// fill the file with zeros
		status = write(fd, initial_data, image_size);
		if (status == image_size) {
			status = 0;
		} else {
			status = -1;
		}
		//
		close(fd);
	} else {
		printf("couldn't create '%s'\n", fs_image_name);
		status = -1;
	}

	return status;
}

int load_fs(const char *fs_image_name)
{
	int fd;
	int status;

	if (current_descriptor == NO_DESCRIPTOR) {
		fd = open(fs_image_name, O_RDWR);
		if (fd > 0) {
			current_descriptor = fd;
			status = 0;
		} else {
			status = -1;
		}
	} else {
		status = -1;
	}

	return status;
}

int unload_fs()
{
	int status;

	if (current_descriptor != NO_DESCRIPTOR) {
		close(current_descriptor);
		current_descriptor = NO_DESCRIPTOR;
		status = 0;
	} else {
		status = -1;
	}

	return status;

}

int read_cluster(unsigned int cluster_index, char *buffer)
{
	int status;

	if (current_descriptor != NO_DESCRIPTOR) {
		status = lseek(current_descriptor, cluster_index * CLUSTER_SIZE, SEEK_SET);
		if (status >= 0) {
			status = read(current_descriptor, buffer, CLUSTER_SIZE);
			if (status == CLUSTER_SIZE) {
				status = 0;
			} else {
				status = -1;
			}
		} else {
			status = -1;
		}
	} else {
		status = -1;
	}

	return status;

}

int write_cluster(unsigned int cluster_index, const char *buffer)
{
	int status;

	if (current_descriptor != NO_DESCRIPTOR) {
		status = lseek(current_descriptor, cluster_index * CLUSTER_SIZE, SEEK_SET);
		if (status >= 0) {
			status = write(current_descriptor, buffer, CLUSTER_SIZE);
			if (status == CLUSTER_SIZE) {
				status = 0;
			} else {
				status = -1;
			}
		} else {
			status = -1;
		}
	} else {
		status = -1;
	}

	return status;
}



