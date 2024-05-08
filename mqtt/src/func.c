/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  packdata.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 14:52:27"
 *                 
 ********************************************************************************/



#include "func.h"

void print_usage(char *progname)
{
	printf("%s:usage:\n",progname);
	printf ("%s is LingYun studio temperature CS program running on RaspberryPi\n", progname);
	printf("-i(--ipaddr) :specify server IP address.\n");
	printf("-p(--port)   :specify server port.\n");
	printf ("-t(--time)  :temp upload time.Default 60s\n");
	printf ("-d(--debug) :running in debug mode\n");
	printf("-h(--help)   :print this help information.\n");
	printf ("test:make successfully?\n");

	return ;
}

int  get_devid(char *id, int size, int sn)
{
	int          rv=0;

	if( !id || size<DEVICEID_LEN )
	{
		log_error ("Argument error:%s\n",strerror(errno));
		rv=-1;
	}

	memset(id, 0, size);
	snprintf(id, size, "DEVID%03d", sn);
	return rv;
}

int  get_time(char *time_buf, int size)
{
	time_t       timep;
	struct tm   *p;

	if( !time_buf || size<TIME_LEN )
	{
		log_error ("Argument error:%s\n",strerror(errno));
        return -1;
	}

	time(&timep);
	p=localtime(&timep);

	memset(time_buf,0,size);

	snprintf(time_buf, 128, "%d/%d/%d--%d:%d:%d", 1900+p->tm_year, 1+p->tm_mon,
			p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);

	log_debug ("%s\n",time_buf);
	return 0;
	//  strncpy(time_buf,asctime(gmtime(&timep)),sizeof(time_buf));
}
