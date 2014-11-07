/**
 * @file   fs.c
 * @date   02/11/2014
 */

#include <stdlib.h>
#include <string.h>
#include "io.h"
#include "fs.h"
#include "helpers.h"


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

int find_entry(char *entry_name, struct dir_entry *entries, int *index)
{

	for (int i = 0; i < 64; i++) {
		if (entries[i].first_block > 0) {
//			printf("Testing entry: %s\n", entries[i].filename);
			if (strncmp((const char*)entries[i].filename, entry_name, 16) == 0) {
				if (index) *index = i;
				return 0;
			}
		}
	}

	return -1;
}

int count_active_entries(struct dir_entry *entries)
{
	int counter = 0;

	for (int i = 0; i < 64; i++) {
		if (entries[i].first_block > 0) {
			counter++;
		}
	}

	return counter;
}

int add_dir_entry(struct dir_entry *entry, unsigned int entries_cluster_index)
{
	int status;
	int first_free_entry;

	status = read_cluster(entries_cluster_index, (uint8_t*)dir);
	if (status == 0) {
		if (find_entry((char*)entry->filename, dir, NULL) == 0) {
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

int alloc_clusters (int last_cluster, int num_of_clusters)
{
	int status;
	unsigned int free_cluster;
	int to_be_allocated = num_of_clusters;

	while (to_be_allocated > 0) {
		status = next_free_cluster(&free_cluster);
		if (status != 0) break;
		fat[last_cluster] = free_cluster;
		fat[free_cluster] = FAT_EOF;
		last_cluster = free_cluster;
		to_be_allocated--;
	}

	return num_of_clusters - to_be_allocated;
}

void get_last_cluster(int first_cluster, int *last_cluster)
{
	int next_cluster = first_cluster;

	while (fat[next_cluster] != FAT_EOF) {
		next_cluster = fat[next_cluster];
	}

	*last_cluster = next_cluster;
}

int update_cluster(int cluster, int start_index, uint8_t *data, int data_size)
{
	int status;

	status = read_cluster(cluster, data_block);
	if (status == 0) {
		memcpy(&data_block[start_index], data, data_size);
		status = write_cluster(cluster, data_block);
	}

	return status;
}

int write_data_to_clusters(int first_cluster, int start_index, uint8_t *data, int data_size)
{
	int status;
	int free_space;
	int size;
	int full_clusters;
	int next_cluster;

	TRACE();
	free_space = CLUSTER_SIZE - start_index;
//	printf("free space: %d\n", free_space);
	size = free_space > data_size ? data_size : free_space;
	status = update_cluster(first_cluster, start_index, data, size);
	data_size -= size;
	data += size;

	full_clusters = data_size / CLUSTER_SIZE;

	next_cluster = fat[first_cluster];
	while (full_clusters > 0) {
		TRACE();
		if (next_cluster == FAT_EOF) {
			TRACE();
			data_size = 0;
			break;
		}
		status = write_cluster(next_cluster, data);
		data_size -= CLUSTER_SIZE;
		data += CLUSTER_SIZE;
		full_clusters--;
		next_cluster = fat[next_cluster];
	}

	if (next_cluster != FAT_EOF) {
		TRACE();
		if (data_size > 0) {
			TRACE();
			memcpy(data_block, data, data_size);
			status = write_cluster(next_cluster, data_block);
		}
	}


	return status;
}

int write_to_file(const char *path[], int path_depth, char *file_name, uint8_t *data, unsigned int data_size)
{
	int status;
	unsigned int dir_entries_index;
	int entry_index;
	int full_clusters;
	int bytes_in_last_cluster;
	int total_clusters;
	int new_total_clusters;
	int additional_clusters;
	int last_cluster;
	int allocated_clusters;

	TRACE();
	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status == 0) {
		TRACE();
		status = read_cluster(dir_entries_index, (uint8_t *)dir);
		if (status == 0) {
			status = find_entry(file_name, dir, &entry_index);
			if (status == 0) {
				TRACE();
				// This command only works with regular files!
				if (dir[entry_index].attributes != DIR_ENTRY_ATTR_FILE) {
					return -1;
				}
				full_clusters = dir[entry_index].size / CLUSTER_SIZE;
				bytes_in_last_cluster = dir[entry_index].size % CLUSTER_SIZE;
				total_clusters = full_clusters + (bytes_in_last_cluster > 0 ? 1 : 0);
				// Special case, when the file is new and empty
				if (total_clusters == 0 && bytes_in_last_cluster == 0) {
					total_clusters = 1;
				}

				new_total_clusters = ((dir[entry_index].size + data_size) / CLUSTER_SIZE) +
						(((dir[entry_index].size + data_size) % CLUSTER_SIZE) > 0 ? 1 : 0);
				additional_clusters = new_total_clusters - total_clusters;
				get_last_cluster(dir[entry_index].first_block, &last_cluster);

//				printf("###############################\n");
//				printf("# full clusters: %d\n",full_clusters);
//				printf("# bytes in last cluster: %d\n",bytes_in_last_cluster);
//				printf("# total clusters: %d\n",total_clusters);
//				printf("# new total clusters: %d\n",new_total_clusters);
//				printf("# additional clusters: %d\n",additional_clusters);
//				printf("# last cluster: %d\n",last_cluster);
//				printf("###############################\n");

				if (additional_clusters > 0) {
					TRACE();
					allocated_clusters = alloc_clusters(last_cluster, additional_clusters);
				}
				status = write_data_to_clusters(last_cluster, bytes_in_last_cluster, data, data_size);
				write_fat();
				dir[entry_index].size = dir[entry_index].size + data_size;
				write_cluster(dir_entries_index, (uint8_t*)dir);
				if (allocated_clusters != additional_clusters) {
					TRACE();
					status = -1;
				}
			}
		}
	}

	return status;
}

int rm_dir(const char *path[], int path_depth, char *dir_name)
{
	unsigned int dir_entries_index;
	int entry_index;
	int status;

	// get the parent directory cluster index
	status = cluster_index_from_path(path, path_depth, &dir_entries_index);
	if (status != 0) {
		return -1;
	}
	// Read the parent directory
	status = read_cluster(dir_entries_index, (uint8_t *)dir);
	if (status != 0) {
		return -1;
	}
	// Find the entry that will be deleted
	status = find_entry(dir_name, dir, &entry_index);
	if (status != 0) {
		return -1;
	}
	// Test.This commando only works on directories.
	if (dir[entry_index].attributes != DIR_ENTRY_ATTR_DIRECTORY) {
		return -1;
	}
	// Read the directory that will be deleted
	status = read_cluster(dir[entry_index].first_block, (uint8_t*)dir);
	if (status != 0) {
		return -1;
	}
	// Check if the directory is empty
	if (count_active_entries(dir) > 0) {
		return -1;
	}
	// Read the parent directory again
	status = read_cluster(dir_entries_index, (uint8_t *)dir);
	if (status != 0) {
		return -1;
	}
	// Read FAT
	read_fat();
	// Free the entry related to the deleted directory
	fat[dir[entry_index].first_block] = FAT_FREE_CLUSTER;
	// Write FAT (really?)
	write_fat();
	// We are using first_block==0 to mark unused clusters
	dir[entry_index].first_block = 0;
	// Write the updated parent directory entries
	status = write_cluster(dir_entries_index, (uint8_t*)dir);
	if (status != 0) {
		return -1;
	}

	return 0;
}
