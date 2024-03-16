#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <getopt.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define  TEMP   "temperature"
int get_temperature(float *temp);
int main(int argc,char **argv)
{
	int                  conn_fd;
	char                *servip="127.0.0.1";
	int                  port=7777;
	int                  rvrv;
	int                  opt=1;
	float                Temp;
	float                tem;
	char                 buff[128];
	struct sockaddr_in   serv_addr;

	while(1)
	{

//	get_temperature(&Temp);/////////kkkkkkkk
	conn_fd = socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd < 0)
	{
		printf("Create conn_fd failure:%s\n",strerror(errno));
		return -1;
	}
	printf("Create conn_fd[%d] successfully!\n",conn_fd);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port =htons(port);
	inet_aton(servip,&serv_addr.sin_addr);

	printf("Start connecting...\n");
	if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		printf("Connect to server[%s:%d] failure:%s\n",servip,port,strerror(errno));
        	goto clean1;
	}
	printf("Connect to server[%s:%d] successfully!\n",servip,port);
    setsockopt(conn_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));

	
	

/*	        printf("Start connecting...\n");
                if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	        {
	           printf("Connect to server[%s:%d] failure:%s\n",servip,port,strerror(errno));
	       
		   goto clean1;
		}
		printf("Connect to server[%s:%d] successfully!\n",servip,port);
	        setsockopt(conn_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
*/
	//      printf("debug0\n");
		get_temperature(&Temp);/////////kkkkkkkk
//		printf("debug1\n");
		printf("Temperature:%.2f\n",Temp);
		memset(buff,0,sizeof(buff));
		snprintf(buff,sizeof(buff),"%s:%.2f\n",TEMP,Temp);
		printf("temp=%s\n",buff);

	//	write(conn_fd,TEMP,strlen(TEMP));

		if(write(conn_fd,buff,strlen(buff))<0)
		{
			printf("Write to server failure:%s\n",strerror(errno));
			goto clean1;
		}
		printf("Write temp:%s successfully!\n",buff);


//		printf("dbug2\n");

		memset(buff,0,sizeof(buff));
		if((rvrv = read(conn_fd,buff,sizeof(buff)))<0)
		{
			printf("Read from server failure:%s\n",strerror(errno));
			goto clean1;
		}
		else if(0 == rvrv)
		{
			printf("Disconnect server\n");
			goto clean1;
		}
		printf(" %s\n",buff);
		memset(buff,0,sizeof(buff));
		sleep(10);
/*		printf("Reconnecting...\n");
		if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
		{
			printf("Reconnecting failure:%s\n",strerror(errno));
			goto clean1;
		}
		printf("Reconnect successfully!\n");
		setsockopt(conn_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
*/
	
	close(conn_fd);
clean1:
	close(conn_fd);
	}
	return 0;



}

int get_temperature(float *temp)
{
	int            fd = -1;
	int            found = 0;
	int            rv=10;
	char           buf[128];
	char          *ptr=NULL;
	char           w1_path[64]="/sys/bus/w1/devices/";
	char           chip_sn[32];
	DIR           *dirp = NULL;
	struct dirent *direntp=NULL;
	

	
	dirp=opendir(w1_path);//打开文件夹，都是字符串，指针类型直接赋值
	if(!dirp)
	{
		printf("Open floder %s failure:%s\n",w1_path,strerror(errno));
		rv=-1;
		goto cleanup;
	}
//	printf("Open floder %s successfully!\n",w1_path);

//	printf("debug00\n");
	while(NULL != (direntp=readdir(dirp)))
	{
		if(strstr(direntp->d_name,"28-"))
		{
			strncpy(chip_sn,direntp->d_name,sizeof(chip_sn));
			found = -1;
			break;
		}
	}
//	closedir(dirp);

//	printf("debug01\n");
	if(!found)
	{
		printf("Can not find ds18b20 chipset\n");
		rv = -4;
		goto cleanup;
	}

	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));
//	printf("w1_path:%s\n",w1_path);

//	printf("debug02\n");
	fd=open(w1_path,O_RDONLY);
	if(fd<0)
	{
		printf("Open file failure:%s\n",strerror(errno));
		rv=-2;
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));
	if(read(fd,buf,sizeof(buf))<0)
	{
		printf("Read file failure:%s\n",strerror(errno));
		rv = -3;
		goto cleanup;
	}

	ptr = strstr(buf,"t=");
	if(!ptr)
	{
		printf("Can not find t= string\n");
		rv = -5;
		goto cleanup;
	}

	ptr += 2;

	*temp = atof(ptr)/1000;
	printf("temperature:%f\n",*temp);

//	printf("dbug03\n");

	close(fd);

//        dirp = NULL;
cleanup:
	if(dirp)
		closedir(dirp);
	if(fd)
		close(fd);
	return rv;
}

