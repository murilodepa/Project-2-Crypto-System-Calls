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
         
    	long int ret_status = syscall(333,fd,1); 
	
	ret_status = syscall(333,fd,2);
         
    	
	close(fd);
         
    if(ret_status == 0) {
         printf("System call 'listProcessInfo' executed correctly. Use dmesg to check processInfo\n");
	
}
    
    else 
         printf("System call 'listProcessInfo' did not execute as expected\n");
          
     return 0;
}
