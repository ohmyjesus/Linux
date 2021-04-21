#include <stdio.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <errno.h>
#include <unistd.h>

#define MAXLINE 10

//如何使用epoll监听一个管道

int main(){
	int efd, i;
	int pfd[2];
	pid_t pid;
	char buf[MAXLINE], ch = 'a';

	pipe(pfd);
	pid = fork();
	//0是读端  1是写端

	if(pid == 0){		//子写
		close(pfd[0]);
		while(1){
			//aaaa\n
			for(i = 0; i < MAXLINE/2; i++)
				buf[i] = ch;
			buf[i-1] = '\n';
			ch++;
			//bbbb\n
			for(; i<MAXLINE; i++)
				buf[i] = ch;
			buf[i-1] = '\n';
			ch++;
			//aaaa\nbbbb\n
			write(pfd[1], buf, sizeof(buf));	//从缓冲区写入管道中
			sleep(5);
		}
		close(pfd[1]);

	}else if(pid > 0){	//父读
		struct epoll_event event;
		struct epoll_event resevent[10];   //epoll_wait就绪返回event
		int res, len;

		close(pfd[1]);
		efd = epoll_create(10);

		event.events = EPOLLIN | EPOLLET;  //ET 边沿触发
		//event.events = EPOLLIN;		//LT 水平触发（默认）
		event.data.fd = pfd[0];
		epoll_ctl(efd, EPOLL_CTL_ADD, pfd[0], &event);

		while(1){
			res = epoll_wait(efd, resevent, 10, -1);
			printf("res %d\n", res);
			if(resevent[0].data.fd == pfd[0]){
				len = read(pfd[0], buf, MAXLINE/2);
				write(2, buf, len);
			}
		}
	}



	return 0;
}
