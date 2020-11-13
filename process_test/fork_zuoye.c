#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid,pid1,pid2,pid3,wpid;
	int i,n,status1,status2,status3;
	printf("fork begin\n");
	for(i=1;i<4;i++)
	{
		pid=fork();
	if(pid==0)    //子进程
	{
		if(i==1)
		{
		pid1=getpid();
		execlp("ps","ps",NULL);
		exit(76);
		}
		else if(i==2)
		{
		pid2=getpid();
		execl("1.out","1.out",NULL);
		exit(55);
		}
		else
		{
		pid3=getpid();
		execl("abnor","abnor",NULL);
		break;
		}
	}
	}
		waitpid(pid1,&status1,0);
		if(WIFEXITED(status1))
		printf("pid1 was finished with %d\n",WEXITSTATUS(status1));
		
		waitpid(pid2,&status2,0);
		if(WIFSIGNALED(status2))
		printf("pid2 was finish by %d\n",WEXITSTATUS(status2));
		
		waitpid(pid3,&status3,0);
		if(WIFSIGNALED(status3))
		printf("pid3 was killed by %d\n",WTERMSIG(status3));
		printf("wait finish\n");	
	return 0;	
}
