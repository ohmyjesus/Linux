#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void printped(sigset_t *p){
	int i;
	for(i=1;i<32;i++)
	{
		if(sigismember(p, i)==1){
			putchar('1'); }
		else{ putchar('0'); }
	}
	printf("\n");
}

int main() { 
	sigset_t myset,oldset,ped;

	sigemptyset(&myset); 
	sigaddset(&myset,SIGQUIT);  //将信号3置一  ctrl+\
	
	sigaddset(&myset,SIGSEGV);  //屏蔽信号11 段错误 
	sigaddset(&myset,SIGSTOP);  //屏蔽信号19   但信号19不能被屏蔽  
	sigaddset(&myset,SIGTSTP);  //屏蔽信号20   ctrl+z  


	sigaddset(&myset,SIGINT);  //屏蔽信号2  ctrl+c
	sigprocmask(SIG_BLOCK, &myset, &oldset); //将自定义的信号集来影响阻塞信号集

	while(1){ 
		sigpending(&ped);  //ped作为传出参数 实时更新读取的状态  sigpending用来读取未决信号集
		sleep(1);
		printped(&ped);
	}

	return 0;
}
