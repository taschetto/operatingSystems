/**
 * @file   fs.c
 * @date   02/11/2014
 */

#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "fs.h"


#define FAT_FREE_CLUSTER 0
#define FAT_EOF -1

#define FAT_FIRST_CLUSTER_INDEX 1
#define ROOT_DIR_CLUSTER_INDEX  3

// Allowed fs data structures - real declaration in 'main.c'
extern uint32_t fat[1024];
extern uint8_t data_block[CLUSTER_SIZE];
extern struct dir_entry dir[64];

int cluster_index_from_path(const char *path[], int path_depth, unsigned int *dir_entries_index)
{
	int status = -1;
	unsigned int current_cluster_index;
	unsigned int next_cluster_index;
	int i;

	current_cluster_index = next_cluster_index = ROOT_DIR_CLUSTER_INDEX;
	status = read_cluster(current_cluster_index, (uint8_t*)dir);
	if (status == 0) {
		for (i = 0; i < path_depth; i++) {
			for (int j = 0; j < 64; j++) {
				if (dir[j].first_block > 0) {
					if (strncmp(path[i], (const char*)dir[j].filename, 16) == 0) {
						if (dir[j].attributes == DIR_ENTRY_ATTR_DIRECTORY) {
							next_cluster_index = dir[j].first_block;
						}
					}
				}
			}
			if (next_cluster_index != current_cluster_index) {
				current_cluster_index = next_cluster_index;
			} else {
				break;
			}
		}
		if (i == path_depth) {
			*dir_entries_index = current_cluster_index;
			status = 0;
		} else {
			status = -1;
		}
	}

	return status;
}

int next_free_cluster(unsigned int *next_free_cluster)
{
	for (unsigned int i = 4; i < 1024; i++) {
		if (fat[i] == FAT_FREE_CLUSTER) {
			*next_free_cluster = i;
			return 0;
		}
	}

	return -1;
}

int index_of_first_free_entry(struct dir_entry *dir_entries, int *index)
{
	for (int i = 0; i < 64; i++) {
		if (dir_entries[i].first_block == 0) {
			*index = i;
			return 0;
		}
	}

	return -1;
}

int read_fat()
{
	int status;

	status = read_cluster(FAT_FIRST_CLUSTER_INDEX, (uint8_t*)&fat[0]);
	if (status == 0) {
		status = read_cluster(FAT_FIRST_CLUSTER_INDEX + 1, (uint8_t*)&fat[512]);
	}

	return status;
}

int write_fat()
{
	int status;

	status = write_cluster(FAT_FIRST_CLUSTER_INDEX, (uint8_t*)&fat[0]);
	if (status == 0) {
		status = write_cluster(FAT_FIRST_CLUSTER_INDEX + 1, (uint8_t*)&fat[512]);
	}

	return status;
}

int find_entry(char *entry_name, struct dir_entry *entries)
{

	for (int i = 0; i < 64; i++) {
		if (strncmp((const char*)entries[i].filename, entry_name, 16) == 0) {
			return 0;
		}
	}

	return -1;
}

int add_dir_entry(struct dir_entry *entry, unsigned int entries_cluster_index)
{
	int status;
	int first_free_entry;

	status = read_cluster(entries_cluster_index, (uint8_t*)dir);
	if (status == 0) {
		if (find_entry((char*)entry->filename, dir) == 0) {
			status = -1; // Duplicated entry
		} else {
			status = index_of_first_free_entry(dir, &first_free_entry);
			if (status == 0) {
				dir[first_free_entry].attributes = entry->attributes;
				strncpy((char*)dir[first_free_entry].filename,(char*)entry->filename, 16);
				dir[first_free_entry].first_block = entry->first_block;
				dir[first_free_entry].size = entry->size;

				status = write_cluster(entries_cluster_index, (uint8_t*)dir);
			}
		}
	}

	return status;
}

int create_file_or_dir(const char *path[], int path_depth, char *new_entry_name, uint8_t attr)
{
	int status;
	unsigned int dir_entries_index;
	unsigned int free_cluster_index;
	struct dir_entry d_entry;

	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status == 0) {
		status = read_fat();
		if (status == 0) {
			status = next_free_cluster(&free_cluster_index);
			if (status == 0) {
				d_entry.attributes = attr;
				strncpy((char*)d_entry.filename, (char*)new_entry_name, 16);
				d_entry.first_block = free_cluster_index;
				if (attr == DIR_ENTRY_ATTR_DIRECTORY) {
					d_entry.size = CLUSTER_SIZE;
				} else {
					d_entry.size = 0;
				}
				status = add_dir_entry(&d_entry, dir_entries_index);
				if (status == 0) {
					fat[free_cluster_index] = FAT_EOF;
					if (attr == DIR_ENTRY_ATTR_DIRECTORY) {
						memset(dir, 0, sizeof(dir));
						status = write_cluster(free_cluster_index, (uint8_t*)dir);
					}

					if (status == 0) {
						status = write_fat();
					}
				}
			}
		}
	}

	return status;
}



int format()
{
	int status;

	// intialize boot block
	memset(data_block, 0xbb, CLUSTER_SIZE);
	status = write_cluster(0, data_block);
	if (status == 0) {
		// intialize FAT
		memset(data_block, FAT_FREE_CLUSTER, CLUSTER_SIZE);
		status = write_cluster(1, data_block);
		if (status == 0) {
			status = write_cluster(2, data_block);
			if (status == 0) {
				// initialize root dir entries
				memset(data_block, 0, CLUSTER_SIZE);
				status = write_cluster(3, data_block);
			}
		}
	}

	return status;
}

int list_dir(const char *path[], int path_depth)
{
	int status;
	unsigned int dir_entries_index;

	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status == 0) {
		status = read_cluster(dir_entries_index, (uint8_t*)dir);
	}

	return status;
}



