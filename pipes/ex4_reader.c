#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>

int readChar(int fd, char *buf){
	int n;

	do {
		n = read(fd, buf, 1);
   	} while (n>0 && *buf++!='\0');

	return n>0;
}

int main(){
	int fd;
	char str[100];

	mkfifo("pipezinho", 0666);
	fd = open("pipezinho", O_RDONLY);
	if (fd < 0){
		printf("Erro na abertura da fila\n");
		exit(1);
	}
	while (readChar(fd, str))
		printf("%s",str);
	close(fd);
  return 0;
}

