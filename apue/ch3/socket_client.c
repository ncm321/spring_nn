#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <getopt.h>
#include <stdlib.h>

#define SERVER_IP         "192.168.2.40"
#define SERVER_PORT       8889
#define MSG_STR           "Hello,Unix Network Program World!"

void print_usage(char *progname)
{
	printf("%s usage:\n",progname);
	printf("-i(--ipaddr):specify server IP address.\n");
	printf("-p(--port):specify server Port.\n");
	printf("-h(--help):print this help information.\n");

	return ;
}

int main(int argc, char **argv)
{
	int                   conn_fd = -1;
	int                   rv = -1;
	int                   ch;
	int                   port;
	int                   opt=1;
        char                  *servip;	
	char                  buf[1024];
	struct sockaddr_in    serv_addr;
	struct option         opts[] = {
		{"ipaddr",required_argument,NULL,'i'},
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

//	servip = argv[1];
//	port   = atoi(argv[2]);

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

	if(!servip || !port)
	{
		print_usage(argv[0]);
		return 0;
	}

	conn_fd = socket(AF_INET,SOCK_STREAM,0);
	if(conn_fd < 0)
	{
		printf("create socket failure:%s\n",strerror(errno));
		return -1;
	}
	printf("create socket[%d] successfully!\n",conn_fd);

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);
	inet_aton( servip,&serv_addr.sin_addr );

	if( connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
	{
		printf("connect to server [%s:%d] failure:%s\n",SERVER_IP,SERVER_PORT,strerror(errno));
		goto cleanup;
	}
	printf("Connect to server [%s:%d] successfully!\n",servip,port);

//	setsockopt(conn_fd,SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
	rv = write(conn_fd,MSG_STR,strlen(MSG_STR));
	if(rv < 0)
	{
		printf("Write to server by client[%d] failure:%s\n",conn_fd,strerror(errno));
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));
	rv = read(conn_fd,buf,sizeof(buf));
	if(rv <0)
	{
		printf("Read data from server failure:%s\n",strerror(errno));
		goto cleanup;
	}

	else if(0 == rv)
	{
		printf("Client connect to server get disconnected:%s\n",strerror(errno));
		goto cleanup;
	}

	printf("Read %d bytes data from server:'%s'\n",rv,buf);

cleanup:
	close(conn_fd);
}
