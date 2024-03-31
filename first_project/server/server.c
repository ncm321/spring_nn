/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(18/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "18/03/24 14:40:42"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <libgen.h>
#include <netinet/in.h>
#include <sqlite3.h>
#include "mysqlite3.h"


#define   MAX_EVENTS        512
#define   ARRAY_SIZE(x)     (sizeof(x)/sizeof(x[0]))
#define   SQL3_NAME           "SERVER_TABLE"

static inline void  print_usage(char *progname);
int                 socket_server_init(char *listen_ip,int listen_port);
void                set_socket_rlimit(void);


int main (int argc, char **argv)
{
	int                 ch=-1;
	int                 rv=0;
	int                 listen_fd,conn_fd=-1;
	int                 on=1;
	int                 daemon_run=0;
	int                 i,j;
	int                 read_rv=-1;
	int                 port;
	char                buf[1024];

	int                 epollfd;
	struct epoll_event  event;
	struct epoll_event  event_array[MAX_EVENTS];
	int                 events;
	sqlite3            *db;
	char                buf_sn[32];
	char                buf_time[128];
	char                table_temp[32];
	float               tb_temp;
	int                 ret=0;

	struct option       opts[]={
		{"daemon",no_argument,NULL,'b'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((ch=getopt_long(argc,argv,"bp:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'b':
				daemon_run=1;
				break;
			case 'p':
				port = atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return EXIT_SUCCESS;
			default:
				break;
		}
	}

	if(!port)
	{
		print_usage(argv[0]);
		return -1;
	}
	set_socket_rlimit();

	if((listen_fd=socket_server_init(NULL,port))<0)
	{

		printf ("ERROR:%s server listen on port %d failure:%s\n",argv[0],port);
		return -2;
	}
	printf ("%s server start to listen on port %d\n",argv[0],port);

	/* set program running on background */
	if(daemon_run)
		daemon(0,0);

	if((epollfd = epoll_create(MAX_EVENTS))<0)
	{

		printf ("epoll_create() failure %s\n",strerror(errno));
		return -3;
	}

	event.events = EPOLLIN;
	event.data.fd = listen_fd;

	if(epoll_ctl(epollfd,EPOLL_CTL_ADD,listen_fd,&event)<0)
	{

		printf ("epoll add listen socket failure:%s\n",strerror(errno));
		return -4;
	}
	for( ; ; )
	{
		/* program will blocked here */
		events = epoll_wait(epollfd,event_array,MAX_EVENTS,-1);
		if(events<0)
		{

			printf ("epoll failure:%s\n",strerror(errno));
			break;
		}
		else if(events == 0)
		{

			printf ("epoll get timeout.\n");
			continue;
		}

		/* events>0 is the active events count */

		printf ("debug03:events:%d\n",events);
		for(i=0;i<events;i++)
		{
			if((event_array[i].events & EPOLLERR)||(event_array[i].events & EPOLLHUP))
			{
				
				printf ("epoll_wait get error on fd[%d] :%s\n",event_array[i].data.fd,strerror(errno));
				epoll_ctl(epollfd,EPOLL_CTL_DEL,event_array[i].data.fd,NULL);
				close(event_array[i].data.fd);
			}
			/* listen socket get event means new client start connect now */
			if(event_array[i].data.fd == listen_fd)
			{
				if((conn_fd=accept(listen_fd,(struct sockaddr *)NULL,NULL))<0)
				{

					printf ("accept new client failure:%s\n",strerror(errno));
					continue;
				}


				printf ("debug02:%d\n",conn_fd);
				event.data.fd = conn_fd;
				event.events = EPOLLIN;
				if(epoll_ctl(epollfd,EPOLL_CTL_ADD,conn_fd,&event)<0)
				{

					printf ("epoll add client socket failure:%s\n",strerror(errno));
					close(event_array[i].data.fd);
					continue;
				}

				printf ("epoll add new client socket[%d] successfully.\n",conn_fd);
			}
			else
			
			{
				memset(buf,0,sizeof(buf));
				if((read_rv = read(event_array[i].data.fd,buf,sizeof(buf)))<=0)
				{
					printf ("debug01:the buf of size:%d and rv:%d\n",sizeof(buf),read_rv);

					printf ("socket[%d] read failure or get disconnect and will be removed.\n",event_array[i].data.fd);
					epoll_ctl(epollfd,EPOLL_CTL_DEL,event_array[i].data.fd,NULL);
					close(event_array[i].data.fd);
					continue;
				}
				else
				{

					printf ("socked[%d] read get %d bytes data:%s\n",event_array[i].data.fd,read_rv,buf);
			        ret = sscanf(buf,"%s %s %s",buf_sn,buf_time,table_temp);
					printf ("debug04:%s,%s,%.2f\n",buf_sn,buf_time,atof(table_temp));
					tb_temp=atof(table_temp);
					/* create a new table */
					if((rv=create_table(db,SQL3_NAME))<0)
					{

						printf ("Create table failure:%s\n",strerror(errno));
					}
					/* insert data to table */
					if((rv=table_insert(db,SQL3_NAME,buf_sn,buf_time,&tb_temp))<0)
					{

						printf ("Insert data to table failure:%s\n",strerror(errno));
					}
					/* select data */
					if((rv=table_select(db,SQL3_NAME))<0)
					{

						printf ("Select data from table failure:%s\n",strerror(errno));
					}
					/* delete or not */
		//			rv=table_delete(db,SQL3_NAME);
				}
			}
		}
	}

cleanUp:
	close(listen_fd);
	return 0;
} 

int    socket_server_init(char *listen_ip,int listen_port)
{
	struct sockaddr_in        servaddr;
	int                       rv = 0;
	int                       on = 1;
	int                       listenfd;

	if( (listenfd = socket(AF_INET,SOCK_STREAM,0))<0)
	{

		printf ("Use socket() to vreate a TCP socket failure:%s\n",strerror(errno));
		return -1;
	}

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(listen_port);

	if(!listen_ip)
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else{
		if( inet_pton(AF_INET,listen_ip,&servaddr.sin_addr)<=0 )
		{

			printf ("inet_pton() set listen IP failure.\n");
			rv=-2;
			goto cleanUp;
		}
	}

	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if( bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr))<0)
	{

		printf ("Use bind() to TCP socket failure:%s\n",strerror(errno));
		rv=-3;
		goto cleanUp;
	}

	if(listen(listenfd,64)<0)
	{

		printf ("Listen to TCP failure:%s\n",strerror(errno));
		rv=-4;
		goto cleanUp;
	}

cleanUp:
	if(rv<0)
		close(listenfd);
	else
		rv=listenfd;

	return rv;
}
void   print_usage(char *progname)
{
	printf ("Usage:%s [OPTION]...\n",progname);
	printf ("%s is a socket server program\n",progname);

	printf ("-b(--daemon) set program running on background\n");
	printf ("-p(--port):specify server port\n");
	printf ("-h(--help):print this help information.\n");
	return ;
}

void   set_socket_rlimit(void)
{
	struct rlimit limit = {0};

	getrlimit(RLIMIT_NOFILE,&limit);
	limit.rlim_cur = limit.rlim_max;
	setrlimit(RLIMIT_NOFILE,&limit);

	printf ("Set socket open fd max count to %d\n",limit.rlim_max);
}
