/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(07/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "07/03/24 19:36:03"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdlib.h>
int       get_temperature(float *temp);
int       main(int argc,char **argv)
{
	int             rv;
	float           temp;

	if((rv=get_temperature(&temp))<0)
	{
		printf("Get temperature failure:%s\n",strerror(errno));
		return -1;
	}
	printf("temp:%f\n",temp);
	return 0;
}

int       get_temperature(float *temp)
{
	int                fd=-1;
	int                rv=0;
	int                found=0;
	DIR               *dirp=NULL;
	struct dirent     *direntp=NULL;
	char              *ptr=NULL;
	char               w1_path[64]="/sys/bus/w1/drivers/";
	char               chip_sn[32];
	char               buf[128];

	if(NULL==(dirp=opendir(w1_path)))
	{
		printf("Open floder %s failure:%s\n",strerror(errno));
		goto cleanUp;
		return -1;
	}

	if(NULL!=(direntp=readdir(dirp)))
	{
		printf("debug00:\n");
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found=1;
			printf("debug01:chip_sn:'%s'\n",chip_sn);
		}
	}

	if(!found)
	{
		printf("Can not find ds18b20 chipset\n");
		return -2;
		goto cleanUp;
	}

	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
	if((fd=open(w1_path,O_RDONLY))<0)
	{
		printf("Open file failure:%s\n",strerror(errno));
		return -3;
		goto cleanUp;
	}

	memset(buf,0,sizeof(buf));
	if((rv=read(fd,buf,sizeof(buf)))<0)
	{
		printf("Read failure:%s\n",strerror(errno));
		return -5;
		goto cleanUp; 
	}
	if(!strstr(buf,"t="))
	{
		printf("Can not find string 't='\n");
		return -4;
		goto cleanUp;
	}

	ptr += 2;
	*temp=atof(ptr)/1000;
	printf("temperature:%f\n",*temp);

	return rv;
cleanUp:
	if(dirp)
		closedir(dirp);
	if(fd>0)
		close(fd);
}
