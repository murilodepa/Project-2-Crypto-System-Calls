#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/sched.h>
#include<linux/syscalls.h>
#include <linux/fcntl.h>
#include <linux/stddef.h>
#include <linux/uio.h>
#include <linux/types.h>
#include <linux/stat.h>
#include <linux/errno.h>
#include <linux/string.h>
#include "processInfo.h"

int file_write(struct file *file, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = sys_write(file, data, size);

    set_fs(oldfs);
    return ret;
}

int file_read(struct file *file, unsigned char *data, unsigned int size) 
{
    mm_segment_t oldfs;
    int ret;

    oldfs = get_fs();
    set_fs(get_ds());

    ret = sys_read(file, data, size);

    set_fs(oldfs);
	printk("TESTANDO DENTRO DA FUNC\n");	
	printk("%s",data);
    return ret;
}   

asmlinkage long sys_listProcessInfo(int fd, int tipo) {    
	if(tipo==1){
	char str[27]="TESTANDO STRING DE ESCRITA";

 	file_write(fd,str,27);

	}else if(tipo==2){

	char buff[50];
	size_t n;

	n=sizeof(buff);

	file_read(fd,buff,n);

	printk("\nTESTANDO\n");
	printk("%s",buff);

	}
	
  	
	

  return 0;
}
