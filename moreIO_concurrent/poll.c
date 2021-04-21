#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <errno.h>
#include <poll.h>

#define MAXLINE 80
#define SERV_PORT 8000
#define OPEN_MAX 1024

int main(int argc,char* argv[])
{
	int i,j,maxi;

	int nready;
	int  listenfd, connfd, sockfd;
	char buf[MAXLINE], str[INET_ADDRSTRLEN];		//define INET——ADDRSTRLEN 16

	ssize_t n;
	struct sockaddr_in cliaddr, servaddr;
	struct pollfd client[OPEN_MAX];
	socklen_t clilen;

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*) &servaddr, sizeof(servaddr));
	listen(listenfd, 128);

	client[0].fd = listenfd;			//要监听的第一个文件描述符 存入client[0]
	client[0].events = POLLIN;			//listenfd监听普通读事件

	for(i=1; i < OPEN_MAX; i++){
		client[i].fd = -1;				//用-1初始化client[]
	}

	maxi = 0;

	while(1){
		nready = poll(client, maxi+1, -1);		//阻塞监听是否有客户端链接请求

		if(client[0].revents & POLLIN){		//说明有新的客户端链接请求

			clilen = sizeof(cliaddr);
			connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);  //Accpet不会阻塞
			printf("receive from %s at PROT %d\n", 
					inet_ntop(AF_INET, &cliaddr.sin_addr, str, sizeof(str)),
					ntohs(cliaddr.sin_port));

			for(i = 1;i < OPEN_MAX; i++)
				if(client[i].fd < 0){	
					client[i].fd = connfd; // 找到client[]中空闲的位置，存放accept返回的connfd
					break;
				}

			if(i == OPEN_MAX){		// 达到了最大客户端数
				perror("too many clients");
				exit(1);
			}
			client[i].events = POLLIN;
			if(i > maxi)
				maxi = i;		//保证maxi存的总是client[]最后一个元素下标

			if(--nready <= 0)
				continue;
		}

		for(i = 1; i<=maxi; i++){		//检测哪个clients有数据就绪
			if((sockfd = client[i].fd) < 0)
				continue;
			if(client[i].revents & POLLIN){

				if((n = read(sockfd, buf, MAXLINE)) < 0){ 
					if(errno == ECONNRESET){
						printf("client[%d] aborted connection\n", i);
						close(sockfd);
						client[i].fd = -1;	//poll中不监控该文件描述符，直接置为-1即可，不用像select那样移除
					}else{
						perror("read error");
						exit(1);
					}
				}else if(n==0){			//说明客户端先关闭链接

					printf("client[%d] aborted connection\n", i);
					close(sockfd);
					client[i].fd = -1;
				}else{
					for(j = 0;j<n;j++)
						buf[j] = toupper(buf[j]);
					write(sockfd, buf, n);
				}
				if(--nready <= 0)
					break;			//跳出for，但还在while中
			}
		}
	}
	return 0;
}
