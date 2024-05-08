/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test_connext_aliyun.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(26/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "26/04/24 21:36:37"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "mosquitto.h"

#define   SESSION_NO   FALSE 
#define   SESSION_ON   TRUE
typedef struct   s_conn_prof{
	char     *hostname;
	char     *pub_topic;
//	char     *sub_topic;
	char     *clientId;
	char     *username;
	char     *password;
	int       port;
	char     *payload;
	int       keepalive;
	int       qos;
}conn_prof_t;


int main (int argc, char **argv)
{
	struct mosquitto    *mosq;
    int                  mid,mid1;
	int                  retain = 0;
	int                  session = true;

	conn_prof_t    rpi1_prof = {
		"iot-06z00g8y9se2n73.mqtt.iothub.aliyuncs.com",
		"/sys/k1674xOAlf4/rpi00001/thing/event/property/post",
		"k1674xOAlf4.rpi00001|securemode=2,signmethod=hmacsha256timestamp=1714135524937|",
		"rpi00001&k1674xOAlf4",
		"0dedd983b96eb32264db2269ef1b562183ac5529c1b31a2bcc5b983dc6fa74ed",
		1883,
		"{\"params\":{\"current_temp\":30.22}}",
		60,
		0
	};

	mosquitto_lib_init();

	mosq = mosquitto_new(rpi1_prof.clientId, session, NULL);

	if( (mosquitto_username_pw_set(mosq, rpi1_prof.clientId, rpi1_prof.password) 
				!= MOSQ_ERR_SUCCESS) )
	{

		printf ("Connect to aliyun failure:%s\n",strerror(errno));
	}
	else{

   		printf ("mqtt connect successfully.\n");
	}
	if (mosquitto_connect(mosq, rpi1_prof.hostname, rpi1_prof.port,
				rpi1_prof.keepalive) != MOSQ_ERR_SUCCESS)
	{
		
		printf ("Connect to aliyun failure:%s\n",strerror(errno));

	}
	printf ("Connect ok!\n");

	if (mosquitto_loop_start(mosq) != MOSQ_ERR_SUCCESS)
	{

		printf ("mosquitto_loop_start failure:%s\n",strerror(errno));
	}

	/* ---------sub message callback ----------? */

	while(1)
	{
		if (mosquitto_publish(mosq, &mid, rpi1_prof.pub_topic, strlen(rpi1_prof.
						payload), rpi1_prof.payload, rpi1_prof.qos, retain) 
				!= MOSQ_ERR_SUCCESS)
		{

			printf ("mosquitto publish failure:%s\n", strerror(errno));
			continue;
		}
		printf ("mosquitto publish data successfully!\n");
		sleep(3);
	}


clean:
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();
	
	return 0;

} 

