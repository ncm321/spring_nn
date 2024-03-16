/*********************************************************************************
 *      Copyright:  (C) 2024 lingyun<lingyun>
 *                  All rights reserved.
 *
 *       Filename:  sig.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(30/01/24)
 *         Author:  lingyun <lingyun>
 *      ChangeLog:  1, Release initial version on "30/01/24 14:04:36"
 *                 
 ********************************************************************************/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int g_stop = 0;
void sig_handle(int signum)
{
	printf("Catch signal [%d]\n",signum);
	g_stop = 1;
}
int main(void)
{

	signal(SIGINT,sig_handle);
	signal(SIGTERM,sig_handle);

	while( !g_stop )
	{
		      ;
	}

	printf("power off gprs\n");
}

