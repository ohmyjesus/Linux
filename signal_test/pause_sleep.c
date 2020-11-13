#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

void action(int a){
	printf("catch succeed\n");
}	


int main(){
	int a,b;
	for(a=0;a<5;a++)
	{
		printf("the first number is %d\n",a);
	}

//	signal(SIGALRM,action);  1.简单的捕捉函数

	struct sigaction act,oldact;	//2.复杂的捕捉函数
	act.sa_handler = action;
	sigemptyset(&act.sa_mask);

	act.sa_flags = 0;
	
	int ret = sigaction(SIGALRM, &act, &oldact);
	if(ret == -1)
	{
		perror("sigaction error");
		exit(1);
	}

	alarm(5);   //定时器5s
	int ret1 = pause();
	if(ret1 == -1&& errno==EINTR) //检查pause的返回值
	{
		printf("pause succeed\n");
	}

	alarm(0);
	sigaction(SIGALRM,&oldact, NULL);//恢复SIGALRM旧有的处理方式
	return 0;
}

