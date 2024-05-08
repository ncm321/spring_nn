/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(09/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "09/04/24 19:09:59"
 *                 
 ********************************************************************************/

#include "ds18b20.h"

int get_temperature(float *temp)
{
	int                 fd=-1;
	char                buf[128];
	char               *ptr=NULL;
	DIR                *dirp=NULL;
	struct dirent      *direntp=NULL;
	char                w1_path[64]="/sys/bus/w1/devices/";
	char                chip_sn[32];
	int                 found=0;
	int                 rv=10;

	dirp=opendir(w1_path);
	if(!dirp)
	{
		log_error ("Open floder %s failure:%S\n",w1_path,strerror(errno));
		rv=-1;
		goto cleanup;
	}

	while(NULL != (direntp=readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found=1;
		}
	}
	closedir(dirp);
	dirp=NULL;

	if(!found)
	{

		log_error ("Cannot find ds18b20 chipset\n");
		rv=-2;
		goto cleanup;
	}
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	fd=open(w1_path,O_RDONLY);
	if(fd<0)
	{
		log_error ("Open file failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{

		log_error ("Read file failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanup;
	}

	ptr = strstr(buf,"t=");
	if(!ptr)
	{

		log_error ("Can not find string t=\n");
		rv =-5;
		goto cleanup;
	}
	ptr += 2;
	*temp=atof(ptr)/1000;


cleanup:
	if(fd)
		close(fd);
	if(!dirp)
		closedir(dirp);

	return rv;
}
