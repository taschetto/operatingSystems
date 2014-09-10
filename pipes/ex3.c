#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(void){
	int pfd[2];

	pipe(pfd);
	if (fork() == 0)
  {
		close(pfd[1]);
		dup2(pfd[0], STDIN_FILENO);
		close(pfd[0]);
		execlp("wc", "wc", NULL);
	}
  else
  {
		close(pfd[0]);
		dup2(pfd[1], STDOUT_FILENO);
		close(pfd[1]);
		execlp("ls", "ls", "-hal", NULL);
	}
	exit(0);
}

