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

void switcha(int columna,int size);
void leer_posix(int columna,int size);
void leer_posix2(int columna,int size);
int particion (int izquierda, int derecha, int size);
void quicksort(int izquierda,int derecha,int size);

#define MEMORIA1 "/urra_memoria1"			//Se crearan dos memorias compartidas posix
#define MEMORIA2 "/urra_memoria2"


int n;		//corresponde al n solicitado en la tarea

int sizeint=8;		//corresponde al espacio asignado a cada numero. Notese eque en la posix cada dato es iingresado como cadena de caracteres terminada en "\" y "n".

int main(void)
 {
	 printf("Ingresar un valor de n: ");
	 scanf("%d", &n);								//aca se solicia al usuario el valor de n
	 pid_t pidC;
	 
	 printf("**proceso PID =%d comienza\n",getpid());
	 pidC = fork();									//se hace un fork para generar el proceso hijo
	 printf("**proceso PID =%d, pidC = %d ejecutandosen\n",getpid(),pidC);
	 
	 if(pidC>0) //Esto se ejecuta solo en el proceso padre
		{	
		int SMOBJ_SIZE=n*sizeint+20;	//se define el tamaño de cada memoria posix. Se considera el tamaño de cada dato multiplicado por el total de datos. 
			
		int fd1;						// se crea la variable file descriptor
		fd1 = shm_open(MEMORIA1 , O_CREAT | O_RDWR, 00600);	//Se define cada file descriptor (uno por cada memoria compartida)
		int fd2;
		fd2 = shm_open(MEMORIA2 , O_CREAT | O_RDWR, 00600);	
		
		
		if(fd1 == -1){				//Mensajes en caso de error
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
			
			
		
		printf("\nConjunto 1 desordenado\n ");
		int k1=0;							//Se inicia
		srand(time(NULL)); 					//Una semilla que varía, para mas "aleatoriedad"
		while(k1<n)
		{
			int j = rand()%n;
			printf("%d, ", j);
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
			//printf("%p\n",ptra);
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
		printf("\nConjunto 2 desordenado\n");
		int k2=0;
		srand(time(NULL)+21); 					//Una semilla que varía, para mas "aleatoriedad"
		
		
		while(k2<n)
		{
			int j = rand()%n;
			printf("%d, ", j);
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
			//printf("%p\n",ptrb);
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
	wait(2);
	int f=0;
	while(f<1000)
	{	int i=0;

		while(i<n+1)
		{
			switcha(i,sizeint);
			i=i+1;
		}
	f=f+1;
	}
	exit(0);
	return(0);
	}
	 	 
	 
	 else
	 {
		 printf("El proceso hijo no pudo ser creado\n");
	 }
	wait(3);
	 
	printf("\n Conjunto 1 ordenado\n");
	int j=0;
	while(j<n){
		leer_posix(j,sizeint);
		j=j+1;
	}
	printf("\nProceso padre finalizado\n"); 
	 return 0; 
 }



 /* ======================================================================*/
 /* ====================== FUNCIONES AUXILIARES ==========================*/
 /* ======================================================================*/

 
void switcha(int columna,int size)	//compara dos valores consecutivos
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
		}
		
		
		close(fd);
}
 
void leer_posix(int columna,int size)	//compara dos valores consecutivos
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
		
		if(ptr1==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		
		char *a=ptr1;
		int aa =  atoi(a);

		printf("%d, ",aa);
		close(fd);
}


void leer_posix2(int columna,int size)
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
		
		if(ptr1==MAP_FAILED)
		{
			printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
			exit(1);
		}
		
		char *a=ptr1;
		int aa =  atoi(a);

		printf("%d, ",aa);
		close(fd);
}

int particion (int izquierda, int derecha, int size){		// esta funcion es el núclero del quicksort:
      		int fd;						// compara los elementos del conjunto recorriendo desde los extremos
	  	char *ptr_izq;					// e intercambiando las posiciones según corresponda
	  	char *ptr_der;
	  	char bufaa[size];
	  	char bufbb[size];
	  	struct stat shmobj_st;
  		fd = shm_open(MEMORIA2, O_RDWR,0);
     		if(fd == -1){
			  printf("Error fd %s\n", strerror(errno));
			  exit(1);
		}
		if(fstat(fd, &shmobj_st)==1){
			  printf("error fstat \n");
		  	  exit(1);
		}
      
 		ptr_izq=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0)+izquierda*size;
		ptr_der=mmap(NULL,shmobj_st.st_size, PROT_READ, MAP_SHARED,fd,0)+derecha*size;	
	  	if(ptr_izq==MAP_FAILED)
	  	{
		 	printf("Fallo el proceso de mapeo leyendo el proceso %s\n",strerror(errno));
		  	exit(1);
	  	}
		  
		char *a=ptr_izq;
		char *b=ptr_der;
		int vizq =  atoi(a);
	  	int vder = atoi(b);
      		int pivote = vizq;
      
      		while (1){

        		ptr_izq=mmap(0,size, PROT_READ, MAP_SHARED,fd,0)+izquierda*size;
		    	ptr_der=mmap(0,size, PROT_READ, MAP_SHARED,fd,0)+derecha*size;
		    	char *a=ptr_izq;
		    	char *b=ptr_der;
	  	  	vizq =  atoi(a);
	    		vder = atoi(b);
  		  
        		if (vizq < pivote){
          			izquierda++;
         			ptr_izq=mmap(0,size, PROT_READ, MAP_SHARED,fd,0)+izquierda*size;
          			char *a=ptr_izq;
         			vizq =  atoi(a);
        		}
        		if (vder > pivote){
          			derecha--;
          			ptr_der=mmap(0,size, PROT_READ, MAP_SHARED,fd,0)+derecha*size;
          			char *b=ptr_der;
          			vder = atoi(b);
        		}
        		if (izquierda >= derecha){
          			return derecha;  
        		} 
        		else {
          			ptr_izq=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+izquierda*size;
		      		ptr_der=mmap(0,size, PROT_WRITE, MAP_SHARED,fd,0)+derecha*size;
          			sprintf(bufaa,"%d\n",vizq);
			   	sprintf(bufbb,"%d\n",vder);			
		    		memcpy(ptr_izq,bufbb,sizeof(bufbb));
			   	memcpy(ptr_der,bufaa,sizeof(bufaa));
          			izquierda++;
          			derecha--;
          		}
       		}
       		close (fd);
}   


void quicksort(int izquierda,int derecha,int size){	// compara e intercambia los elementos del conjunto mediante el uso de la funcion particion
							// y lo vuelve a hacer recursivamente seccionando el conjunto.
    if (izquierda < derecha){
      int indice = particion(izquierda, derecha, size);
      quicksort(izquierda,indice,size);
      quicksort(indice+1,derecha,size);
      }
}


