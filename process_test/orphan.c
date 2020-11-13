#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	pid_t pid;
	pid = fork();

	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid==0)
	{
		while(1)
		{
			printf("I'm child , my parent pid is %u\n", getppid());
			sleep(1);
		}
	}
	else 
	{
		printf("I'm parent , my pid is %u\n", getpid());
		sleep(9);
		printf("I'm going to die ----------------\n");
	}
	return 0;

}
