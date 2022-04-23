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


#define MEMORIA1 "/memoria1"
#define MEMORIA2 "/memoria2"


int n=22;


int main(void)
 {
	 pid_t pidC;
	 
	 printf("**proceso PID =%d comienza\n",getpid());
	 pidC = fork();
	 printf("**proceso PID =%d, pidC = %d ejecutrandosen\n",getpid(),pidC);
	 
	 if(pidC>0) //Esto se ejecuta solo en el proceso padre
		{	
		int SMOBJ_SIZE=900;	
			
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
			char bufa[]="algo se escribio\n";
			char *ptra;
			fda = shm_open(MEMORIA1 , O_RDWR, 0);
			
			if(fda == -1){
			printf("hubo un error :c \n");
			exit(1);
			}
			
			ptra = mmap(0, sizeof(bufa), PROT_WRITE, MAP_SHARED,fda,0);
			printf("%p\n",ptra);
			if (ptra==MAP_FAILED){
				printf("Error mapeando \n");
				exit(1);
			}
			ptra=ptra+k1*sizeof(bufa);
			memcpy(ptra,bufa,sizeof(bufa));
			close(fda);	
			ptra=ptra+k1*sizeof(bufa);
			k1=k1+1;
		}
		
		int k2=0;
		srand(time(NULL)); 					//Una semilla que varía, para mas "aleatoriedad"
		while(k2<n)
		{
			int j = rand()%n;
			printf("Proceso padreaaaa %d ha generado: %d\n",pidC, j);
			int fdb;
			char bufb[]="proceso escrito\n";
			char *ptrb;
			//printf("%d\n",ptrb);
			fdb = shm_open(MEMORIA2 , O_RDWR, 0);
			
			if(fdb == -1){
			printf("hubo un error :c \n");
			exit(1);
			}
			
			ptrb = mmap(0, sizeof(bufb), PROT_WRITE, MAP_SHARED,fdb,0);
			if (ptrb==MAP_FAILED){
				printf("Error mapeando \n");
				exit(1);
			}
			memcpy(ptrb,bufb,sizeof(bufb));
			close(fdb);	
			k2=k2+1;
		}		
		
	
		
	 }
	 
	
 
	 
	 else if(pidC == 0)//Esto se ejecuta solo en el proceso hijo
	 { 
		 
	 }
	 
	 else
	 {
		 printf("El proceso hijo no pudo ser creado\n");
	 }
	 while(1)
	 {

	 }
	 
	 return 0;
	 
	 
 }
