#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <error.h>

#define MAXLINE 8192
#define SERV_PORT 8000
#define OPEN_MAX 5000

int main(int argc,char* argv[])
{
	int  i, listenfd, connfd, sockfd;
	int n, num = 0;
	ssize_t nready, efd, res;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];		//define INET——ADDRSTRLEN 16

	struct sockaddr_in cliaddr, servaddr;
	struct epoll_event tep, ep[OPEN_MAX];			//tep: epoll_ctl参数  ep[]：epoll_wait参数
	socklen_t clilen;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));//端口复用

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	listen(listenfd, 128);

	efd = epoll_create(OPEN_MAX);   			//创建epoll模型，efd指向红黑树根结点
	if(efd == -1){
		perror("epoll_create error");
		exit(1);
	}

	tep.events = EPOLLIN;
	tep.data.fd = listenfd;					//指定lfd的监听事件为读
	res = epoll_ctl(efd, EPOLL_CTL_ADD, listenfd, &tep);	//将lfd及对应的结构体设置到树上，即添加根结点
	if(res == -1){
		perror("epoll_ctl error");
		exit(1);
	}

	for(;;){
		//epoll为server阻塞监听事件，ep为struct epoll_event 类型数组， OPEN_MAX为数组容量， -1表示永久阻塞
		nready = epoll_wait(efd, ep, OPEN_MAX, -1);
		if(nready == -1){
			perror("epoll_wait error");
			exit(1);
		}
		for(i = 0;i < nready; i++){
			if(!(ep[i].events & EPOLLIN))		//如果不是读事件，继续循环
				continue;

			if(ep[i].data.fd == listenfd){		//判断满足读事件的fd是不是lfd的读事件
				clilen = sizeof(cliaddr);
				connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);  //接受链接
				printf("receive from %s at PROT %d\n", 
						inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
						ntohs(cliaddr.sin_port));
				printf("cfd %d---client %d\n", connfd, ++num);

				tep.events = EPOLLIN;		//将客户端成功连接到的结点添加到红黑树上
				tep.data.fd = connfd;
				res = epoll_ctl(efd, EPOLL_CTL_ADD, connfd, &tep);
				if(res == -1){
					perror("epoll_ctl error");
					exit(1);
				}
			} else {				//不是lfd,则说明是客户端的发送数据,就开始读数据
				sockfd = ep[i].data.fd;
				n = read(sockfd, buf, MAXLINE);

				if(n==0){			//读到0,说明客户端关闭链接
					res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL); //将该文件描述符从红黑树删除
					if(res == -1){
						perror("epoll_ctl error");
						exit(1);
					}
					close(sockfd);		//关闭与客户端的链接
					printf("client[%d] closed connection\n", sockfd);

				}else if(n < 0){		//出错的话也没必要再监听
					perror("read n<0 error: ");	
					res = epoll_ctl(efd, EPOLL_CTL_DEL, sockfd, NULL);
					close(sockfd);

				}else{					//实际读到的字节数
					for(i = 0; i < n; i++)
						buf[i] = toupper(buf[i]); //转大写，回写给客户端

					write(1, buf, n);	//输出到屏幕
					write(sockfd, buf, n);
				}
			}
		}
	}
	return 0;
}
