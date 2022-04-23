 #include <unistd.h>
 #include <stdio.h>
 int var = 22;
 int main(void)
 {
	 pid_t pidC;
	 printf("**proceso PID =%d comienza\n",getpid());
	 pidC = fork();
	 printf("**proceso PID =%d, pidC = %d ejecutrandosen\n",getpid(),pidC);
	 
	 if(pidC>0) //soy el proceso padre
	 {
		 var=23;
	 }
	 else if(pidC == 0)//entonces soy el proceso hijo
	 { 
		 var=15;
	 }
	 
	 else
	 {
		 printf("El proceso hijo no pudo ser creado\n");
	 }
	 while(1)
	 {
		sleep(1);
		printf("%d\n",var); 
	 }
	 
	 return 0;
	 
	 
 }
