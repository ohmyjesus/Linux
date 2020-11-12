#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>

int main()
{
	char *p = NULL;
	int fd = open("mytest.txt", O_CREAT|O_RDWR, 0644);
	if(fd<0)
	{
		perror("open error:");
		exit(1);
	}
	ftruncate(fd , 4);

	p = mmap(NULL,4, PROT_READ|PROT_WRITE, MAP_SHARED,fd,0 );
	if(p == MAP_FAILED){
		perror("mmap error:");
		exit(1);
	}
	strcpy(p , "abc"); //写数据
	int a = munmap(p,4);
	if(a<0){
		perror("munmap error:");
		exit(1);}

	close(fd);

	return 0;
}
