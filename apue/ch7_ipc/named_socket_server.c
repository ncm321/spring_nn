/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  named_socket_server.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(29/02/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "29/02/24 20:28:33"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <ctype.h>

#define    SOCKET_PATH       "/tmp/socket.domain"

int main(int argc,char **argv)
{
	int                    rv=-1;
	int                    i=0;
	int                    listen_fd,client_fd = -1;
	struct sockaddr_un     serv_addr;
	struct sockaddr_un     cli_addr;
	socklen_t              cliaddr_len;
	char                   buf[1024];

	//listen_fd=socket(AF_UNIX,SOCK_STREAM,0);
	listen_fd=socket(AF_LOCAL,SOCK_STREAM,0);
	if(listen_fd<0)
	{
		printf("Create socket failure:%s\n",strerror(errno));
		return -1;
	}
	printf("socket create fd[%d]\n",listen_fd);

	
	if( !access(SOCKET_PATH,F_OK) )
	{
		unlink(SOCKET_PATH);
	}
/* 	if( access(SOCKET_PATH,F_OK) )
	{
		printf("File path \"%s\" not exist and create it now\n",SOCKET_PATH);
		mkdir(SOCKET_PATH,0777);
	}
*/
	memset(&serv_addr,0,sizeof(serv_addr));
	serv_addr.sun_family=AF_UNIX;
	strncpy(serv_addr.sun_path,SOCKET_PATH,sizeof(serv_addr.sun_path)-1);

	if( bind(listen_fd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) <0 )
	{
		printf("bind socket failure:%s\n",strerror(errno));
//		close(listen_fd);
        printf("Debug:listen_fd:%d\n",listen_fd);
		printf("%d\n",sizeof(serv_addr));
		printf("%s\n",serv_addr.sun_path);
		printf("%d\n",serv_addr.sun_family);
		unlink(SOCKET_PATH);
		return -2;
	}
	printf("socket[%d] bind on path \"%s\" successfully!\n",listen_fd,SOCKET_PATH);

	listen(listen_fd,13);

	while(1)
	{
		printf("\n%d start waiting and accept new client connect...\n",listen_fd);
     // cliaddr_len=sizeof(cli_addr);
		client_fd = accept(listen_fd,(struct sockaddr*)&cli_addr,&cliaddr_len);
    	if(client_fd < 0)
		{
			printf("accept client failure:%s\n",strerror(errno));
			return -2;
		}

		memset(buf,0,sizeof(buf));
		if( (rv=read(client_fd,buf,sizeof(buf))) <0 )
		{
			printf("Read data from client[%d] failure:%s\n",client_fd,strerror(errno));
			close(client_fd);
			continue;
		}
		else if(0 == rv)
		{
			printf("Client socket[%d] disconnected\n",client_fd);
			close(client_fd);
			continue;
		}
		printf("Read %d bytes data from client[%d] and echo it back:'%s'\n",rv,client_fd,buf);

		for(i=0;i<=rv;i++)
		{
			buf[i]=toupper(buf[i]);
		}

		if( (write(client_fd,buf,rv)) <0 )
		{
			printf("Write to client socket[%d] failure:%s\n",client_fd,strerror(errno));
			close(client_fd);
		}

		sleep(1);
		close(client_fd);
	}

	close(listen_fd);
}
