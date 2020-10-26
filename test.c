#include <stdio.h>
#include <linux/kernel.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
int main()
{  
    	printf("Invoking 'listProcessInfo' system call");

	int fd = open("arquivo.txt",O_RDWR);

	if(fd<0) printf("deu ruim no arquivo");
	char buffer[16];
	printf("ESCREVA A STRING:\n");
	__fpurge(stdin);
	gets(buffer);
        
    	long int ret_status = syscall(333,fd,1,buffer); 
	
	//char bufferReturn;
	//ret_status = syscall(333,fd,2,bufferReturn);
        //printf("%s",bufferReturn);
	close(fd);
         
    if(ret_status == 0) {
         printf("System call 'listProcessInfo' executed correctly. Use dmesg to check processInfo\n");
	
}
    
    else 
         printf("System call 'listProcessInfo' did not execute as expected\n");
          
     return 0;
}
