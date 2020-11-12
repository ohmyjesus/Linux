#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

int main()
{
	int fd;
	fd = open("ps.out", O_WRONLY|O_CREAT|O_TRUNC,0644);
	if(fd<0)
	{
		perror("open");
		exit(1);
	}
	dup2(fd, 1);
	
	execlp("ps","ps","aux",NULL);
	//close(fd)_;
	//
	return 0;
}
