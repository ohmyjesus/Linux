#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
	int fd = open("aa", O_RDWR);
	if(fd==-1)
	{
		perror("open");
		exit(1);
	}

	//获取文件长度
//	int ret = lseek(fd, 0, SEEK_END);    //从end向后偏移0个字节 再读长度
//	printf("file length = %d\n",ret);
	
	//文件拓展
	int ret = lseek(fd, 2000, SEEK_END);
	printf("return value %d\n", ret);

	//实现文件拓展，需要再最后做一次写操作;
	write(fd, "a", 1);
	close(fd);
	return 0 ;

}


