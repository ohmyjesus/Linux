#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	int i;
	printf("xxxxxxxxx\n");
	for (i=0;i<5;i++)
	{
		pid=fork();
		if(pid==-1)
		{
			perror("fork");
			exit(1);
		}
		if(pid==0)
		{
			break;
		}
	}
	if(i<5)
	{
		sleep(i);
		printf("I'm %dth child , pid=%u\n",i+1, getpid());
	}
	else
	{
	sleep(i);
	printf("I'm parent , pid = %u\n", getpid());
	}
	return 0;
}

