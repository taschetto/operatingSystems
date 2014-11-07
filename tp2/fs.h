/**
 * @file   fs.h
 * @date   02/11/2014
 */


#ifndef FS_H
#define FS_H

#include <stdint.h>
#include "io.h"

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

int format();
int create_file_or_dir(const char *path[], int path_depth, char *new_entry_name, uint8_t attr);
int list_dir(const char *path[], int path_depth);
int write_to_file(const char *path[], int path_depth, char *file_name, uint8_t *data, unsigned int data_size);
int rm_dir(const char *path[], int path_depth, char *dir_name);
int rm_file(const char *path[], int path_depth, char *file_name);

#endif // FS_H
