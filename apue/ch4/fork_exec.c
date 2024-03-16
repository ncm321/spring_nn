#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>

#define   TMP_FILE      "/tmp/.ifconfig.log"

int main(int argc,char **argv)
{
	pid_t         pid;
	int           fd;
	char          buf[1024];
	int           rv;
	char         *ptr;
	FILE         *fp;
	char         *ip_start;
	char         *ip_end;
	char          ipaddr[16];

	if( (fd = open(TMP_FILE,O_RDWR|O_CREAT|O_TRUNC,0644))<0 )
	{
		printf("Redirect standard output to file failure:%s\n",strerror(errno));
		return -1;
	}

	pid = fork();
	if(pid < 0)
	{
		printf("Fork() create child process failure:%s\n",strerror(errno));
		return -1;
	}
	else if( 0==pid )
	{
		printf("Child process start execute ifconfig program\n");
		dup2(fd,STDOUT_FILENO);//子进程继承父进程已经打开的fd，此时子进程重定向标准输出到父进程所打开的文件中
		/*execl()函数用来执行ifconfig命令
		 * 参数：命令路径，选项，参数，，，NULL
		 * 命令执行的结果原本在stdout，子进程重定向到fd里，且父进程不会复制该行为
		 * execl（）函数不会返回，因为去执行别的程序了，返回则失败
		 * 父进程到fd里读取ifconfig内容并进行寻找信息
		 */
		execl("/sbin/ifconfig","ifconfig","eth0",NULL);
		printf("Chile process execute another program,will not return here.Return here means execl() error\n");
		return -1; 
	}
	else
	{
		//让子进程先执行
		printf("Parent process is waiting...\n");
		sleep(3);
	}

	//子进程调用execl（）函数后不会再执行之后代码
	memset(buf,0,sizeof(buf));

	//父进程此时读不到内容，文件偏移量在文件末尾
	rv = read(fd,buf,sizeof(buf));
	printf("Read %d bytes data directly after child process write.\n",rv);

	//设置文件偏移量到开头
	memset(buf,0,sizeof(buf));
	lseek(fd,0,SEEK_SET);
	rv = read(fd,buf,sizeof(buf));
	printf("Read %d bytes data after lseek:\n %s\n",rv,buf);

	//read()会一次性读N字节进去buffer，想要一行一行读取，fdopen（）将fd转成文件流fp
	fp = fdopen(fd,"a+");
	fseek(fp,0,SEEK_SET);//重新设置文件偏移量
	while( fgets(buf,sizeof(buf),fp) )
	{
	//	printf("Debug00\n");
		//寻找包含要找内容那一行关键字，再在那一行匹配关键字解析IP地址
		if( strstr(buf,"netmask") )
		{
			ptr = strstr(buf,"inet");
			if( !ptr )
				continue;
		//	printf("Debug01:%s\n",ptr);
			ptr += strlen("inet");

		//	printf("Debug02:%s\n",ptr);
			while( isblank(*ptr) )
				ptr++;
			ip_start = ptr;

		//	printf("Debug03:%s\n",ptr);
			while( !isblank(*ptr) )
				ptr++;
			ip_end = ptr;

		//	printf("Debug04:%s\n",ptr);
			memset(ipaddr,0,sizeof(ipaddr));
			memcpy(ipaddr,ip_start,ip_end-ip_start);

			break;
		}
	}

	printf("Parser and get IP address:%s\n",ipaddr);

	fclose(fp);
	unlink(TMP_FILE);

	return 0;


}
