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

int main()
{
  shell();
	return 0;
}

