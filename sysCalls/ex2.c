#include <stdio.h>
#include <unistd.h>

int main(void) {
	int x;

	x = 0;
	fork();
	x = 1;
	printf("I am process %ld, my parent is %ld and my x is %d\n", (long)getpid(), (long)getppid(), x);
	return 0;
}

