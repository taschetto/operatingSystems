#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1
#define STDOUT 1

#define MAX 128
#define LEN 9

int main(){
	int n, fd[2];
	pid_t pid;

	char line[MAX];
	char msg[LEN]={'B','o','m',' ','d','i','a','\n','\0'};

	if (pipe(fd) < 0){
		fprintf(stderr,"Erro no tubo\n");
		_exit(1);
	}

	if ((pid = fork()) < 0){
		fprintf(stderr,"Erro no fork\n");
		_exit(1);
	}

	if (pid > 0) /* processo pai */
  {
		close(fd[WRITE]);
		n = read(fd[READ], line, MAX);
		write(STDOUT, &line[0], n);
		close(fd[READ]);
		kill(pid, SIGKILL); /* elimina processo descendente */
		_exit(0);
	}

	if (pid == 0) /* processo filho */
  {
		close(fd[READ]);
		write(fd[WRITE], &msg[0], LEN);
		close(fd[WRITE]);
		pause();
	}

  return 0;
}

