/*********************************************************************************
 *      Copyright:  (C) 2024 LiYi<1751425323@qq.com>
 *                  All rights reserved.
 *
 *       Filename:  proc.c
 *    Description:  This file 
 *                 
 *        Version:  1.0.0(16/04/24)
 *         Author:  LiYi <1751425323@qq.com>
 *      ChangeLog:  1, Release initial version on "16/04/24 15:40:37"
 *                 
 ********************************************************************************/


#include "proc.h"

int g_sigstop = 0;
void proc_default_sighandler(int sig)
{
	switch(sig)
	{
		case SIGINT:
			log_warn("SIGINT - stopping\n");
			g_sigstop = 1;
			break;

		case SIGTERM:
			log_warn("SIGTERM - stopping\n");
			g_sigstop = 1;
			break;

		case SIGSEGV:
			log_warn("SIGSEGV - stopping\n");
#if 0
			if(g_sigstop)
				exit(0);

			g_sigstop = 1;
#endif
			break;

		case SIGPIPE:
			log_warn("SIGPIPE - warning\n");
			break;

		default:
			break;
	}
}

void install_default_signal(void)
{
	struct sigaction   sigact,sigign;

	log_info("Install default signal handler.\n");

	/* Initialize the catch signal structure. */
	sigemptyset( &sigign.sa_mask);
	sigact.sa_flags = 0;
	sigign.sa_handler = SIG_IGN;

	sigaction(SIGTERM, &sigact, 0);
	sigaction(SIGINT, &sigact, 0);
//	sigaction(SIGSEGV, &sigact, 0);
	sigaction(SIGPIPE, &sigact, 0);
}

