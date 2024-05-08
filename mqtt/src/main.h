/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  main.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(25/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "25/04/24 20:28:51"
 *                 
 ********************************************************************************/

#ifndef  _MAIN_H_
#define  _MAIN_H_

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "ds18b20.h"
#include "pack.h"
#include "database.h"
#include "logger.h"
#include "func.h"
#include "cJSON.h"
#include "get_config.h"
#include "iniparser.h"
#include "proc.h"
#include "dictionary.h"
#include "mosquitto.h"

#define  KEEPALIVE   60


#endif
