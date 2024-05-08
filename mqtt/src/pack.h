/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  pack.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(25/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "25/04/24 19:52:05"
 *                 
 ********************************************************************************/

#ifndef        _CLI_FUNC_H_
#define        _CLI_FUNC_H_

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

#define        MAX_SIZE 20
#define        DEVICEID_LEN      16
#define        TIME_LEN          64

typedef struct    data_pack{
	char      devid[DEVICEID_LEN];
	char      sample_time[TIME_LEN];
	float     temp;
}data_s;

#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

typedef int    (* pack_func_t)(data_s *all_data, char *all_buf, int size);
extern int     pack_data(data_s *all_data, char *all_buf, int size);
extern int     pack_data_json(data_s *all_data, char *all_buf, int size);

#endif
