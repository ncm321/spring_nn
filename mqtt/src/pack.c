/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  pack.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(25/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "25/04/24 19:54:52"
 *                 
 ********************************************************************************/

#include "pack.h"

int   pack_data(data_s *all_data, char *all_buf, int size)
{
	if( !all_data || !all_buf || size<=0 )
	{

		printf ("Invalid input arguments\n");
		return-1;
	}

	memset(all_buf, 0, size);

	snprintf(all_buf, size, "%s %s %.2f", all_data->devid,
			all_data->sample_time, all_data->temp);

	return strlen(all_buf);
}


int    pack_data_json(data_s *all_data, char *all_buf, int size)
{
	if( !all_data || !all_buf || size<=0 )
	{
		printf ("Invaild input arguments\n");
		return -1;
	}

	memset(all_buf, 0, size);
	snprintf(all_buf, size, "{\"device_id\":\"%s\",\"time\":\"%s\",\"temp\":\"%.2f\"}",
			all_data->devid, all_data->sample_time, all_data->temp);

	return strlen(all_buf);
}
