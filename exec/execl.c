#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	pid = fork();
	if(pid == -1)
	{
		perror("fork");
		exit(1);
	}
	else if(pid>0)
	{
		sleep(1);
		printf("I'm parent\n");
	}
	else
		execl("/bin/ls", "ldadwas","-l",NULL);
	return 0;
}
