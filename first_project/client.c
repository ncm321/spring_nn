/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  client.c
 *    Description:  This file a client for the first project.
 *                 
 *        Version:  1.0.0(16/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "16/03/24 16:06:21"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <fcntl.h>
#include <getopt.h>
#include <arpa/inet.h>
#include <time.h>
#include <libgen.h>
#include <linux/tcp.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sqlite3.h>
#include "mysqlite3.h"

#define  MSG_STR "Hello,I am ready to connect with you!"
#define  TEMP "temperature"
#define  HEARTBEAT_INTERVAL 5
#define  SQL_NAME      "COMPANY"

void print_usage(char *progname);
int  get_temperature(float *temp);
int  get_devid(char *id,int len);
void get_time(char *time_buf);
void send_heartbeat(int sockfd);
int  connect_to_server();
int  socketconnected(int sockfd);

int      port=0;
char    *servip;
int main (int argc, char **argv)
{
	int                  conn_fd=-1;
	int                  rv=-1;
	struct sockaddr_in   serv_addr;
//	int                  port;
//	char                *servip=NULL;
	char                 buf[1024];
	int                  ch=-1;
	int                  temp_time=10;
	float                temp;
	char                 id[16];
	int                  len=16;
	char                 time_buf[128];
	char                 all_buf[256];
//	time_t               last_heartbeat_time;
//	time_t               current_time;
//	ssize_t              num_bytes;
    int                  error = 0;
	socklen_t            err_len = 0;
	sqlite3             *db;
	char                *sqlnam3="DB_TB_CLI";
    int                  tb_num=-1;
	char                 rt_buf[128];

	struct option       opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"temp_time",required_argument,NULL,'t'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((ch=getopt_long(argc,argv,"i:p:t:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'i':
				servip=optarg;
				break;
			case 'p':
				port=atoi(optarg);
				break;
			case 't':
				temp_time=atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}

	if(!servip||!port||!temp_time)
	{
		print_usage(argv[0]);
		return 0;
	}
	conn_fd=connect_to_server();
	/* init the last time of sending heartbeat */
//	last_heartbeat_time = time(NULL);

	while(1)
	{
	/* Acquire devece id */
    	if((rv=get_devid(id,len))<0)
    	{

	    	printf ("Get device id failure:%s\n",strerror(errno));
	    	goto cleanUp;
    	}
    	printf("debug_Device id:%s\n",id);
    	/* Acquire current time */
    	get_time(time_buf);
//  	printf("current time:%s\n",time_buf);
    	/* Acquire temperature */
    	if((rv=get_temperature(&temp))<0)
    	{
	    	printf("Acquire temperature failure:%s\n",strerror(errno));
	    	goto cleanUp;
	    }
	
//  	printf ("debug00:temp:%.2f\n",temp);

	    memset(buf,0,sizeof(buf));
		snprintf(buf,sizeof(buf),"%s:%.2f",TEMP,temp);

		snprintf(all_buf,sizeof(all_buf),"%s %s %.2f",id,time_buf,temp);
		/*  connected or not  */
		rv=socketconnected(conn_fd);
		if( 1== rv)//连接成功，将数据上传到服务器
		{
		/* After package judge connected or not */
/* 		if(getsockopt(conn_fd,SOL_SOCKET,SO_ERROR,&error,&err_len) != 0)
		{
	
			printf ("getsockopt failure:%s\n",strerror(errno));
			break;
		}
		if(error != 0)
		{

			printf ("Connection lost.\n");
			break;
		}
*/	
		if(write(conn_fd,all_buf,strlen(all_buf))<0)
		{

			printf ("Write to server failure:%s\n",strerror(errno));
			goto cleanUp;
		}	

		printf ("Update %s successfully!\n",all_buf);
		}
		else//连接失败，进行重连,并把数据暂存到数据库中
		{
			if((rv=sqlite3_open("test.db",&db)))
			{
				fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
				exit(0);
			}
			else
				fprintf(stdout,"Table created successfully\n");

		//将数据暂存于数据库中
		//
		/* 创建数据库和表 ,先判断表是否存在*/  
        	if((rv = create_table(db,SQL_NAME)) <0)
			{

				printf ("Create database failure:%s\n",strerror(errno));
				goto clean;
			}

 			if((rv=table_insert(db,SQL_NAME,id,time_buf,&temp))<0)
			{	

				printf ("Insert data to table failure:%s\n",strerror(errno));
				goto clean;
			}

		/* 查看数据库中表的数据*/
			if((rv=table_select(db,SQL_NAME))<0)
			{

				printf ("Select data failure:%s\n",strerror(errno));
				goto clean;
			}
        /* 数据已存入数据库，等待重连 */
			close(conn_fd);
			conn_fd=connect_to_server();
			if(conn_fd <= 0)
			{

				printf ("Reconnect to server failure:%s\n",strerror(errno));
			}
			else
			{
				 printf("client %d reconnect to server successfully!\n",conn_fd);
				do
				{

			
				printf ("debug02:Repeat!!where is segmatation out!!\n");
			    /* 判断表里有多少行数据 */
			    memset(rt_buf,0,sizeof(rt_buf));
			    tb_num=determine_table_exist(SQL_NAME,db,rt_buf);
			    printf ("debug03:The table %s has %d num\n",SQL_NAME,tb_num);

			    /* 把表里数据上传到服务端 */
		//		printf ("dddddddddd\n");
			
				if((rv=write(conn_fd,rt_buf,strlen(rt_buf)))<0)
				{

					printf ("Write data to server from table failure:%s\n",strerror(errno));
					goto cleanUp;
				}
				else
					printf ("debug04:Write %s from table to server successfully!\n",rt_buf);
			    rv=table_delete(db,SQL_NAME);
				tb_num--;

				}while(0!=tb_num);
				
			}
		}
	
clean:
		sqlite3_close(db);
		close(conn_fd);
	sleep(temp_time);
	}
cleanUp:
	close(conn_fd);
	return 0;
} 

void print_usage(char *progname)
{
	printf("%s:usage:\n",progname);
	printf("-i(--ipaddr):specify server IP address.\n");
	printf("-p(--port):specify server port.\n");
	printf ("-t(--time):temp upload time\n");
	printf("-h(--help):print this help information.\n");

	return ;
}

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
		printf("Open floder %s failure:%S\n",w1_path,strerror(errno));
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

		printf ("Cannot find ds18b20 chipset\n");
		rv=-2;
		goto cleanup;
	}
	strncat(w1_path,chip_sn,sizeof(w1_path)-strlen(w1_path));
	strncat(w1_path,"/w1_slave",sizeof(w1_path)-strlen(w1_path));

	fd=open(w1_path,O_RDONLY);
	if(fd<0)
	{
		printf ("Open file failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));

	if(read(fd,buf,sizeof(buf))<0)
	{

		printf ("Read file failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanup;
	}

	ptr = strstr(buf,"t=");
	if(!ptr)
	{
		
		printf ("Can not find string t=\n");
		rv =-5;
		goto cleanup;
	}
	ptr += 2;
	*temp=atof(ptr)/1000;
	
//	printf ("temperature:\n");
	close(fd);
cleanup:
	if(fd)
		close(fd);
	if(!dirp)
		closedir(dirp);

	return rv;
}

int  get_devid(char *id,int len)
{
	int          sn=1;
	int          rv=0;

	if(!id||!len)
	{

		printf ("Argument error:%s\n",strerror(errno));
		rv=-1;
	}
	snprintf(id,len,"DEVID%03d",sn);
	return rv;
}

void  get_time(char *time_buf)
{
	time_t        timep;
	struct tm    *p;
	time(&timep);
	p=localtime(&timep);

	memset(time_buf,0,sizeof(time_buf));

//	printf ("%d\n",sizeof(time_buf));
	snprintf(time_buf,128,"%d/%d/%d--%d:%d:%d",1900+p->tm_year,1+p->tm_mon,p->tm_mday,p->tm_hour,p->tm_min,p->tm_sec);

	printf("%s\n",time_buf);
	return ;
//	strncpy(time_buf,asctime(gmtime(&timep)),sizeof(time_buf));

}

void send_heartbeat(int sockfd)
{
	const char                *heartbeat_msg="heartbeat";
	if(send(sockfd,heartbeat_msg,strlen(heartbeat_msg),0)==-1)
	{
		printf ("Send failure:%s\n",strerror(errno));
		
	}
	else
		printf ("Heartbeat sent successfullt!\n");
}

int connect_to_server()
{
	struct sockaddr_in      server_addr;
	int                     sockfd;
	socklen_t                len=0;

	if((sockfd=socket(AF_INET,SOCK_STREAM,0))==-1)
	{
		
		printf ("Create socket failure:%s\n",strerror(errno));
		return -1;
	}

	memset(&server_addr,0,sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	if(inet_aton(servip,&server_addr.sin_addr)==-1)
	{

		printf ("inet_pton:%s\n",strerror(errno));
		return -2;
	}
	if(connect(sockfd,(struct sockaddr *)&server_addr,sizeof(server_addr))<0)
	{

		printf ("debug00:sockfd is %d\n",sockfd);

		printf ("Connect failure:%s\n",strerror(errno));
		close(sockfd);
		return -3;
	}

	printf ("connectd to server successfully!\n");
	return sockfd;
}

int socketconnected(int sockfd)
{
	struct tcp_info info;
	int    len=sizeof(info);

	if(socket<=0)
	{
		printf ("connected failure:sockfd<=0\n");
		return 0;
	}
	getsockopt(sockfd,IPPROTO_TCP,TCP_INFO,&info,(socklen_t *)&len);//这儿len没有直接定义成socket_t的数据类型，而是先int sizeof之后强制转的，直接socket_t的不能sizeof吗
	if(info.tcpi_state == 1)
	{

		printf ("socket connected!\n");
		return 1;
	}
	else
	{

		printf ("socket disconnected\n");
		return 0;
	}
}
