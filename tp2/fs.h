/**
 * @file   fs.h
 * @date   02/11/2014
 */


#ifndef FS_H
#define FS_H

#include <stdint.h>

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
int create_dir_entry(const char *path[], int path_depth, char *new_entry_name, uint8_t attr);
int list_dir(const char *path[], int path_depth, struct dir_entry *entries, int *entries_size);

#endif // FS_H
