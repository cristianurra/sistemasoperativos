#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>

#define SMOBJ_NAME "/mimemoria"
#define SMOBJ_SIZE 200
int var=22;

int main(void) {
	int fd;
	fd = shm_open(SMOBJ_NAME , O_CREAT | O_RDWR, 00600);
	
	if(fd == -1){
		printf("hubo un error :c \n");
				exit(1);
	}

	if(-1 == ftruncate(fd, SMOBJ_SIZE)){
		printf("la memoria compartida no pudo ser rezsizeada \n");
	}

	return(0);
}

