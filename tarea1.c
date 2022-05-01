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



#define MEMORIA1 "/memoria1"
#define MEMORIA2 "/memoria2"


void swap(int* xp, int* yp);
void bubbleSort(int arr[], int n);
void printArray(int arr[], int size);

int n=220;


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
			
			char bufa[5];
			sprintf(bufa,"%d\n",j);
			
			//char bufa[]="algo se escribio%d\n";
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
		srand(time(NULL)-21); 					//Una semilla que varía, para mas "aleatoriedad"
		
		
		while(k2<n)
		{
			int j = rand()%n;
			printf("Proceso padre %d ha generado: %d\n",pidC, j);
			int fdb;
			
			char bufb[5];
			sprintf(bufb,"%d\n",j);
			
			//char bufa[]="algo se escribio%d\n";   [2,2,5,\,n]
			char *ptrb;
			fdb = shm_open(MEMORIA2 , O_RDWR, 0);
			
			if(fdb == -1){
			printf("hubo un error :c \n");
			exit(1);
			}
			
			ptrb = mmap(0, sizeof(bufb), PROT_WRITE, MAP_SHARED,fdb,0);
			printf("%p\n",ptrb);
			if (ptrb==MAP_FAILED){
				printf("Error mapeando \n");
				exit(1);
			}
			ptrb=ptrb+k2*sizeof(bufb);
			memcpy(ptrb,bufb,sizeof(bufb));
			close(fdb);	
			ptrb=ptrb+k2*sizeof(bufb);
			k2=k2+1;
		}	
				
	 }
	 
	 
	 else if(pidC == 0)//Esto se ejecuta solo en el proceso hijo
	 { 
		burbujas();
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
 
 
 
 
 
 
void burbujas()
{	
	for(k=1;k<1000;k++){		//calcular valor optimo de k
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
		ptr=ptr+k*sizeof(bufb);
		
		if(ptr==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		printf("%s\n",ptr);
		close(fd);
		
		return(0);	
			
		
		
		
		
		
	} 
	
	
	
	
	
}
 
 
 
 
 
 
 
 
 
 
 
 
 
 
void swap(int* xp, int* yp)
{
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}
 
 
void bubbleSort(int arr[], int n)
{
    int i, j;
    for (i = 0; i < n - 1; i++)
 
        // Last i elements are already in place
        for (j = 0; j < n - i - 1; j++)
            if (arr[j] > arr[j + 1])
                swap(&arr[j], &arr[j + 1]);
}
 
void printArray(int arr[], int size)
{
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}
 
 
 
 
