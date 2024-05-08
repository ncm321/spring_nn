/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  cli_func.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 18:52:59"
 *                 
 ********************************************************************************/

#ifndef        _FUNC_H_
#define        _FUNC_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include "ds18b20.h"
#include "logger.h"
#include "pack.h"

#define        MAX_SIZE 20
#define        DEVICEID_LEN      16
#define        TIME_LEN          64


#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

typedef int  (* pack_func_t)(data_s *all_data, char *all_buf, int size);

extern int     get_devid(char *id, int size, int sn);
extern int     get_time(char *time_buf, int size);
extern void    print_usage(char *progname);
extern int     get_temperature(float *temp);

static inline void msleep(unsigned long ms)
{
	struct     timespec cSleep;
	unsigned long       ulTmp;     

	cSleep.tv_sec = ms / 1000;
	if( cSleep.tv_sec == 0 )
	{
        ulTmp = ms * 10000;
		cSleep.tv_nsec = ulTmp * 100;
	}
	else
	{
		cSleep.tv_nsec = 0;
	}

	nanosleep(&cSleep, 0);
	return ;
}

#endif
