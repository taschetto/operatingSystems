#include <stdio.h>
#include <string.h>
#include <sys/file.h>

int main(){
	int fd, i;
	char msg[100];

	do {
		fd = open("pipezinho", O_WRONLY);
		if (fd == -1) sleep(1);
	} while (fd == -1);

	for(i = 1; i <= 3; i++){
		sprintf(msg, "Hello [%d] from process %d\n", i, getpid());
		write(fd, msg, strlen(msg) + 1);
		sleep(3);
	}
	close(fd);
  return 0;
}

