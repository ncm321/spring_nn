#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	char           cmd_buf[256];
	int            count = 4;
	char          *interface="eth0";
	char          *dst_ip = "4.2.2.2";

	snprintf(cmd_buf,sizeof(cmd_buf),"ping -c %d -I %s %s",count,interface,dst_ip);
	system(cmd_buf);
}