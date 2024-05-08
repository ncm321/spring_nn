/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test_print.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(07/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "07/05/24 11:00:56"
 *                 
 ********************************************************************************/


#include <stdio.h>
#include <string.h>
#include "cJSON.h"


int main (int argc, char **argv)
{

	char *json = "{\"years\":22,\"name\":\"fool\",\"man\":true,\"adult\":false,\"season\":[\"spring\",\"summer\",\"fall\",\"winter\"],\"money\":null,\"child\":{\"girlfriend\":\"june\",\"boyfriend\":null}}";
	cJSON *root = cJSON_Parse(json);

	cJSON *item = cJSON_GetObjectItem(root, "years");
	int years = years->valueint;
	printf("years=%d \r\n", years);

	item = cJSON_GetObjectItem(root, "name");
	char *name = cJSON_GetStringValue(item);
	printf("name=%s \r\n", name);

	item = cJSON_GetObjectItem(root, "man");
	int man = item->type;
	printf("man=%d \r\n", man);

	item = cJSON_GetObjectItem(root, "adult");
	int adult = item->type;
	printf("adutl=%d \r\n", adult);


	item = cJSON_GetObjectItem(root, "season");
	int arry_size = cJSON_GetArraySize(item);

	for (int i = 0; i < arry_size;i++)
	{

		char *season = cJSON_GetStringValue(cJSON_GetArrayItem(item, i));
		printf("season[%d]=%s \r\n",i, season);
	}

	item = cJSON_GetObjectItem(root, "money");
	int money = item->type;
	printf("money=%d \r\n", money);


	cJSON *child = cJSON_GetObjectItem(root, "child");
	item = cJSON_GetObjectItem(child, "girlfriend");
	char *girlfriend = cJSON_GetStringValue(item);
	printf("girlfriend=%s \r\n", girlfriend);

	item = cJSON_GetObjectItem(child, "boyfriend");
	int boyfriend = item->type;
	printf("boyfriend=%d \r\n", boyfriend);


	cJSON_Delete(root);

	return 0;
} 

