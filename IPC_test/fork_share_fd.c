/*
 * 父子进程共享打开的文件描述符--- 使用文件完成进程间通信
 */
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<sys/wait.h>
#include<stdlib.h>

int main()
{
	int fd1,fd2;
	pid_t pid;
	char buf[1024];
	char str[]="------test for shared fd in parent child process--------\n";

//	pid = fork();
//	if(pid<0)
//	{
//		perror("fork");
//		exit(1);
//	}
//	else if(pid==0)		//子进程
//	{
	fd1 = open("test.txt", O_RDWR);
	if(fd1 == -1)
	{
		perror("open");
		exit(1);
	}
	write(fd1, str, strlen(str));
	printf("child write over\n");
	close(fd1);
//	}
//	else{ 			//父进程
		fd2 = open("test.txt", O_RDWR);
		if(fd2<0){
			perror("open");
			exit(1);
		}
		sleep(1);
		int ret = read(fd2, buf, sizeof(buf));
		write(STDOUT_FILENO, buf, ret);
		wait(NULL);
//	}
	return 0;
}
