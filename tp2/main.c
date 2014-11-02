
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "io.h"


void io_test()
{
	char *test_cluster;
	int status;

	test_cluster = malloc(CLUSTER_SIZE);
	if (test_cluster != NULL) {
		status = create_new_fs("fat.part");
		if (status == 0) {
			printf("create_new_fs: OK\n");
		} else {
			printf("create_new_fs: ERR\n");
		}

		status = load_fs("fat.part");
		if (status == 0) {
			printf("load_fs: OK\n");
		} else {
			printf("load_fs: ERR\n");
		}

		memset(test_cluster, 0x55, CLUSTER_SIZE);
		status = write_cluster(1, test_cluster);
		if (status == 0) {
			printf("load_fs: OK\n");
		} else {
			printf("load_fs: ERR\n");
		}

		memset(test_cluster, 0x00, CLUSTER_SIZE);
		status = read_cluster(1, test_cluster);
		if (status == 0 && test_cluster[2] == 0x55) {
			printf("load_fs: OK\n");
		} else {
			printf("load_fs: ERR\n");
		}

		status = unload_fs();
		if (status == 0) {
			printf("unload_fs: OK\n");
		} else {
			printf("unload_fs: ERR\n");
		}

	} else {
		printf("couldn't allocate memory for 'test_cluster'\n");
	}
}

int main()
{
	io_test();
}


