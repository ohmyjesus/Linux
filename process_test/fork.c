#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid;
	printf("xxxxxxxxxxx\n");

	pid = fork();
	if(pid==-1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid == 0)
	{
		printf("I am child , pid=%u ,ppid=%u\n " ,getpid(),getppid());
		sleep(3);
		printf("I am child , pid=%u ,ppid=%u\n " ,getpid(),getppid());
	}
	else 
	{
		sleep(1);
		printf("I am parent , pid=%u, ppid=%u\n ",getpid(), getppid());
		
	}

	printf("YYYYYYYYYYYYYYYY\n");

	return 0;
}
