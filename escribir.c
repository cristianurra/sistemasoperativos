#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SMOBJ_NAME "/mimemoria"

int main(void) {
	int fd;
	char buf[]="proceso escrito\n";
	char *ptr;
	fd = shm_open(SMOBJ_NAME , O_RDWR, 0);
	
	if(fd == -1){
	printf("hubo un error :c \n");
	exit(1);
	}
	
	ptr = mmap(0, sizeof(buf), PROT_WRITE, MAP_SHARED,fd,0);
	if (ptr==MAP_FAILED){
		printf("Error mapeando \n");
		exit(1);
	}
	memcpy(ptr,buf,sizeof(buf));
	close(fd);	
	return(0);
}


