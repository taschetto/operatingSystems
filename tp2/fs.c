/**
 * @file   fs.c
 * @date   02/11/2014
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "io.h"
#include "fs.h"


#define FAT_FREE_CLUSTER 0
#define FAT_EOF -1

#define FAT_FIRST_CLUSTER_INDEX 1
#define ROOT_DIR_CLUSTER_INDEX  3

#define DIR_ENTRY_ATTR_FILE      0
#define DIR_ENTRY_ATTR_DIRECTORY 1
#define DIR_ENTRY_MAX_NAME_SIZE  16

#pragma pack(push, 1)
struct dir_entry {
	uint8_t filename[DIR_ENTRY_MAX_NAME_SIZE];
	uint8_t attributes;
	uint8_t reserved[7];
	uint32_t first_block;
	uint32_t size;
};
#pragma pack(pop)



uint32_t fat_cluster[512];


int format()
{
	char cluster[CLUSTER_SIZE];
	int status;

	// intialize boot block
	memset(cluster, 0xbb, CLUSTER_SIZE);
	status = write_cluster(0, cluster);
	if (status == 0) {
		// intialize FAT
		memset(cluster, FAT_FREE_CLUSTER, CLUSTER_SIZE);
		status = write_cluster(1, cluster);
		if (status == 0) {
			status = write_cluster(2, cluster);
			if (status == 0) {
				// initialize root dir entries
				memset(cluster, 0, CLUSTER_SIZE);
				status = write_cluster(3, cluster);
			}
		}
	}

	return status;
}





