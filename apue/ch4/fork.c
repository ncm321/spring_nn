#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>

int main(int argc,char **argv)
{
	pid_t       pid;

	printf("Parent process PID[%d] start running...\n",getpid());

	pid = fork();
	if(pid <0)
	{
		printf("fork() create chile process failure:%s\n",strerror(errno));
		return -1;
	}
	else if( pid == 0)
	{
		printf("Child process POD[%d] start running,my parent PID is [%d]\n",getpid(),getppid());
		return 0;
	}
	else
	{
		printf("Parent process PID[%d] continue running,and child process PID is [%d]\n",getpid(),pid);
		return 0;
	}
}
