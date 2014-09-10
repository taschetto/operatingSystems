#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

#define READ 0
#define WRITE 1
#define LEN 12

int main(){
	int fd[2]; 		/* tubo de leitura do processo principal */
	pid_t pid, pidA, pidB;
	char buf[LEN];
	int i, n, cstat;

	if (pipe(fd) < 0){
		fprintf(stderr,"Erro no tubo\n");
		_exit(1);
	}

	if ((pid = fork()) < 0){
		fprintf(stderr,"Erro no fork\n");
		_exit(1);
	}
   	if (pid==0) { 	/* primeiro processo descendente */
		char channel[20];
		close(fd[READ]);
		sprintf( channel,"%d",fd[WRITE] );
		execl("./ex2_son", "Son", channel, "1", NULL);
	}
	pidA = pid;

	if ((pid = fork()) < 0){
		fprintf(stderr,"Erro no fork\n");
		_exit(1);
	}
	if ( pid==0 ) { /* segundo processo descendente */
		char channel[20];
		close(fd[READ] );
		sprintf( channel,"%d",fd[WRITE] );
		execl("./ex2_son", "Son", channel, "2", NULL);
	}
	
  pidB = pid;
	close( fd[1] );

	n = read(fd[READ], buf, LEN);
	for(i = 0; i < LEN ; i++)
		printf("%c", buf[i]);
	printf( "\n" );

	n = read( fd[READ],buf,LEN );
	for(i = 0; i < LEN; i++)
		printf("%c", buf[i]);
	printf( "\n" );

	waitpid(pidA, &cstat, 0);
  waitpid(pidB, &cstat, 0);
	
  _exit(0);
}

