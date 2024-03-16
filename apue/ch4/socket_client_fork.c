#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdlib.h>

#define MSG           "Hello,I am client for child process."

void print_usage(char *progname)
{
	printf("%s usage:\n",progname);
	printf("-i(--ipaddr):specify server IP address.\n");
	printf("-p(--port):specify server port.\n");
	printf("-h(--help):print this help information.\n");
	return ;
}

int main(int argc,char **argv)
{
	int                 conn_fd=-1;
	int                 rv =-1;
	int                 ch;
	int                 port;
	char               *servip;
	char                buf[1024];
	struct sockaddr_in  serv_addr;
	struct option       opts[]={
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while((ch=getopt_long(argc,argv,"i:p:h",opts,NULL))!=-1)
	{
		switch(ch)
		{
			case 'i':
				servip=optarg;
				break;
			case 'p':
				port=atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;

		}
	}
	if( !servip || !port)
	{
		print_usage(argv[0]);
		return 0;
	}

	conn_fd=socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd<0)
	{
		printf("Create sockfd failure:%s\n",strerror(errno));
		return -1;
	}
	printf("Create sockfd[%d] successfully!\n",conn_fd);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family =AF_INET;
	serv_addr.sin_port = htons(port);
	inet_aton(servip,&serv_addr.sin_addr);

	if(connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0)
	{
                printf("Connect to server [%s:%d] failure:%s\n",servip,port,strerror(errno));
		goto cleanup;
	}
	printf("Connect to server[%s:%d] successfully!\n",servip,port);
	rv = write(conn_fd,MSG,strlen(MSG));
	if(rv <0)
	{
		printf("Write data to server by client[%d] failure:%s\n",conn_fd,strerror(errno));
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));
	rv = read(conn_fd,buf,sizeof(buf));
	if(rv <0)
	{
		printf("Read data from server failure:%s\n",strerror(errno));
		goto cleanup;
	}
	else if(rv == 0)
	{
		printf("Client connect to server get disconnected.\n");
		goto cleanup;
	}
	printf("Read %d bytes data from server:'%s'\n",rv,buf);

cleanup:
	close(conn_fd);
}
