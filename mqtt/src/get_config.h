/********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_config.h
 *    Description:  This file 
 *
 *        Version:  1.0.0(01/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "01/05/24 13:26:44"
 *                 
 ********************************************************************************/

#ifndef  __GET_CONFIG_H_
#define  __GET_CONFIG_H_

#define   SIZE         1024
#define   KEEP_ALIVE   60

enum{
	SUB,
	PUB
};

typedef struct mqtt_user_data_s{
	char    brokeraddress[SIZE];
	int     brokerport;
	char    username[SIZE];
	char    password[SIZE];
	char    clientId[SIZE];
	char    topic[SIZE];
	int     Qos;

	char    method[SIZE];
	char    time[SIZE];
	char    jsonid[SIZE];
	char    identifier[SIZE];
	char    version[SIZE];


}mqtt_user_data_t;


int   get_config(char *path, mqtt_user_data_t *mqtt, int mode);
#endif
