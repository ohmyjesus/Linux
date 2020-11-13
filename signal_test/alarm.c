#include <stdio.h>
#include <unistd.h>

int main()
{

	int i ;
	alarm(1);
	while(1){
		i++;
		printf(" %d",i);
	
	}

}
