#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int main()
{  
    	printf("Invoking 'listProcessInfo' system call");

	int fd = open("arquivo.txt",O_WRONLY|O_CREAT|O_TRUNC,0666);
	//open("arquivo.txt",O_RDONLY|O_CREAT|O_TRUNC,0666);
//ou
//open("arquivo.txt",O_RDWR|O_CREAT|O_TRUNC,0666);

	if(fd<0) printf("deu ruim no arquivo");
	
	char buffer[16];

	printf("ESCREVA A STRING:\n");
	__fpurge(stdin);
	gets(buffer);
	
	int n;
	
	n=strlen(buffer);

	int cont=n;

	while(cont<16){
		
		buffer[cont]='0';
		cont++;	
	}
	buffer[cont]='\0';

        
    	long int ret_status = syscall(333,fd,buffer,n); 
	
	//printf("\nRetorno buffer: %s\n",buffer);
	//char bufferReturn;
	//ret_status = syscall(333,fd,2,bufferReturn);
        //printf("%s",bufferReturn);
	if(close(fd)<0) printf("\nerro ao fechar arquivo\n");

         
    if(ret_status == 0) {
         printf("System call 'listProcessInfo' executed correctly. Use dmesg to check processInfo\n");
	
}
    
    else 
         printf("System call 'listProcessInfo' did not execute as expected\n");
          
     return 0;
}
