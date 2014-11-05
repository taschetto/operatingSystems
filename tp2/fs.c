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

int cluster_index_from_path(const char *path[], int path_depth, unsigned int *dir_entries_index)
{
	int status = -1;
	unsigned int current_cluster_index;
	unsigned int next_cluster_index;
	struct dir_entry dir_entries[64];
	int i;

	current_cluster_index = next_cluster_index = ROOT_DIR_CLUSTER_INDEX;
	status = read_cluster(current_cluster_index, (char*)dir_entries);
	if (status == 0) {
		for (i = 0; i < path_depth; i++) {
			for (int j = 0; j < 64; j++) {
				if (dir_entries[j].first_block > 0) {
					if (strncmp(path[i], (const char*)dir_entries[j].filename, 16) == 0) {
						if (dir_entries[j].attributes == DIR_ENTRY_ATTR_DIRECTORY) {
							next_cluster_index = dir_entries[j].first_block;
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

int next_free_cluster(uint32_t *fat, unsigned int *next_free_cluster)
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

int read_fat(uint32_t *fat)
{
	int status;

	status = read_cluster(FAT_FIRST_CLUSTER_INDEX, (char*)&fat[0]);
	if (status == 0) {
		status = read_cluster(FAT_FIRST_CLUSTER_INDEX + 1, (char*)&fat[512]);
	}

	return status;
}

int write_fat(uint32_t *fat)
{
	int status;

	status = write_cluster(FAT_FIRST_CLUSTER_INDEX, (char*)&fat[0]);
	if (status == 0) {
		status = write_cluster(FAT_FIRST_CLUSTER_INDEX + 1, (char*)&fat[512]);
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
	struct dir_entry dir_entries[64];
	int first_free_entry;

	status = read_cluster(entries_cluster_index, (char*)dir_entries);
	if (status == 0) {
		if (find_entry((char*)entry->filename, dir_entries) == 0) {
			status = -1; // Duplicated entry
		} else {
			status = index_of_first_free_entry(dir_entries, &first_free_entry);
			if (status == 0) {
				dir_entries[first_free_entry].attributes = entry->attributes;
				strncpy((char*)dir_entries[first_free_entry].filename,(char*)entry->filename, 16);
				dir_entries[first_free_entry].first_block = entry->first_block;
				dir_entries[first_free_entry].size = entry->size;

				status = write_cluster(entries_cluster_index, (char*)dir_entries);
			}
		}
	}

	return status;
}

int create_dir_entry(const char *path[], int path_depth, char *new_entry_name, uint8_t attr)
{
	int status;
	unsigned int dir_entries_index;
	unsigned int free_cluster_index;
	struct dir_entry d_entry;
	struct dir_entry dir_entries[64];
	uint32_t fat[1024];

	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status == 0) {
		status = read_fat(fat);
		if (status == 0) {
			status = next_free_cluster(fat, &free_cluster_index);
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
						memset(dir_entries, 0, sizeof(dir_entries));
						status = write_cluster(free_cluster_index, (char*)dir_entries);
					}

					if (status == 0) {
						status = write_fat(fat);
					}
				}
			}
		}
	}

	return status;
}



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

int list_dir(const char *path[], int path_depth, struct dir_entry *entries, int *entries_size)
{
	int status;
	unsigned int dir_entries_index;
	struct dir_entry dir_entries[64];

	*entries_size = 0;
	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status == 0) {
		status = read_cluster(dir_entries_index, (char*)dir_entries);
		if (status == 0) {
			for (int i = 0; i < 64; i++) {
				if (dir_entries[i].first_block > 0) {
					memcpy(&entries[*entries_size], &dir_entries[i], sizeof(struct dir_entry));
					(*entries_size)++;
				}
			}
		}
	}

	return status;
}



