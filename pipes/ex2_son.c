#define LEN 13
#include <stdio.h>
#include <unistd.h>	/* Son.c */
#include <stdlib.h>

int main(int argc, char *argv[]) {
	/* argv[1] - descritor de escrita
	argv[2] - posicao do filho */
	char texto[LEN] = {' ',':',' ','B','o','m',' ','d','i','a','!','\0'};
	texto[0] = 'A' + atoi(argv[2])-1;
	write(atoi(argv[1]), texto, LEN);
  printf("oi");
	_exit(0);
}

