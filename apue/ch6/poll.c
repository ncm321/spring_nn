/*********************************************************************************
 *      Copyright:  (C) 2024 lingyun<lingyun>
 *                  All rights reserved.
 *
 *       Filename:  poll.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(21/02/24)
 *         Author:  lingyun <lingyun>
 *      ChangeLog:  1, Release initial version on "21/02/24 14:29:47"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>
#include <time.h>
#include <pthread.h>
#include <getopt.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <poll.h>

#define ARRAY_SIZE(x)            (sizeof(x)/sizeof(x[0]))

static inline void print_usage(char *progname);
int                socket_server_init(char *listen_ip,int listen_port);

int main(int argc,char **argv)
{
	int                   listenfd,connfd;
	int                   serv_port = 0;
	int                   daemon_run = 0;
	char                 *progname=NULL;
	int                   opt;
	int                   rv;
	int                   i,j;
	int                   found;
	int                   max;
	char                  buf[1024];
	struct pollfd         fds_array[1024];

	struct option         long_options[] =
	{
		{"daemon",no_argument,NULL,'b'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	progname =basename(argv[0]);
	/* Parser the command line parameters */
	while((opt = getopt_long(argc,argv,"bp:h",long_options,NULL)) != -1 )
	{
		switch(opt)
		{
			case 'b':
				daemon_run=1;
				break;
			case 'p':
				serv_port = atoi(optarg);
				break;
			case 'h':
				print_usage(progname);
				return EXIT_SUCCESS;
			default:
				break;
		}
	}

	if( !serv_port)
	{
		print_usage(progname);
		return -1;
	}

	if((listenfd=socket_server_init(NULL,serv_port))<0)
	{
		printf("ERROR:%s server listen on port %d failure\n",argv[0],serv_port);
		return -2;
	}
	printf("%s server start to listen on port %d\n",argv[0],serv_port);

	/* set program running in backfround */
	if(daemon_run)
	{
		daemon(0,0);
	}

	for(i=0;i<ARRAY_SIZE(fds_array);i++)
		fds_array[i].fd=-1;
	fds_array[0].fd=listenfd;
	fds_array[0].events=POLLIN;

	max = 0;

	for( ; ; )
	{
		/* program will blocked here */
		rv = poll(fds_array,max+1,-1);
		if(rv < 0)
		{
			printf("select failure:%s\n",strerror(errno));
			break;
		}
		else if(rv == 0)
		{
			printf("select get timeout\n");
			continue;
		}

		/* listen socket get event means new client start connect now */
		if(fds_array[0].revents & POLLIN)
		{
			if((connfd = accept(listenfd,(struct sockaddr *)NULL,NULL)) < 0	)
			{
				printf("accept new client failure:%s\n",strerror(errno));
				continue;
			}

			found = 0;
			for(i=1;i<ARRAY_SIZE(fds_array);i++)
			{
				if( fds_array[i].fd <0)
				{
					printf("accept new client[%d] and add it into array\n",connfd);
					fds_array[i].fd = connfd;
					fds_array[i].events = POLLIN;
					found = 1;
					break;
				}
			}

			if( !found )
			{
				printf("accept new client[%d] but full,so refuse it\n",connfd);
				close(connfd);
				continue;
			}

			max = i>max ? i:max;
			if(--rv <= 0)
				continue;
		}

		else /* data arrive from already connected client */
		{
			for(i=1;i<ARRAY_SIZE(fds_array);i++)
			{
				if(fds_array[i].fd<0)
					continue;

				if((rv=read(fds_array[i].fd,buf,sizeof(buf)))<=0)
				{
					printf("Socket[%d] read failure and get disconnect.\n",fds_array[i].fd);
					close(fds_array[i].fd);
					fds_array[i].fd=-1;
				}
				else
				{
					printf("socket[%d] read get %d bytes data\n",fds_array[i].fd,rv);

					/* convert letter from lowercase to uppercase */
					for(j=0;j<rv;j++)
						buf[i]=toupper(buf[j]);

					if(write(fds_array[i].fd,buf,rv)<0)
					{
						printf("Socket[%d] write failure :%s\n",fds_array[i].fd,strerror(errno));
						close(fds_array[i].fd);
						fds_array[i].fd = -1;
					}
				}
			}
		}

	}

CleanUp:
	close(listenfd);
	return 0;
}

static inline void print_usage(char *progname)
{
	printf("Usage:%s [OPTION]...\n",progname);
	printf(" %s is a socket server program,which used to verify client and echo back string progname");
	printf("\nMandatory arguments to long options are mandatory for short options too:\n");
	printf(" -b[daemon] set program running on background\n");
	printf(" -p[port] socket server port address\n");
	printf(" -h[help] display this help information\n");

	printf("\nExample:%s -b -p 8900\n",progname);
	return ;
}

int socket_server_init(char *listen_ip,int listen_port)
{
	struct sockaddr_in          servaddr;
	int                         rv = 0;
	int                         on = 0;
	int                         listenfd;

	if( (listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
	{
		printf("Use socket() to vreate a TPC socket failure:%s\n",strerror(errno));
		return -1;
	}
	/* Set socket port reuseable,fix 'Address already in use'bug */
	setsockopt(listenfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port=htons(listen_port);

	if( !listen_ip )
		servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	else
	{
		if( inet_pton(AF_INET,listen_ip,&servaddr.sin_addr) <= 0)
		{
			printf("inet_pton() set listen IP address failure.\n");
			rv = -2;
			goto CleanUp;
		}
	}

	if(bind(listenfd,(struct sockaddr *)&servaddr,sizeof(servaddr)) < 0)
	{
		printf("Use bind() to the TCP socket failure:%s\n",strerror(errno));
	    rv = -3;
		goto CleanUp;
	}

	if(listen(listenfd,13)<0)
	{
	    printf("listen() to TCP failure:%s\n",strerror(errno));
		rv = -4;
		goto CleanUp;
	}
CleanUp:
    if(rv < 0)
	    close(listenfd);
	else
	    rv =listenfd;

	return rv;
}
