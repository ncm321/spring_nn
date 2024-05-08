/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  ds18b20.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(09/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "09/04/24 19:12:39"
 *                 
 ********************************************************************************/


#ifndef  _DS18B20_H_
#define  _DS18B20_H_

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include "logger.h"

extern int get_temperature(float *temp);

#endif   
