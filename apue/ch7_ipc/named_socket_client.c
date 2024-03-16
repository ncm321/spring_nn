/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  named_socket_client.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(29/02/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "29/02/24 21:34:59"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>

#define       SOCKET_PATH   "/tmp/socket.domain"
#define       MSG_STR       "Hello,Unix Domain Socket Server!"

int main(int argc,char **argv)
{
	int                conn_fd = -1;
	int                rv = -1;
	char               buf[1024];
	struct sockaddr_un serv_addr;

	conn_fd = socket(AF_UNIX,SOCK_STREAM,0);
	if(conn_fd<0)
	{
		printf("Create socket failure:%s\n",strerror(errno));
		return -1;
	}

	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sun_family=AF_UNIX;
	strncpy(serv_addr.sun_path,SOCKET_PATH,sizeof(serv_addr.sun_path)-1);

	if( connect(conn_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0 )
	{
		printf("Connect to unix domain socket server on \"%s\" failure:%s\n",SOCKET_PATH,strerror(errno));
		return 0;
	}
	printf("Connect to unix domain socket server on \'%s\" successfully!\n",SOCKET_PATH);

	if( write(conn_fd,MSG_STR,strlen(MSG_STR))<0)
	{
		printf("Write data to unix domain socket server on \"%s\" failure:%s\n",SOCKET_PATH,strerror(errno));
		goto cleanup;
	}

	memset(buf,0,sizeof(buf));
	if( (rv=read(conn_fd,buf,sizeof(buf))) <0 )
	{
		printf("Read data from server failure:%s\n",strerror(errno));
		goto cleanup;
	}
	else if(0 == rv)
	{
		printf("Client connect to server get disconnected\n");
		goto cleanup;
	}

	printf("Read %d bytes data from server :'%s'\n",rv,buf);

cleanup:
	close(conn_fd);
}
