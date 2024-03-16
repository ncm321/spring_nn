#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

int main(int argc,char **argv)
{
	struct stat stbuf;

	stat("stat.c",&stbuf);
	printf("File Mode:%o Real Size: %1uB,Space Size:%1uB\n",stbuf.st_mode,stbuf.st_size,stbuf.st_blksize);

	return 0;
}
