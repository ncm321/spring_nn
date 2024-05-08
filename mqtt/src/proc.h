/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  proc.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(16/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "16/04/24 16:31:53"
 *                 
 ********************************************************************************/

#ifndef __PROC_H_
#define __PROC_H_ 


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <libgen.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <time.h>
#include "logger.h"

extern int   g_sigstop;

extern void proc_default_sighandler(int sig);
extern void install_default_signal(void);

#endif
