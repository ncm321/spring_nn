/*********************************************************************************
 *      Copyright:  (C) 2024 lingyun<lingyun>
 *                  All rights reserved.
 *
 *       Filename:  daemon.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(01/02/24)
 *         Author:  lingyun <lingyun>
 *      ChangeLog:  1, Release initial version on "01/02/24 11:37:34"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <syslog.h>
#include <libgen.h>
int main(int argc,char **argv)
{
	char        *progname = basename(argv[0]);

	if( daemon(0,0) < 0)
	{
		printf("Program daemon() failure:%s\n",strerror(errno));
		return -1;
	}
	openlog("daemon",LOG_CONS | LOG_PID,0);
	syslog(LOG_NOTICE,"Program '%s' start running\n",progname);
	syslog(LOG_WARNING,"Program '%s' running with a warnning message\n",progname);
	syslog(LOG_EMERG,"Program '%s' running with a emergency message\n",progname);
    syslog(LOG_ALERT,"Program '%s' running with a alert message\n",progname);
	while(1)
	{
		//Do Something here
		;
	}

	syslog(LOG_NOTICE,"Program '%s' stop running\n",progname);
	closelog();

	return 0;
}
