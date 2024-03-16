/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  main.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "11/03/24 21:55:19"
 *                 
 ********************************************************************************/


#include <stdio.h>

#include <string.h>

#include "crypto.h"

int main (int argc, char **argv)
{
	char         *message="abcdefghijklmnopq";
	char          ciphertext[64];
	char          plaintext[64];

	memset(ciphertext,0,sizeof(ciphertext));
	if(encrypt(message,ciphertext,sizeof(ciphertext))<0)
	{
		printf("encrypt plaintext failure\n");
		return -1;
	}
	
	printf ("encrypt ciphertext:%s\n",ciphertext);

	memset(plaintext,0,sizeof(plaintext));
	if(decrypt(ciphertext,plaintext,sizeof(plaintext))<0)
	{

		printf ("decrypt ciphertext failure\n");
		return -1;
	}

	printf ("decrypt plaintext:%s\n",plaintext);
	return 0;
} 

