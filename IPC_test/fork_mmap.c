#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>

int var = 100;
int main()
{
	int *p;
	pid_t pid;
	int fd;
	fd = open("temp", O_RDWR|O_CREAT|O_TRUNC, 0644);
	if(fd<0){
		perror("open error");
		exit(1);
	
	unlink("temp");
	ftruncate(fd, 4);
//	p = mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_PRIVATE, fd, 0);
	p = mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
	if(p == MAP_FAILED){
		perror("mmap error");
		exit(1);
	}
	close(fd);  	 //映射区建立完毕
	
	pid = fork();	 //创建子进程
	if(pid == 0){
		*p = 2000;
		var = 1000;
		printf("child, *p = %d, var = %d\n",*p, var);    //子进程独享0-3G的用户空间  即全局变量独享
	}
	else{
		sleep(1);
		printf("parent, *p = %d, var = %d\n", *p, var);//   子进程和父进程共享mmap建立的映射区
		wait(NULL);

		int ret = munmap(p,4);		//释放映射区
		if(ret == -1){
			perror("munmap error");
			exit(1);
		}
	}




}
