#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <signal.h>

void my_func(int signo)
{
	printf("hello world\n");
}


int main(){
	struct itimerval it, oldit;   //old为传出参数
	signal(SIGALRM, my_func);    //信号的捕捉


	it.it_value.tv_sec = 5;   //定时时长  秒
	it.it_value.tv_usec = 0;  //微妙

	it.it_interval.tv_sec = 3;  //定时间隔时间
	it.it_interval.tv_usec = 0;

	if(setitimer(ITIMER_REAL, &it, &oldit)==-1){
		perror("setitimer error");
		exit(1);
	}
	
	while(1);

	return 0;
}
