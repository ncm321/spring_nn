/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  cli_sql.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 17:09:18"
 *                 
 ********************************************************************************/

#ifndef _DATABASE_H_
#define _DATABASE_H_

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
#include "func.h"
#include "logger.h"

#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG
#define dbg_print(format,args...) printf(format,##args)
#else
#define dbg_print(format,args...) do{} while(0)
#endif

#define MAX_SIZE 20
extern int        create_table(const char *db_file);
extern int        table_insert(data_s *all_data);
extern int        table_delete();
extern int        table_select();
extern int        determine_table_exist(char *tr_buf);
extern void       database_term(void);
static int        callback(void *data,int argc,char **argv,char **azColName);

#endif
