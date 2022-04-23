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
		 
		 
		 
		int SMOBJ_SIZE=n*12;
		int k=0;
		srand(time(NULL)); 					//Una semilla que varía, para mas "aleatoriedad"
		while(k<n)
		{
			int j = rand()%n;
			printf("Proceso padre %d ha generado: %d\n",pidC, j);
			/*sleep(0.2);*/
			k=k+1;
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
