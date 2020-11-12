#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/mman.h>
#include<string.h>


int main() {
	pid_t pid;
	int fd1, fd2;
	//打开源文件
	fd1 = open("english.txt", O_RDWR);

	//1.指定创建子进程的个数
	int n = 6;

	//3.打开目的文件，不存在则创建
	fd2 = open("english2.txt", O_RDWR|O_CREAT,0644);
	if(fd2<0){
		perror("open error:");
		exit(1);
	}
	
	//4.获取文件大小
	int ret = lseek(fd1, 0, SEEK_END);
	printf("ret=%d\n", ret);

	//5.根据文件大小拓展目标文件
	ftruncate(fd2,12000);

	//6.为源文件创建映射
	int *p1 = mmap(NULL ,12000, PROT_READ|PROT_WRITE, MAP_SHARED, fd1, 0);
	int *a=p1;
	if(p1==MAP_FAILED){
		perror("mmap1 error:");
		exit(1);}

	ftruncate(fd2,2000);
	//7.为目标文件创建映射
	int *p2 = mmap(NULL ,12000, PROT_READ|PROT_WRITE, MAP_SHARED, fd2, 0);
	int *b=p2;
	if(p2==MAP_FAILED){
		perror("mmap2 error:");
		exit(1);}
	int i=0;
	int j=0;
	//.创建6个子进程
	for(i;i<7;i++){
		pid=fork();
		if(pid==0){
			break;}
		sleep(i);
	}
	for(j=0+2000*i;j<2000+2000*i;j++){
		*(b+j)=*(a+j);
		printf("i am %d th child",i+1);
		exit(1);
	}
	
	//.子进程完成分块拷贝
	
	close(fd1);
	close(fd2);
	//.释放映射区
	munmap(p1,12000);
	munmap(p2,12000);
	return 0;
	}
