#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void docatch(int signo) 
{ 
	printf("  信号%d捕捉成功\n",signo); //打印出来哪个信号被捕捉
	sleep(10);
	printf("------finish---------\n";
} 
int main() 
{ 
	struct sigaction act;

	act.sa_handler = docatch;
	sigemptyset(&act.sa_mask);   	  //将集合sa_mask清零
	sigaddset(&act.sa_mask, SIGQUIT); //将需要屏蔽的信号加入到sa_mask中  ctrl+\
	act.sa_flags = 0;		  //默认属性， 信号捕捉函数期间自动屏蔽本信号

	int ret = sigaction(SIGINT,&act, NULL );   //NULL忽略原的处理动作 sigint=ctrl+c
	if(ret == -1){
		perror("sigaction error");
		exit(1);
	}

	while(1);

	return 0;
}
