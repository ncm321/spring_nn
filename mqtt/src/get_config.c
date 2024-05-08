/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  get_config.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(01/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "01/05/24 13:44:42"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include "iniparser.h"
#include "dictionary.h"
#include "get_config.h"

#define DEFAULT_BROKER_ADDRESS   "iot-06z00g8y9se2n73.mqtt.iothub.aliyuncs.com"
#define DEFAULT_BROKER_PORT      1883
#define DEFAULT_BROKER_USERNAME  "rpi00001&k1674xOAlf4"
#define DEFAULT_BROKER_PASSWORD  "0dedd983b96eb32264db2269ef1b562183ac5529c1b31a2bcc5b983dc6fa74ed"
#define DEFAULT_BROKER_CLIENTID  " k1674xOAlf4.rpi00001|securemode=2,signmethod=hmacsha256,timestamp=1714135524937|"
#define DEFAULT_BROKER_JSONID    "current_temp"
#define DEFAULT_BROKER_SUB_TOPIC "/sys/k1674xOAlf4/rpi00001/thing/event/property/set"
#define DEFAULT_BROKER_PUB_TOPIC " /sys/k1674xOAlf4/rpi00001/thing/event/property/post"
#define DEFAULT_BROKER_METHOD    "thing.service.property.set"
#define DEFAULT_BROKER_VERSION   "1.0.0"
#define DEFAULT_BROKER_TIME      60

int get_config(char *path, mqtt_user_data_t *mqtt, int mode)
{
	dictionary     *ini = NULL;
	const char     *brokeraddress;
	const char     *username;
	const char     *password;
	const char     *clientId;
	const char     *topic;
	const char     *method;
	const char     *time;
	const char     *jsonid;
	const char     *identifier;
	const char     *version;
	int             brokerport = 0;
	int             Qos = 0;
	const char     *def = "error";
	int             notfound = 0;

	if( !path || !mqtt )
	{
		printf("invalid_argument:%s\n",__FUNCTION__);
		return -1;
	}
	ini = iniparser_load(path);

	if(!ini)
	{
		printf("iniparser_load failed!\n");
		return -1;
	}

	brokeraddress = iniparser_getstring(ini,"server_addr:host",DEFAULT_BROKER_ADDRESS);
	brokerport = iniparser_getint(ini,"server_addr:port", DEFAULT_BROKER_PORT);
	username = iniparser_getstring(ini, "user_name_pwd:user_name", DEFAULT_BROKER_USERNAME);
	password = iniparser_getstring(ini, "user_name_pwd:pwd", DEFAULT_BROKER_PASSWORD);
	clientId = iniparser_getstring(ini, "clientId:clientId", DEFAULT_BROKER_CLIENTID);
	identifier = iniparser_getstring(ini, "aliyun_json:identifier", DEFAULT_BROKER_JSONID);

	if(mode == SUB)
	{
		topic = iniparser_getstring(ini, "sub_topic:sub_topic", DEFAULT_BROKER_SUB_TOPIC);
	}
	else if(mode == PUB)
	{
		topic = iniparser_getstring(ini, "pub_topic:pub_topic", DEFAULT_BROKER_PUB_TOPIC);
		method = iniparser_getstring(ini,"aliyun_json:method", DEFAULT_BROKER_METHOD);
		version = iniparser_getstring(ini, "aliyun_json:version", DEFAULT_BROKER_VERSION);
	}

	else if( mode != SUB && mode != PUB)
	{
		printf("invalid_argument : %s mode!\n",__FUNCTION__);
		return -1;
	}

	strncpy(mqtt->brokeraddress, brokeraddress, SIZE);
	mqtt->brokerport = brokerport;
	mqtt->Qos = Qos;
	strncpy(mqtt->username, username, SIZE);
	strncpy(mqtt->password, password, SIZE);
	strncpy(mqtt->clientId, clientId, SIZE);
	strncpy(mqtt->topic, topic, SIZE);
	strncpy(mqtt->identifier, identifier, SIZE);

	if( mode == PUB)
	{
		strncpy(mqtt -> method, method, SIZE);
		strncpy(mqtt -> version, version, SIZE);
	}

    iniparser_freedict(ini);
	return 0;

}
