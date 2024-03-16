#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <getopt.h>
#include <ctype.h>

void print_usage(char *progname)
{
	printf("%s usage:\n",progname);
	printf("-p(--port):sepcify server listen port.\n");
	printf("-h(--help):print this help information.\n");
	return ;
}

int main(int argc,char **argv)
{
	int                  cli_fd,listen_fd=-1;
	int                  rv = -1;
	int                  on = 1;
	int                  ch;
	int                  port = 0;
	struct sockaddr_in   servaddr;
	struct sockaddr_in   cliaddr;
	socklen_t            len = 0;
	pid_t                pid;

	struct  option       opts[] = {
		{"port",required_argument,NULL,'p'},
		{"help",no_argument,NULL,'h'},
		{NULL,0,NULL,0}
	};

	while( (ch=getopt_long(argc,argv,"p:h",opts,NULL))!= -1 )
	{
		switch(ch)
		{
			case 'p':
				port = atoi(optarg);
				break;
			case 'h':
				print_usage(argv[0]);
				return 0;
		}
	}

	if(!port)
	{
		print_usage(argv[0]);
		return 0;
	}

	listen_fd = socket(AF_INET,SOCK_STREAM,0);
	if(listen_fd <0 )
	{
		printf("Create socket failure:%s\n",strerror(errno));
		return -1;
	}
	printf("Create sockfd[%d] successfully!\n",listen_fd);

	setsockopt(listen_fd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);//listen all IP address on this host
	//inet_aton("192.168.2.40",&servaddr.sin_addr);// Only listen specify IP address
	
	if( (bind(listen_fd,(struct sockaddr *)&servaddr,sizeof(servaddr)))<0 )
	{
		printf("Sockfd[%d] bind on port[%d] failure:%s\n",listen_fd,port);
		return -2;
	}

	listen(listen_fd,13);
	printf("Start to listen on port [%d]\n",port);

	while(1)
	{
		printf("Start accept new client incoming...\n");

		cli_fd=accept(listen_fd,(struct sockaddr *)&cliaddr,&len);
		if(cli_fd<0)
		{
			printf("Accept new client failure:%s\n",strerror(errno));
			continue;
		}

		printf("Accept new client_fd[%s:%d] successfully!",inet_ntoa(cliaddr.sin_addr),ntohs(cliaddr.sin_port));

		pid = vfork();
		if(pid<0)
		{
			printf("Fork() create child process failure:%s\n",strerror(errno));
			return -3;
		}
		else if( 0==pid )
		{
			char       buf[1024];
			int        i;
			printf("Child process start to communicate with socket client...\n");
			close(listen_fd);

			while(1)
			{
				memset(buf,0,sizeof(buf));
				rv=read(cli_fd,buf,sizeof(buf));
				if(rv <0)
				{
					printf("Read data from client sockfd[%d] failure:%s\n",cli_fd,strerror(errno));
					close(cli_fd);
					exit(0);
				}
				else if( 0==rv )
				{
					printf("Client[%d] get disconnected\n",cli_fd);
					close(cli_fd);
					exit(0);
				}
				else
					printf("Read %d bytes data from server:%s\n",rv,buf);

				//convert letter from lowercase to uppercase
				for(i=0;i<rv;i++)
					buf[i]=toupper(buf[i]);

				rv =write(cli_fd,buf,rv);
				if(rv<0)
				{
					printf("Write to client by sockfd[%d] failure:%s\n",cli_fd,strerror(errno));
					close(cli_fd);
					exit(0);
				}//Child process loop
			}//Child process start
		}
		else
		{
			//Parent process close client fd and goes to accept new socket client again
			close(cli_fd);
			continue;
		}
		close(listen_fd);
		close(cli_fd);
		return 0;

	}

}
