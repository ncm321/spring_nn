/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  database.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(02/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "02/04/24 14:13:59"
 *                 
 ********************************************************************************/

#include "database.h"

#define  TABLE_NAME    "ClientTable"
static    sqlite3       *s_clidb = NULL;

int create_table(const char *db_file)
{
	char            *zErrMsg = 0;
	int              rc;
	char             sql[512];

	/*  Open database */
	if( SQLITE_OK != sqlite3_open(db_file,&s_clidb) )
	{
		log_error ("Open database file '%s' failure\n",db_file);
		return -2;
	}
	else
	{
		log_info ("Open database '%s' successfully!\n",db_file);
	}


	/*  Create SQL statement */
	snprintf(sql,512,"CREATE TABLE IF NOT EXISTS %s(sn TEXT NOT NULL,time TEXT NOT NULL,temp REAL NOT NULL)",TABLE_NAME);

	/*  Execute SQL statement */
	rc = sqlite3_exec(s_clidb,sql,0,0,&zErrMsg);//第三个参数本来是回调函数，没有使用0
	if(rc != SQLITE_OK)
	{
		log_error ("SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		database_term();
		return rc=-2;
	}
	else
		log_info ("Table created successfully\n");
	return 0;
}


int table_insert(data_s *all_data)
{
	char          *zErrMsg = 0;
	int            rc;
	char           sql[512];

	if( !all_data )
	{
		log_error("%s() Invaild input arguments\n",__func__);
		return -1;
	}

	if( !s_clidb )
	{
		log_error ("SQL database not opened!\n");
		return -2;
	}

	/*  create SQL statement */
	snprintf(sql,512,"INSERT INTO %s(sn,time,temp) VALUES ('%s','%s',%.2f)",
			TABLE_NAME,all_data->devid,all_data->sample_time,all_data->temp);

	/*  Execute  SQL statement */
	rc = sqlite3_exec(s_clidb, sql, 0, 0, &zErrMsg);
	if( rc != SQLITE_OK )
	{
		log_error ("SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		log_info ("Records created successfully\n");

	return 0;
}

int table_delete(void)
{
	char            *zErrMsg=NULL;
	int              rc;
	char             sql[128];
	const char*      data = "Callback function called";


	/*  Create merged SQL statement */
	snprintf(sql,128,"DELETE from %s where ROWID IN(SELECT ROWID from %s LIMIT 1)",
			TABLE_NAME,TABLE_NAME);

	/*  Execute SQL statement */
	rc = sqlite3_exec(s_clidb,sql,0,0,&zErrMsg);
	if( rc!= SQLITE_OK )
	{
		log_error ("SQL(delete) error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		log_info ("Delete a row data successfully\n");
	return 0;
}


static int callback(void *data,int argc,char **argv,char **azColName)
{
	int i;
	log_error ("%s:",(const char*)data);
	for(i=0;i<argc;i++)
		log_info ("%s=%s\t",azColName[i],argv[i] ? argv[i]:NULL);

	log_info ("\n");
	return 0;
}


int table_select(void)
{
	char           *zErrMsg=0;
	int             rc;
	char            sql[128];
	const char*     data="Callback function called";

	/*   Create merged SQL statement */
	snprintf(sql,128,"SELECT * from %s",TABLE_NAME);
	/*  Execute SQL statement */
	rc = sqlite3_exec(s_clidb,sql,callback,(void*)data,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		log_error ("SQL(select) error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		log_info ("Select all datas successfully\n");
	return rc;
}


int determine_table_exist(char *rt_buf)
{
	char            sendata_buffer2[128];
	int             rc = -1;
	char           *zErrMsg=0;
	char          **dbResult;
	int             nRow=0,nColumn=0;

	memset(sendata_buffer2,0,sizeof(sendata_buffer2));
	snprintf(sendata_buffer2,128,"SELECT * from %s",TABLE_NAME);

	rc=sqlite3_get_table(s_clidb,sendata_buffer2,&dbResult,&nRow,&nColumn,&zErrMsg);
	if(rc!= SQLITE_OK)
	{
		log_error ("Execute sqlite3_get_table failure:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		database_term();
		exit(0);
	}

	memset(rt_buf,0,128);
	if( 0 != nRow)
	{
    	snprintf(rt_buf,128,"%s %s %.2f",dbResult[3],dbResult[4],atof(dbResult[5]));
    	log_info ("%s,%s,%s\n",dbResult[3],dbResult[4],dbResult[5]);
     	log_debug ("debug01:%s\n",rt_buf);
	}

	sqlite3_free_table(dbResult);
	return nRow;
}


void   database_term(void)
{
	sqlite3_close(s_clidb);
	return ;
}
