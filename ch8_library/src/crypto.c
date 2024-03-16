/*********************************************************************************
 *      Copyright:  (C) 2024 linuxer<linuxer@email.com>
 *                  All rights reserved.
 *
 *       Filename:  crypto.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(11/03/24)
 *         Author:  linuxer <linuxer@email.com>
 *      ChangeLog:  1, Release initial version on "11/03/24 21:27:27"
 *                 
 ********************************************************************************/

#include <stdio.h>

#include <string.h>

int encrypt (char *plaintext,char *ciphertext,int size)
{
	int          i;

	if(size<strlen(plaintext))
		return -1;

	for(i=0;i<strlen(plaintext);i++)
		ciphertext[i]=plaintext[i]+3;
	return 0;
} 


int decrypt (char *ciphertext,char *plaintext,int size)
{
	int        i;
	if(size<strlen(ciphertext))
		return -1;
	for(i=0;i<strlen(ciphertext);i++)
		plaintext[i]=ciphertext[i]-3;

	return 0;
} 


