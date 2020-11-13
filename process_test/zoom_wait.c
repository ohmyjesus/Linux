#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid,wpid ; // 非负整数类型
	int status;
	pid = fork();

	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if (pid==0)
	{
		execl("./abnor","abnor",NULL);
		printf("I'm  child , my parent is %u ,I am going to sleep 3s\n", getppid());
		sleep(3);
		printf("----------child die --------------\n");
		exit(55);
	}
	else 
	{
		wpid = wait(&status);
		if(wpid==-1)
		{
			perror("wait error");
			exit(1);
		}
		if(WIFEXITED(status))
		{
			printf("child exit with %d\n", WEXITSTATUS(status));
		}
		if(WIFSIGNALED(status))
		{
			printf("child killed by %d\n",WTERMSIG(status));
		}
		while(1)
		{
		printf("I'm parent , pid=%u, myson pid =%d\n", getpid(), pid);
		sleep(1);
		}
	}
	return 0;
}
