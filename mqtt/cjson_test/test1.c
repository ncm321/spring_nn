/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test1.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(06/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "06/05/24 15:00:32"
 *                 
 ********************************************************************************/

#include <string.h>
#include <stdio.h>
#include "cJSON.h"


int main (int argc, char **argv)
{
	const char *str_arry[4] = 
	{
		"spring",
		"summer",
		"fall",
		"winter"
	};

	cJSON *root = cJSON_CreateObject();
	cJSON *years = cJSON_CreateNumber(22);
	cJSON *name = cJSON_CreateString("fool");
	cJSON *man = cJSON_CreateTrue();
	cJSON *adult = cJSON_CreateFalse();
	cJSON *money = cJSON_CreateNull();
	cJSON *season = cJSON_CreateStringArray(str_arry, 4);
	cJSON *child = cJSON_CreateObject();
	cJSON *girlfriend = cJSON_CreateString("june");

	cJSON_AddItemToObject(root, "years", years);
	cJSON_AddItemToObject(root, "name", name);
	cJSON_AddItemToObject(root, "man", man);
	cJSON_AddItemToObject(root, "adult", adult);
	cJSON_AddItemToObject(root, "money", money);
	cJSON_AddItemToObject(root, "season", season);
	cJSON_AddItemToObject(child, "girlfriend", girlfriend);
	cJSON_AddItemToObject(root, "child", child);

	char *json=cJSON_Print(root);

	printf("%s\n",json);


	//release!
	cJSON_Delete(root);


    return 0;
} 




