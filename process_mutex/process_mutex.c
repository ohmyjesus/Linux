#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/wait.h>

struct mt{
	int num;			//共享资源
	pthread_mutex_t mutex;		//锁
	pthread_mutexattr_t mutexattr;	//锁的属性
};

int main()
{
	int i;
	struct mt* mm;
	pid_t pid;
/*
 *	int fd = open("mt_test", O_CREAT | O_RDWR, 0777);
 *	ftruncate(fd, sizeof(*mm));
 *	mm = mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
 *	close(fd);
 *	unlink("mt_test");
 */
	mm = mmap(NULL, sizeof(*mm), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANON, -1, 0);	//创建匿名映射区
	memset(mm, 0, sizeof(*mm));	//指针清0

	pthread_mutexattr_init(&mm->mutexattr);					//初始化mutex属性对象
	pthread_mutexattr_setpshared(&mm->mutexattr, PTHREAD_PROCESS_SHARED);	//修改属性为进程间共享

	//初始化之前修改其属性
	pthread_mutex_init(&mm->mutex, &mm->mutexattr);				//初始化一把mutex锁

	pid = fork();
	if(pid == 0){
		for(i = 0; i < 10; i++){
                         pthread_mutex_lock(&mm->mutex);
                         (mm->num)++;
                         printf("----child---num++  %d\n", mm->num);
			 pthread_mutex_unlock(&mm->mutex);
			 sleep(1);
		}
	}else if(pid > 0){
		for(i = 0; i < 10; i++){
			sleep(1);
			pthread_mutex_lock(&mm->mutex);
			mm->num += 2;
			printf("----parent---num+=2  %d\n", mm->num);
			pthread_mutex_unlock(&mm->mutex);
		}
		wait(NULL);
	}

	pthread_mutexattr_destroy(&mm->mutexattr);	//销毁mutex属性对象
	pthread_mutex_destroy(&mm->mutex);		//销毁mutex
	munmap(mm, sizeof(*mm));			//释放映射区

	return 0;
}
