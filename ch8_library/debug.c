/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  debug.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(14/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "14/03/24 17:16:52"
 *                 
 ********************************************************************************/
#include <stdio.h>
#define CONFIG_DEBUG
#ifdef  CONFIG_DEBUG

#define  dbg_print(format,args...) printf(format,##args)
#else
#define  dbg_print(format,args...) do{} while(0)
#endif


int main (int argc, char **argv)
{
	dbg_print("Program runs here1\n");

	dbg_print("Program runs here2\n");

	dbg_print("Program runs here3\n");

	dbg_print("Program runs here4\n");
} 

