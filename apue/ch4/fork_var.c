#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int g_var = 6;
char g_buf[]="A string write to stdout.\n";
int main(int argc,char **argv)
{
	int         var = 88;
	pid_t       pid;

	if(write(STDOUT_FILENO,g_buf,sizeof(g_buf)-1)<0)
	{
		printf("Write string to stdout error:%s\n",strerror(errno));
		return -1;
	}
	printf("Before fork.\n");

	if((pid=fork())<0)
	{
		printf("Fork() error:%s\n",strerror(errno));
		return -2;
	}
	else if( 0 == pid)
	{
		printf("Child process PID[%d] is running\n",getpid());
		var ++;
		g_var ++;
	}
	else
	{
		printf("Parent process PID[%d] is waiting\n",getpid());
		sleep(1);
	}

	printf("PID=%ld,g_var=%d,var=%d\n",(long)getpid,g_var,var);
	return 0;

}
