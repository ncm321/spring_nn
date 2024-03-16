#include<stdio.h>

int main(int argc, char **argv)
{
	int var = 0x12345678;
	printf("var address: %p or 0x%lx\n",&var,(unsigned long)&var);
	return 0;
}
