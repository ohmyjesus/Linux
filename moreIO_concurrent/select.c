#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <arpa/inet.h>

#define SERV_PORT 6666

int main(int argc,char* argv[])
{
	int i,j,n,maxi;

	int nready, client[FD_SETSIZE];			//自定义数组client，防止遍历1024个文件描述符 FD_SETSIZE默认为1024
	int maxfd, listenfd, connfd, sockfd;
	char buf[BUFSIZ], str[INET_ADDRSTRLEN];		//define INET_ADDRSTRLEN 16

	struct sockaddr_in clie_addr, serv_addr;
	socklen_t clie_addr_len;
	fd_set rset, allset;				// rset用来监听读事件的集合  allset用来保存原来的集合

	listenfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(SERV_PORT);

	bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));
	listen(listenfd, 20);

	maxfd = listenfd;				// 起初listenfd 即为最大文件描述符

	maxi = -1;					// 将来用作client[]的下标，初始值指向0个元素之前的下标位置
	for(i=0; i<FD_SETSIZE; i++){
		client[i] = -1;				//用-1初始化client[]
	}
	FD_ZERO(&allset);
	FD_SET(listenfd, &allset);			//构造select监听文件描述符集

	while(1){
		rset = allset;				//每次循环时都从新设置select监听信号集
		nready = select(maxfd+1, &rset, NULL, NULL, NULL);  //阻塞等待
		if(nready < 0){
			perror("select error");
			exit(1);
		}

		if(FD_ISSET(listenfd, &rset)){		//说明有新的客户端链接请求
			clie_addr_len = sizeof(clie_addr);
			connfd = accept(listenfd, (struct sockaddr*)&clie_addr, &clie_addr_len);  //Accpet不会阻塞
			printf("receive from %s at PROT %d\n", 
					inet_ntop(AF_INET, &clie_addr.sin_addr, str, sizeof(str)),
					ntohs(clie_addr.sin_port));

			for(i = 0;i<FD_SETSIZE;i++)
				if(client[i] < 0){	//找client[]中没有使用的位置
					client[i] = connfd; // 保存accept返回的文件描述符到client[]里
					break;
				}

			if(i == FD_SETSIZE){		// 达到select能监听的文件个数上限1024
				fputs("too many clients\n", stderr);
				exit(1);
			}

			FD_SET(connfd, &allset);	//向监听文件描述符集合allset添加新的文件描述符connfd
			if(connfd > maxfd)
				maxfd = connfd;

			if(i > maxi)
				maxi = i;		//保证maxi存的总是client[]最后一个元素下标

			if(--nready == 0)
				continue;
		}

		for(i = 0; i<=maxi; i++){		//检测哪个clients有数据就绪
			if((sockfd = client[i]) < 0)
				continue;
			if(FD_ISSET(sockfd, &rset)){

				if((n = read(sockfd, buf, sizeof(buf))) == 0){ //当client关闭链接时，服务器端也关闭链接
					close(sockfd);
					FD_CLR(sockfd, &allset);		//解除select对此文件描述符的监听
					client[i] = -1;
				}else if(n > 0){
					for(j = 0;j<n;j++)
						buf[j] = toupper(buf[j]);
					write(1, buf, n);
					write(sockfd, buf, n);
				}
				if(--nready == 0)
					break;			//跳出for，但还在while中
			}
		}
	}
	close(listenfd);
	return 0;
}
