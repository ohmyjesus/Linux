#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	char *val;
	const char *name="ABD";

	val = getenv(name);    //没有name这个环境变量名 所以返回NULL
	printf("1, %s = %s\n", name ,val);  //  1,  ABD=NULL

	setenv(name,"haha-day-and-night",1);//设置name的环境变量值

	val=getenv(name);	//val=haha-day-and-night
	printf("2, %s = %s\n", name,val);    //2, ABD=haha-day-and-night

	int ret = unsetenv("ABCD");
	printf("ret= %d\n",ret);   //ret=0
	
	val=getenv(name);   
	printf("3, %s = %s\n", name, val);//ADB=haha-day-and-night

        ret = unsetenv(name); //取消环境变量名name下的环境变量值
	printf("ret = %d\n",ret);     //取消成功返回0  否则返回1

	val = getenv(name);     //没有name这个变量名下的环境变量值  返回NULL
	printf("3, %s = %s\n", name,val);
}
