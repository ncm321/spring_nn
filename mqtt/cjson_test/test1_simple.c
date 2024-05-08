/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  test1_simple.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(07/05/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "07/05/24 10:13:01"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
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
	cJSON_AddNullToObject(root, "money");
	cJSON_AddTrueToObject(root, "man");
	cJSON_AddFalseToObject(root, "adult");
	cJSON_AddNumberToObject(root, "years", 22);
	cJSON_AddStringToObject(root, "name", "fool");

	cJSON *season = cJSON_CreateStringArray(str_arry, 4);
	cJSON_AddItemToObject(root, "season", season);

    cJSON *child = cJSON_AddObjectToObject(root, "child");
    cJSON_AddStringToObject(child, "girlfriend", "june");

    char *json = cJSON_Print(root);
	printf ("%s\n",json);

	cJSON_Delete(root);
    return 0;
} 

