/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  sqlite3.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(22/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "22/03/24 20:02:41"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>
#include <string.h>
static int callback(void *data,int argc,char **argv,char **azColName);
int       create_table(sqlite3 *db);
int       table_insert(sqlite3 *db);
int       table_delete(sqlite3 *db);
int       table_select(sqlite3 *db);


int create_table(sqlite3 *db)
{
//	sqlite3         *db;
	char            *zErrMsg = 0;
	int              rc;
	char            *sql;

	/* Open database */
	rc = sqlite3_open("test.db",&db);
	if(rc)
	{
		fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
		return rc=-1;
		exit(0);
	}
	else
		fprintf(stdout,"Opened database successfully!\n");
	/* Create SQL statement */
	sql = "CREATE TABLE COMPANY(sn INT PRIMARY KEY NOT NULL,time TEXT NOT NULL,data INT NOT NULL)";

	/* Execute SQL statement */
	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);//第三个参数本来是回调函数，没有使用0
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Table created successfully\n");
	sqlite3_close(db);
	return 0;
}
int table_insert(sqlite3 *db)
{
//	sqlite3       *db;
	char          *zErrMsg = 0;
	int            rc;
	char          *sql;
	/* Open database */
	rc = sqlite3_open("test,db",&db);
	if( rc )
	{
		fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
		return rc=-1;
		exit(0);
	}
	else
		fprintf(stdout,"Opened database successfully\n");
	/* create SQL statement */
	sql = "INSERT INTO COMPANY(sn,time,data)" \
		  "VALUES (1,'2021',19);";//向表中插入数据
	/* Execute 	SQL statement */
	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
	if( rc != SQLITE_OK )
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Records created successfully\n");

	sqlite3_close(db);
	return 0;
}

int table_delete(sqlite3 * db)
{
	char            *zErrMsg=NULL;
	int              rc;
	char            *sql;
	const char*      data = "Callback function called";

	/* Open database */
	rc = sqlite3_open("test.db",&db);
	if(rc)
	{
		fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
		return rc=-1;
		exit(0);
	}
	else
		fprintf(stdout,"Opened database successfully\n");

	/* Create merged SQL statement */
	sql = "DELETE from COMPANY where ID=2;SELECT * from COMPANY";

	/* Execute SQL statement */
	rc = sqlite3_exec(db,sql,0,(void *)data,&zErrMsg);
	if( rc!= SQLITE_OK )
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Operation done successfully\n");
	sqlite3_close(db);
	return 0;
}

/* 回调函数，进一步处理sqlite1_exec函数的返回结果，并打印data参数传入的值 */
static int callback(void *data,int argc,char **argv,char **azColName)
{
	int i;
	fprintf(stderr,"%s:",(const char*)data);
	for(i=0;i<argc;i++)
		printf("%s=%s\n",azColName[i],argv[i] ? argv[i]:NULL);

	printf ("\n");
	return 0;
}
int table_select(sqlite3 *db)
{
	char           *zErrMsg=0;
	int             rc;
	char           *sql;
	const char*     data="Callback function called";
	/* Open database */
	rc = sqlite3_open("test.db",&db);
	if(rc)
    {
         fprintf(stderr,"Can't open database:%s\n",sqlite3_errmsg(db));
		 return rc=-1;
         exit(0);
	}
    else
         fprintf(stdout,"Opened database successfully\n");    
     /*  Create merged SQL statement */ 
	sql = "SELECT * from COMPANY";
	/* Execute SQL statement */
	rc = sqlite3_exec(db,sql,callback,(void*)data,&zErrMsg);
	if(rc != SQLITE_OK)
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Operation done successfully\n");
	sqlite3_close(db);
	return rc;
}
/* 更新数据 */

//判断表是否空
int determine_table_exist(char *table_name,sqlite3 *db)
{
	char            sendata_buffer2[100];
	int             rc = -1;
	char           *zErrMsg=0;
	char          **dbResult;
	int             nRow=0,nColumn=0;

	memset(sendata_buffer2,0,sizeof(sendata_buffer2));
	sprintf(sendata_buffer2,"select * from %s limit 1",table_name);

	rc=sqlite3_get_table(db,sendata_buffer2,&dbResult,&nRow,&nColumn,&zErrMsg);
	if(rc!= SQLITE_OK)
	{

		printf ("Execute sqlite3_get_table failure:%\n",zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
	return nRow;
}
