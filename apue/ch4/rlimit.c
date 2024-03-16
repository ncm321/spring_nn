#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>

void print_limits(char *name,int resource)
{
	struct rlimit    limit;
	if(getrlimit(resource,&limit)<0)
	{
		printf("Getrlimit for %s failure:%s\n",strerror(errno));
		return ;
	}

	printf("%-15s",name);
	if(limit.rlim_cur == RLIM_INFINITY)
		printf("(infinite)     ");
	else
		printf("%-15d",limit.rlim_cur);


	if(limit.rlim_max == RLIM_INFINITY)
		printf("(infinite)     ");
	else
		printf("%-15ld",limit.rlim_max);

	printf("\n");

}

int main(void)
{
	struct rlimit       limit = {0};

	print_limits("RLIMIT_NPROC",RLIMIT_NPROC);
	print_limits("RLIMIT_DATA",RLIMIT_DATA);
	print_limits("RLIMIT_STACK",RLIMIT_STACK);
	print_limits("RLIMIT_NOFILE",RLIMIT_NOFILE);

	printf("\nAfter set RLIMIT_NOFILE:\n");
	getrlimit(RLIMIT_NOFILE,&limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE,&limit);

	print_limits("RLIMIT_NOFILE",RLIMIT_NOFILE);

	return 0;
}