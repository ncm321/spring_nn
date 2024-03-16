#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>

#define MSG_STR        "Hello LingYun IoT Studio\n"
int main(int argc,char **argv)
{
	int                     sockfd = -1;
	int                     rv = -1;
	struct sockaddr_in      servaddr;
	int                     port;
	char                    *servip;
	char                    buf[1024];

	printf("1112\n");
	if(argc < 3)
	{
		printf("Progeam usage:%s [ServerIP] [Prot]\n",argv[0]);
		return -1;
	}

	servip = argv[1];
	port = atoi(argv[2]);
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd < 0)
	{
		printf("111");
		printf("Create socket failure:%s\n",strerror(errno));
		return -1;
	}
	printf("Create socket[%d] successfully!\n",sockfd);

	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	inet_aton(servip,&servaddr.sin_addr);

	rv = connect(sockfd,(struct sockaddr *)&servaddr,sizeof(servaddr));
	if(rv < 0)
	{
		printf("Connect to server[%s:%d] failure:%s\n",servip,port,strerror(errno));
		return -2;
	}
	printf("Connect to server[%s:%d] successfully!\n",servip,port);

	rv = write(sockfd,MSG_STR,strlen(MSG_STR));
	if(rv < 0)
	{
		printf("Write data to server by sockfd[%d] failure:%s\n",sockfd,strerror(errno));
		return -3;
	}

	memset(buf,0,sizeof(buf));
	rv = read(sockfd,buf,sizeof(buf));
	if(rv < 0)
	{
		printf("Read data from server by sockfd[%d] failure:%s\n",sockfd,strerror(errno));
		return -4;
	}
	else if(0 == rv)
	{
		printf("Sockfd[%d] get disconnected\n",sockfd);
		return -5;
	}
	else if(rv > 0)
		printf("Read %d bytes data from server:%s\n",rv,buf);

	close(sockfd);
	return 0;





}

