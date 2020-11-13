#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(int argc,char *argv[])
{
	int n =5 ,i;
	pid_t p,q;

	if(argc==2)
	{
		n=atoi(argv[1]);
	}
	for(i=0;i<n;i++)
	{
		p=fork();
		if(p==0)
		{
			break;
		}
		else if(i==3)
		{
			q=p;   //父进程返回的是子进程的id  将三儿子的进程id保存
		}
	}
	if(n==i)
	{
		sleep(n);
		printf("I'm parent, pid=%d\n", getpid(),getgid());
		while(waitpid(-1,NULL,0));
		printf("wait finish\n");
	}
	else 
	{
		sleep(i);
		printf("I'm %dth child, pid=%d,gpid=%d\n",i+1,getpid(),getgid());
	}
	return 0;
}
