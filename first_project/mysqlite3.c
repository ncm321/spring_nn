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
int       create_table(sqlite3 *db,char *table_name);
int       table_delete(sqlite3 *db,char *table_name);
int       table_select(sqlite3 *db,char *table_name);
int       table_insert(sqlite3 *db,char *table_name,char *device_num,char *time,float *temp);
int       determine_table_exist(char *table_name,sqlite3 *db,char *rt_buf);
int main (int argc, char **argv)
{

	return 0;
} 


int create_table(sqlite3 *db,char *table_name)
{
//	sqlite3         *db;
	char            *zErrMsg = 0;
	int              rc;
	char             sql[512];

	/* Open database */
	/* Create SQL statement */
	snprintf(sql,512,"CREATE TABLE IF NOT EXISTS %s(sn TEXT NOT NULL,time TEXT NOT NULL,temp REAL NOT NULL)",table_name);

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
	printf ("debug:%s\n",table_name);
	return 0;
}
int table_insert(sqlite3 *db,char *table_name,char *device_num,char *time,float *temp)
{
//	sqlite3       *db;
	char          *zErrMsg = 0;
	int            rc;
	char           sql[512];
	/* create SQL statement */
	snprintf(sql,512,"INSERT INTO %s(sn,time,temp) VALUES ('%s','%s',%.2f)",table_name,device_num,time,*temp);
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

	return 0;
}

int table_delete(sqlite3 * db,char *table_name)
{
	char            *zErrMsg=NULL;
	int              rc;
	char             sql[128];
	const char*      data = "Callback function called";


	/* Create merged SQL statement */
//	sql = "DELETE from COMPANY where ID=2;SELECT * from COMPANY";
    snprintf(sql,128,"DELETE from %s where ROWID IN(SELECT ROWID from %s LIMIT 1)",table_name,table_name);
	/* Execute SQL statement */
	rc = sqlite3_exec(db,sql,0,0,&zErrMsg);
	if( rc!= SQLITE_OK )
	{
		fprintf(stderr,"SQL error:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		return rc=-2;
	}
	else
		fprintf(stdout,"Operation done successfully\n");
	return 0;
}

/* 回调函数，进一步处理sqlite1_exec函数的返回结果，并打印data参数传入的值 */
static int callback(void *data,int argc,char **argv,char **azColName)
{
	int i;
	fprintf(stderr,"%s:",(const char*)data);
	for(i=0;i<argc;i++)
		printf("%s=%s\t",azColName[i],argv[i] ? argv[i]:NULL);

	printf ("\n");
	return 0;
}
int table_select(sqlite3 *db,char *table_name)
{
	char           *zErrMsg=0;
	int             rc;
	char            sql[128];
	const char*     data="Callback function called";
	/* Open database */
     /*  Create merged SQL statement */ 
    snprintf(sql,128,"SELECT * from %s",table_name);
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
	return rc;
}
/* 更新数据 */

//判断表是否空
int determine_table_exist(char *table_name,sqlite3 *db,char *rt_buf)
{
	char            sendata_buffer2[128];
	int             rc = -1;
	char           *zErrMsg=0;
	char          **dbResult;
	int             nRow=0,nColumn=0;

	/* Open database */
	memset(sendata_buffer2,0,sizeof(sendata_buffer2));
	snprintf(sendata_buffer2,128,"SELECT * from %s",table_name);

	rc=sqlite3_get_table(db,sendata_buffer2,&dbResult,&nRow,&nColumn,&zErrMsg);
	if(rc!= SQLITE_OK)
	{
		printf ("Execute sqlite3_get_table failure:%s\n",zErrMsg);
		sqlite3_free(zErrMsg);
		exit(0);
	}
	memset(rt_buf,0,sizeof(rt_buf));
	//printf ("debug00:%s,%s\n",rt_buf,&rt_buf);
	printf("debug:%s\n",dbResult[0]);
	printf ("debug:%s\n",dbResult[1]);
	printf ("debug:%s\n",dbResult[2]);
	printf ("debug:%s\n",dbResult[3]);
	snprintf(rt_buf,128,"%s %s %.2f",dbResult[3],dbResult[4],atof(dbResult[5]));
    printf("debug01:%s\n",rt_buf);
	return nRow;
}
