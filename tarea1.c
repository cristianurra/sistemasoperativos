#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

int switcha(int columna,int size);
void switchb(int columna,int size);

#define MEMORIA1 "/memoria1"
#define MEMORIA2 "/memoria2"


int n=256;
int sizeint=5;

int main(void)
 {
	 pid_t pidC;
	 
	 printf("**proceso PID =%d comienza\n",getpid());
	 pidC = fork();
	 printf("**proceso PID =%d, pidC = %d ejecutrandosen\n",getpid(),pidC);
	 
	 if(pidC>0) //Esto se ejecuta solo en el proceso padre
		{	
		int SMOBJ_SIZE=n*sizeint+20;	
			
		int fd1;
		fd1 = shm_open(MEMORIA1 , O_CREAT | O_RDWR, 00600);
		int fd2;
		fd2 = shm_open(MEMORIA2 , O_CREAT | O_RDWR, 00600);	
		
		
		if(fd1 == -1){
			printf("hubo un error al crear memoria compartida 1\n");
			exit(1);
		}
		if(fd2 == -1){
			printf("hubo un error al crear memoria compartida 2\n");
			exit(1);
		}			

		if(-1 == ftruncate(fd1, SMOBJ_SIZE)){
			printf("la memoria compartida no pudo ser rezsizeada \n");
			}			
		if(-1 == ftruncate(fd2, SMOBJ_SIZE)){
			printf("la memoria compartida no pudo ser rezsizeada \n");
			}				
			
			
		
		int k1=0;
		srand(time(NULL)); 					//Una semilla que varía, para mas "aleatoriedad"
		while(k1<n)
		{
			int j = rand()%n;
			printf("Proceso padre %d ha generado: %d\n",pidC, j);
			int fda;
			
			char bufa[sizeint];
			sprintf(bufa,"%d\n",j);
			
			//char bufa[]="algo se escribio%d\n";
			char *ptra;
			fda = shm_open(MEMORIA1 , O_RDWR, 0);
			
			if(fda == -1){
			printf("hubo un error :c \n");
			exit(1);
			}
			
			ptra = mmap(0, sizeint, PROT_WRITE, MAP_SHARED,fda,0);
			printf("%p\n",ptra);
			if (ptra==MAP_FAILED){
				printf("Error mapeando \n");
				exit(1);
			}
			ptra=ptra+k1*sizeint;
			memcpy(ptra,bufa,sizeint);
			close(fda);	
			ptra=ptra+k1*sizeint;
			k1=k1+1;
		}
		
		int k2=0;
		srand(time(NULL)+21); 					//Una semilla que varía, para mas "aleatoriedad"
		
		
		while(k2<n)
		{
			int j = rand()%n;
			printf("Proceso padre %d ha generado: %d\n",pidC, j);
			int fdb;
			
			char bufb[sizeint];
			sprintf(bufb,"%d\n",j);
			
			//char bufa[]="algo se escribio%d\n";   [2,2,5,\,n]
			char *ptrb;
			fdb = shm_open(MEMORIA2 , O_RDWR, 0);
			
			if(fdb == -1){
			printf("hubo un error :c \n");
			exit(1);
			}
			
			ptrb = mmap(0, sizeint, PROT_WRITE, MAP_SHARED,fdb,0);
			printf("%p\n",ptrb);
			if (ptrb==MAP_FAILED){
				printf("Error mapeando \n");
				exit(1);
			}
			ptrb=ptrb+k2*sizeint;
			memcpy(ptrb,bufb,sizeint);
			close(fdb);	
			ptrb=ptrb+k2*sizeint;
			k2=k2+1;
		}	
				
	 }
	 
	  
	 else if(pidC==0)//Esto se ejecuta solo en el proceso hijo
{
	
	int f=0;
	while(f<1000)
	{	int i=0;

		while(i<n)
		{
			switcha(i,sizeint);
			switchb(i,sizeint);
			i=i+1;
		}
	f=f+1;
	}

	return(0);
}
	 	 
	 
	 else
	 {
		 printf("El proceso hijo no pudo ser creado\n");
	 }
	 
	 printf("BAndera\n");
	wait(3);
	 
	 
	 
	 return 0; 
 }
 
 
 
 
 
 
int switcha(int columna,int size)	//compara dos valores consecutivos
{
		int fd;
		char *ptr1;
		char *ptr2;
		char bufaa[size];
		char bufbb[size];
		struct stat shmobj_st;	
		fd = shm_open(MEMORIA1, O_RDWR,0);

		
		if(fd == -1){
			printf("Error fd %s\n", strerror(errno));
			exit(1);
			}
			
		if(fstat(fd, &shmobj_st)==1)
			{
			printf("error fstat \n");
			exit(1);
			}
		
		ptr1=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0)+columna*size;
		ptr2=ptr1+size;	
		if(ptr1==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		
		char *a=ptr1;
		char *b=ptr2;
		
		
		//printf("%s\n",a);		
		//printf("%s\n",b);
		
		int aa =  atoi(a);
		int bb = atoi(b);
		
		if(bb<aa)
		{
			
			sprintf(bufaa,"%d\n",aa);
			sprintf(bufbb,"%d\n",bb);
			//printf("Se cambia\n");			
			ptr1=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+columna*size;
			ptr2=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+columna*size+size;

			memcpy(ptr1,bufbb,sizeof(bufbb));
			memcpy(ptr2,bufaa,sizeof(bufaa));
			close(fd);
			return(1);
		}
		
		
		close(fd);
}
 
 void switchb(int columna,int size)	//compara dos valores consecutivos
{
		int fd;
		char *ptr1;
		char *ptr2;
		char bufaa[size];
		char bufbb[size];
		struct stat shmobj_st;	
		fd = shm_open(MEMORIA2, O_RDWR,0);

		
		if(fd == -1){
			printf("Error fd %s\n", strerror(errno));
			exit(1);
			}
			
		if(fstat(fd, &shmobj_st)==1)
			{
			printf("error fstat \n");
			exit(1);
			}
		
		ptr1=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0)+columna*size;
		ptr2=ptr1+size;	
		if(ptr1==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		
		char *a=ptr1;
		char *b=ptr2;
		
		
		//printf("%s\n",a);		
		//printf("%s\n",b);
		
		int aa =  atoi(a);
		int bb = atoi(b);
		
		if(bb<aa)
		{
			
			sprintf(bufaa,"%d\n",aa);
			sprintf(bufbb,"%d\n",bb);
			//printf("Se cambia\n");			
			ptr1=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+columna*size;
			ptr2=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+columna*size+size;

			memcpy(ptr1,bufbb,size);
			memcpy(ptr2,bufaa,size);

			
		}
		
		
		close(fd);
}
