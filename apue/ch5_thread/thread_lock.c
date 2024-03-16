/*********************************************************************************
 *      Copyright:  (C) 2024 lingyun<lingyun>
 *                  All rights reserved.
 *
 *       Filename:  thread_lock.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(05/02/24)
 *         Author:  lingyun <lingyun>
 *      ChangeLog:  1, Release initial version on "05/02/24 13:24:42"
 *                 
 ********************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>



void    *thread_worker1(void *args);
void    *thread_worker2(void *args);

typedef struct worker_ctx_s
{
	int              shared_var;
	pthread_mutex_t  lock;
}worker_ctx_t;

int main(int argc,char **argv)
{
	worker_ctx_t        worker_ctx;
	pthread_t           tid;
	pthread_attr_t      thread_attr;

	worker_ctx.shared_car = 1000;
	pthread_mutex_init(&worker_ctx.lock,NULL);

}
