#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc,char *argv[])
{
	if(argc<2)
	{
		printf("a.out  filename\n");
		exit(1);
	}

	int ret = access(argv[1], R_OK);
	if(ret == -1)
	{
		perror("access");
		exit(1);
	}
	else
	{
		printf("you can read this file\n");
		return 0;
	
	}
}
