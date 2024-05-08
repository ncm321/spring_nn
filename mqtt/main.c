/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(25/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "25/04/24 20:16:47"
 *                 
 ********************************************************************************/

#include "main.h"

#define KEEPALIVE    60
#define PATH         "./aliyun.ini"
#define PROG_VERSION "1.0.0"


void connect_callback(struct mosquitto *mosq, void *obj, int rc);
int main (int argc, char **argv)
{
	int                    rv=-1;
	char                  *logfile = "mqtt.log";
	int                    loglevel = LOG_LEVEL_INFO;
	int                    logsize = 10;
	struct mosquitto      *mosq = NULL;
	mqtt_user_data_t       mqtt;
	float                  temp = 0;
	char                   devid[16];
	char                   time_buf[32];
	char                  *progname = basename(argv[0]);
	int                    brokerport = 1883;
	char                  *brokeraddress = NULL;
	char                  *clientId = NULL;
	char                  *username = NULL;
	char                  *password = NULL;
	char                  *topic = NULL;

	int                    opt = -1;


	if( (log_open(logfile, loglevel, logsize, THREAD_LOCK_NONE)) < 0 )
	{
		fprintf(stderr,"Initial log system failure\n");
		return -1;
	}
	/* 安装信号 */
	install_default_signal();

	/* 载入配置文件 */
	memset(&mqtt,0,sizeof(mqtt));
	rv = get_config(PATH,&mqtt,PUB);

	/* MQTT初始化 */
	if( (rv=mosquitto_lib_init()) < 0 )
	{
		log_error ("mosquitto init failure:%s", strerror(errno));
		goto clean;
	}

	/* 创建新的客户端 */
	mosq = mosquitto_new(mqtt.clientId, true, (void *)&mqtt);

	if(!mosq)
	{

		printf ("mosquitto_new() failed:%s\n",strerror(errno));
		goto clean;
		return -1;
	}

	if(mosquitto_username_pw_set(mosq, mqtt.username, mqtt.password)
			!= MOSQ_ERR_SUCCESS)
	{

		printf ("mosquitto_username_pw_set() failed:%s\n",strerror(errno));
		goto clean;
	}

	printf ("Create mosquitto successfully!\n");

	mosquitto_connect_callback_set(mosq,connect_callback);

	while(1)
	{
		if(mosquitto_connect(mosq, mqtt.brokeraddress, mqtt.brokerport, KEEPALIVE) 
				!= MOSQ_ERR_SUCCESS)
		{

			printf ("mosquitto_connect() failed:%s\n",strerror(errno));
			goto clean;
		}
		printf ("connect successfully!\n");

		/* 无阻塞，断线连接 */
		mosquitto_loop_forever(mosq,-1,1);
		sleep(3);
	}

clean:
	log_close();
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
} 

void connect_callback(struct mosquitto *mosq,void *obj,int rc)
{

	char                    ipaddr[16];
	char                    *interface="eth0";
	char                    datetime[64];
	cJSON                   *root;
	cJSON                   *item;
	char                    *msg;
	mqtt_user_data_t        *mqtt;

	char                    buf[2048];

	printf("Connection successful cJSON call packaging\n");

	float temper = 0.00000000;

	if(get_temperature(&temper) < 0)
	{
		printf("get_temperature failed.\n");
		return;
	}

	if(get_time(datetime,sizeof(datetime))<0)
	{
		printf("get_time failure\n");
		return ;
	}
/*  
	memset(ipaddr,0,sizeof(ipaddr));
	if(get_ipaddr(interface,ipaddr,sizeof(ipaddr))<0)
	{
		printf("ERROR:get ip address failure\n");
		return ;
	}
*/
	snprintf(buf,sizeof(buf),"%s/%f",datetime,temper);

	root = cJSON_CreateObject();
	item = cJSON_CreateObject();


	if(!obj)
	{
		printf("invalid_argument in %s\n",__FUNCTION__);
		return;
	}

	mqtt = (mqtt_user_data_t *)obj;

	cJSON_AddItemToObject(root,"method",cJSON_CreateString(mqtt->method));
	cJSON_AddItemToObject(root,"time",cJSON_CreateString(datetime));
	cJSON_AddItemToObject(root,"params",item);
	cJSON_AddItemToObject(item,"current_temp",cJSON_CreateNumber(temper));
	cJSON_AddItemToObject(root,"version",cJSON_CreateString(mqtt->version));

	msg = cJSON_Print(root);
	printf("%s\n",msg);


	if(!rc)
	{
		if(mosquitto_publish(mosq,NULL,mqtt->topic,strlen(msg),msg,mqtt->Qos,NULL) != MOSQ_ERR_SUCCESS)
		{
			printf("mosquitto_publish failed: %s\n",strerror(errno));
			return;
		}
	}

	mosquitto_disconnect(mosq);

}
