#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid ; // 非负整数类型
	pid = fork();

	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid==0)
	{
		printf("I'm  child , my parent is %u ,I am going to sleep 10s\n", getppid());
		sleep(10);
		printf("----------child die --------------\n");
	}
	else 
	{
		while(1)
		{
		printf("I'm parent , pid=%u, myson pid =%d\n", getpid(), pid);
		sleep(1);
		}
	}
	return 0;
}
