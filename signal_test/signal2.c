#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <error.h>
#include <stdlib.h>


typedef void (*sighandler_t)(int);

void myfunc(int a){
	printf("000\n");
}

int main()
{	
	
	sighandler_t handler;

	handler = signal(SIGINT,myfunc);
	if(handler == SIG_ERR){
		perror("signal error");
		exit(1);
	}
	while(1);

	return 0;
	
}
