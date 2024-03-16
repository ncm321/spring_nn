#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

int main(int argc,char *argv)
{
	int fd = -1;
	fd = open("std.txt",O_RDWR|O_CREAT|O_TRUNC,0666);
	if(fd<0)
	{
		printf("Open file failure:%s\n",strerror(errno));
		return -1;
	}
	printf("create fd[%d]\n",fd);
	close(0);
	close(2);
	printf("create fd[%d]\n",fd);
	dup(fd);
	printf("create fd[%d]\n",fd);
        dup(fd);
	printf("create fd[%d]\n",fd);
	dup(fd);
	dup2(fd,STDIN_FILENO);
	dup2(fd,STDOUT_FILENO);
	dup2(fd,STDERR_FILENO);


	printf("fd=%d\n",fd);
	close(fd);

	return 0;

}
