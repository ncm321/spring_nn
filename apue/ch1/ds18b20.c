#include<stdio.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<unistd.h>
#include<dirent.h>


int get_temperature(float *temp);
int main(int argc,char **argv)
{
	float          temp;
	int            rvrv;

	rvrv=get_temperature(&temp);
	if(rvrv<0)
	{
		printf("Get temperature failure,return value:%d",rvrv);
		return -1;
	}
	printf("temp: %f\n",temp);
}

int get_temperature(float *temp)
{
	int            fd=-1;
	char           buf[128];
	char          *ptr=NULL;
	DIR           *dirp=NULL;
	struct dirent *direntp=NULL;
	char           w1_path[64]="/sys/bus/w1/devices/";
	char           chip_sn[32];
	//char         ds18b20_path[64];
	int            found=0;
	int            rv=10;

	dirp=opendir(w1_path);
	if(!dirp)
	{
		printf("Open folder %s failure:%s\n",w1_path,strerror(errno));
		rv=-1;
		goto cleanup;
	}

	while(NULL != (direntp=readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found = 1;
		}
	}
	closedir(dirp);

	if (!found)
	{
		printf("Can not find ds18b20 chipset\n");
		rv=-4;
		goto cleanup;
	}
	//snprintf(ds18b20_path,sizeof(ds18b20_path),"%s/%s/w1_slave",w1_path,chip_sn);
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
//	printf("w1_path:%s\n",w1_path);

	fd=open(w1_path,O_RDONLY);
	if(fd < 0)
	{
		printf("Open file failure: %s\n",strerror(errno));
		rv=-2;
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{
		printf("Read file failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanup;
	}
//	printf("buf:%s\n",buf);

	ptr = strstr(buf,"t=");
	if(!ptr)
	{
		printf("Can not finf t= string\n");
		rv=-5;
		goto cleanup;
	}

	ptr += 2;
//	printf("ptr:%s\n",ptr);

       	*temp = atof(ptr)/1000;
	printf("temprature:%f\n",*temp);

	close(fd);

	
cleanup:
	if(!dirp)
		closedir(dirp);
	if(fd)
		close(fd);

	return rv;
}
