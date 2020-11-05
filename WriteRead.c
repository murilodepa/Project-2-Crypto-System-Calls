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
#include <linux/mutex.h>
#include "processInfo.h"

/* Skcipher kernel crypto API */
#include <crypto/skcipher.h>
/* Scatterlist manipulation */
#include <linux/scatterlist.h>
/* Error macros */
#include <linux/err.h>

struct tcrypt_result
{
    struct completion completion;
    int err;
};

/* tie all data structures together */
struct skcipher_def
{
    struct scatterlist sg;
    struct crypto_skcipher *tfm;
    struct skcipher_request *req;
    struct tcrypt_result result;
};


static char message[1024] = {0};
char result[32];
char hashResult[40];
char message_conv[34];   

//CRIPTOGRAFIA
static void test_skcipher_cb(struct crypto_async_request *req, int error)
{
    struct tcrypt_result *result = req->data;

    if (error == -EINPROGRESS)
        return;
    result->err = error;
    complete(&result->completion);
    pr_info("Encryption finished successfully\n");
}

/* Realiza a conversão de Hexadecimal para String */
void conv(char *vet, char *resul, int tamanho)
{
    int calc = 0;
    int j = 0;
    int i = 0;
    while (i < tamanho)
    {
        if (vet[i] > 96)
        {
            calc += (vet[i] - 87) * 16;
        }
        else
        {
            calc += (vet[i] - 48) * 16;
        }

        if (vet[i + 1] > 96)
        {
            calc += (vet[i + 1] - 87);
        }
        else
        {
            calc += (vet[i + 1] - 48);
        }
        resul[j] = calc;
        calc = 0;
        j++;
        i += 2;
    }
    resul[j] = '\0';
}

/* Conversão para string */
int toString(unsigned char n)
{
    if (n > 9)
    {
        n += 87;
    }
    else
    {
        n += 48;
    }
    return n;
}

static unsigned int test_skcipher_encdec(struct skcipher_def *sk,
                                         int enc)
{
    int rc = 0;

    if (enc)
        rc = crypto_skcipher_encrypt(sk->req);
    else
        rc = crypto_skcipher_decrypt(sk->req);

    if (rc)
        pr_info("skcipher encrypt returned with result %d\n", rc);

    return rc;
}

/* Initialize and trigger cipher operation */
static int test_skcipher(char *scratchpad1, int tam, int tipo)
{
    struct skcipher_def sk;
    struct crypto_skcipher *skcipher = NULL;
    struct skcipher_request *req = NULL;
    char *scratchpad = NULL;
    char *ivdata = NULL;
    unsigned char key[32];
    int ret = -EFAULT;
    char *resultdata = NULL;

    skcipher = crypto_alloc_skcipher("ecb(aes)", 0, 0);
    if (IS_ERR(skcipher))
    {
        pr_info("could not allocate skcipher handle\n");
	
        return PTR_ERR(skcipher);
    }
	printk("TESTANDO ERRO\n");
    req = skcipher_request_alloc(skcipher, GFP_KERNEL);
    if (!req)
    {
        pr_info("could not allocate skcipher request\n");
        ret = -ENOMEM;
        goto out;
    }

    skcipher_request_set_callback(req, CRYPTO_TFM_REQ_MAY_BACKLOG,
                                  test_skcipher_cb,
                                  &sk.result);

    /* AES 256 with random key */
    //key=keyp;
    memcpy(key, "0123456789123456",16);
    if (crypto_skcipher_setkey(skcipher, key, 16))
    {
        pr_info("key could not be set\n");
        ret = -EAGAIN;
        goto out;
    }

    /* IV will be random */
    ivdata = kmalloc(16, GFP_KERNEL);
    if (!ivdata)
    {
        pr_info("could not allocate ivdata\n");
        goto out;
    }
    memcpy(ivdata, "0123456789123456",16);
    //ivdata=iv;

    /* Input data will be random */
    scratchpad = kmalloc(16, GFP_KERNEL);
    if (!scratchpad)
    {
        pr_info("could not allocate scratchpad\n");
        goto out;
    }
    memcpy(scratchpad, scratchpad1,16);

    sk.tfm = skcipher;
    sk.req = req;

    /* We encrypt one block */
    sg_init_one(&sk.sg, scratchpad, 16);
    skcipher_request_set_crypt(req, &sk.sg, &sk.sg, 16, ivdata);
    init_completion(&sk.result.completion);
    //crypto_init_wait(&sk.wait);

    /* encrypt data */
    ret = test_skcipher_encdec(&sk, tipo);
    if (ret)
        goto out;

    pr_info("Encryption triggered successfully\n");
    resultdata = sg_virt(&sk.sg);
    memcpy(result, resultdata,16);
	
	printk("\nPASSOUMESSMO??\n");

out:
    if (skcipher)
        crypto_free_skcipher(skcipher);
    if (req)
        skcipher_request_free(req);
    if (ivdata)
        kfree(ivdata);
    if (scratchpad)
        kfree(scratchpad);
    return ret;
}


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

asmlinkage long write_crypt(int fd,char *buffer, size_t nbytes)  {    
	printk("ENTROU");
	
	//char str[27]="TESTANDO STRING DE ESCRITA";
	printk("ABLALBALBLAB");
	printk("%s",buffer);
	int n;
	
	n=strlen(buffer);
	
	int cont=n;
	
	test_skcipher(buffer, 16, 1);//1 p/ encrypt

	char vet[64];
	int j=0,i=0;
	while (i < 16)
        {
            vet[j] = toString((unsigned char)result[i] / 16);
            j++;
            vet[j] = toString((unsigned char)result[i] % 16);
            j++;
            i++;
        }
        vet[j] = '\0';
	
	printk("%s\n",vet);

 	file_write(fd,vet,32);	

  return 0;
}


asmlinkage long read_crypt(int fd,char *buffer, size_t nbytes) { 

	char buff[50],message_conv[16];
	size_t n;

	n=sizeof(buff);
	
	file_read(fd,buff,32);
	

	printk("PRINTANFDO BUFF: %s\n",buff);

	conv(buff, message_conv, 32);

	//printk("PRINTANDO MESSAGE_CONV: %s\n",message_conv);	

	test_skcipher(message_conv, 16, 0);//0 p/ decrypt

	printk("\nTESTANDO\n");
	printk("%s\n",result);

	memcpy(buffer,result,16);

	buffer[16]='\0';

	printk("\nTESTANDO BUFFER\n");
	printk("%s",buffer);
	return 0;
}
