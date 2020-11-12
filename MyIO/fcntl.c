#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
	int fd ;
	int flag;
	//测试字符串
	char *p="66666666666666";
	char *q="000000000000000";

	//只写的方式打开文件
	fd = open("test.txt",O_WRONLY);
	if(fd == -1)
	{
		perror("open");
		exit(1);
	}
	//输入新的内容，会覆盖原来旧的内容
	if(write(fd, p, strlen(p))==-1)
	{
		perror("write");
		exit(1);
	}
	//使用F_GETFL得到命令的状态标志
	flag = fcntl(fd,F_GETFL, 0);
	if(flag==-1)
	{
		perror("fcntl");
		exit(1);
	}
	//将文件状态标志添加“追加写”选项
	flag |= O_APPEND;
	//将文件状态修改为追加写
	if(fcntl(fd, F_SETFL,flag)==-1)
	{
		perror("fcntl -- append write");
		exit(1);
	}
	//再次输入新内容，该内容会追加到旧内容的后面
	if(write(fd, q, strlen(q))==-1)
	{
		perror("write again");
		exit(1);
	}

	close(fd);
	return 0;
}
