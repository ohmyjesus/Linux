#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

#define N 5

int main(){
	int j;
	pid_t pid,q;
	for( j=1;j<6;j++){
	pid = fork();
	if(pid==-1){
		perror("fork error");
	}
	
	if(pid==0)
	{
		break;
	}
	if(j==3)
	{
		q = pid;
	}
	}
	if(j<6)
	{
		for(;;){
			printf("i am child %d, mypid is %u\n",j,getpid());
			sleep(1);
		}
	}
	else
	{
	sleep(1);
	int ret = kill(q,SIGKILL);   //父进程保存q的变量 用来杀死进程
	while(1);
	if(ret==-1)
	{
		perror("kill error");
		exit(1);
	}
	}
	return 0;
}
