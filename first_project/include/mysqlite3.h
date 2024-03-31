/********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  mysqlite3.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(23/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "23/03/24 15:21:46"
 *                 
 ********************************************************************************/
#ifndef _MYSQLITE3_H
#define _MYSQLITE3_H

#define MAX_SIZE 20
extern int        create_table(sqlite3 *db,char *table_name);
extern int        table_insert(sqlite3 *db,char *table_name,char *device_num,char *time,float * temp);
extern int        table_delete(sqlite3 *db,char *table_name);
extern int        table_select(sqlite3 *db,char *table_name);
extern int        determine_table_exist(char *table_name,sqlite3 *db,char *tr_buf);
static int               callback(void *data,int argc,char **argv,char **azColName);

#endif
