#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <error.h>
#include <fcntl.h>

#define MAXLINE 10
#define SERV_PORT 9000

int main(int argc,char* argv[])
{
	struct sockaddr_in cliaddr, servaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	char buf[MAXLINE];
	char str[INET_ADDRSTRLEN];
	int efd, flag;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	
	listen(listenfd, 128);
	///////////////////////////////////////////////////////////////////
	struct epoll_event event;
	struct epoll_event resevent[10];
	int res, len;
	
	efd = epoll_create(10);   			//创建epoll模型，efd指向红黑树根结点
	
	event.events = EPOLLIN | EPOLLET;  		//ET边沿触发，默认是水平触发
	//event.events = EPOLLIN;
	printf("accepting connections ... \n");
	cliaddr_len = sizeof(cliaddr);
	connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &cliaddr_len);

	printf("receive from %s at PROT %d\n", 
		inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
		ntohs(cliaddr.sin_port));
	
	flag = fcntl(connfd, F_GETFL);			//修改connfd为非阻塞读
	flag |= O_NONBLOCK;
	fcntl(connfd, F_SETFL, flag);

	event.data.fd = connfd;
	epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &event);		//将connfd加入监听红黑树
	while(1){
		printf("epoll_wait begin\n");
		res = epoll_wait(efd, resevent, 10, -1);	//最多10个，阻塞监听
		printf("epoll_wait end res %d\n", res);

		if(resevent[0].data.fd == connfd){
			while((len = read(connfd, buf, MAXLINE/2)) > 0)  //非阻塞读，轮询
				write(2, buf, len);
				
		}
	
	}
	return 0;
}
