#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int g_var1;            //未初始化全局变量，数据段BBS区，默认值为0
int g_var2=20;         //初始化的全局变量，数据段DATA区，值为20


int main(int argc,char **argv)  //argv 存放的是命令行参数，存放在命令行参数区
{
	static int s_var1;      //未初始化的静态变量，数据段BBS区，默认值0
	static int s_var2=10;   //初始化了的静态变量，数据段DATA区，值为20

	char      *str="Hello"; //初始化局部变量，栈中，值“Hello”在DATA段的RODATA区中的地址
	char      *ptr;         //未初始化的局部变量，栈中，随机值也就是野指针
	ptr = malloc(100);      //malloc()从堆（HEAP）中分配100个字节的内存空间，并将该内存空间的首地址返回给ptr存放

	printf("[cmd args]: argv address:%p\n",argv);
	printf("\n");

	printf("[Stack]: str address:%p\n",&str);
	printf("[Stack]: ptr address:%p\n",&ptr);
	printf("\n");

	printf("[ Heap ]: ptr address:%p\n",ptr);
	printf("\n");

	printf("[ bss ]: s_var1 address:%p value:%d\n",&s_var1,s_var1);
	printf("[ bss ]: g_var1 address:%p value:%d\n",&g_var1,g_var1);

	printf("[ data ]:s_var2 address:%p value:%d\n",&s_var2,s_var2);
	printf("[ data ]:g_var2 address:%p value:%d\n",&g_var2,g_var2);

	printf("[rodata]:\"%s\" address:%p \n",str,str);
	printf("\n");

	printf("[text]:main() address:%p\n",main);
	printf("\n");

	free(ptr);
	return 0;
}
