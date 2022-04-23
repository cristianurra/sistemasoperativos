#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SMOBJ_NAME "mimemoria"

int main(void)
{
	int fd;
	char *ptr;
	struct stat shmobj_st;
	
	fd = shm_open(SMOBJ_NAME, O_RDONLY,0);
	
	if(fd == -1){
		printf("Error fd %s\n", strerror(errno));
		exit(1);
	}
	if(fstat(fd, &shmobj_st)==1)
	{
		printf("error fstat \n");
		exit(1);
	}
	
	ptr=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0);	
	
	if(ptr==MAP_FAILED)
	{
		printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
		exit(1);
	}
	printf("%s\n",ptr);
	close(fd);
	
	return(0);
	
}
