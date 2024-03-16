#include<stdio.h>
#include<string.h>
#include<dirent.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<fcntl.h>

#define TEST_DIR "dir"

int main(int argc,char **argv)
{
	int rv;
	int fd1=-1;
	int fd2=-1;
	DIR *dirp;
	struct dirent *direntp;

	//创建文件夹dir,并设置文件夹权限为755
	if(mkdir(TEST_DIR,0755)<0)
	{
		printf("Create directory '%s' failure:%s\n",TEST_DIR,strerror(errno));
		return -1;
	
	}
	//更改当前工作路径到文件夹dir下去
	if(chdir(TEST_DIR)<0)
	{
		printf("Change directory to '%s' failure:%s\n",TEST_DIR,strerror(errno));
		rv=-2;
		goto cleanup;
	}
	//在dir文件夹下 创建普通文本文件file1.txt,并设置其权限为644

	if((fd1=open("file1.txt",O_RDWR|O_CREAT|O_TRUNC,0644))<0)
	{
		printf("Create file1.txt failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanup;
	}
	//在dir文件夹下 创建普通文本文件file2.txt,并设置其权限为644
	if((fd2=open("file2.txt",O_RDWR|O_CREAT|O_TRUNC,0644))<0)
	{
		printf("Create file2.txt failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanup;
	}
	//更改当前工作路径到父目录下去 
	if(chdir("../")<0)
	{
		printf("Change directory to '%s' failure:%s\n",TEST_DIR,strerror(errno));
		rv=-5;
		goto cleanup;
	}
	//打开dir文件夹
	if((dirp=opendir(TEST_DIR))==NULL)
	{
		rv=-6;
		printf("opendir %s error: %s\n",TEST_DIR,strerror(errno));
		goto cleanup;
	}
	//列出dir里面所有文件和文件夹
	while((direntp = readdir(dirp))!=NULL)
	{
		printf("Find file:%s\n",direntp->d_name);
	}

	closedir(dirp);

cleanup:
	if(fd1 >=0)
		close(fd1);
	if(fd2 >= 0)
		close(fd2);

	return 0;
}
