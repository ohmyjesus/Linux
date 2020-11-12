#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>

int main()
{
	int fd[2];
	pid_t pid;
	int ret = pipe(fd);
	if(ret == -1)
	{
		perror("pipe error");
		exit(1);
	}
	
	pid = fork();
	if(pid == -1)
	{
		perror("fork error");
		exit(1);
	}
	else if(pid == 0)    //子进程读数据  关闭写端
	{
		close(fd[1]);
		char buf[1024];
		ret = read(fd[0], buf, sizeof(buf));
		if (ret==0)
		{
			printf("---------\n");
		}
		write(STDOUT_FILENO, buf, ret); //输出到屏幕
	}
	else{			//父进程写数据 关闭读端
		close(fd[0]);
		write(fd[1], "hello pipe\n", strlen("hello pipe\n"));
	}
	return 0;
}
