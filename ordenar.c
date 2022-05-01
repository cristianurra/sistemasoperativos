#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SMOBJ_NAME "\memoria1"

void switche(int columna);

int main(void)
{
	switche(4);
	return(0);
}






void switche(int columna)	//compara dos valores consecutivos
{
		int fd;
		char *ptr1;
		char *ptr2;
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
		
		ptr1=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0)+columna*5;
		ptr2=ptr1+5;	
		if(ptr1==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		printf("%s\n",ptr1);		
		printf("%s\n",ptr2);
		close(fd);
		
		return(0);	
	
}
