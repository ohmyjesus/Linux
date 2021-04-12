#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>	//toupper的头文件

#define SERV_IP "127.0.0.1"
#define SERV_PORT 6666		//指定服务器端的端口号，一般指定3000以上稍大的，小的为系统端口

int main(){
	int lfd, cfd;
	struct sockaddr_in serv_addr, clie_addr;  //服务器端和客户端的地址信息
	socklen_t clie_addr_len;		//传入传出参数
	char buf[1024] = {0};
	char clie_IP[1024];
	int n, i, ret;

	lfd = socket(AF_INET, SOCK_STREAM, 0);
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);  //6666是主机字节序，需要转换为网络字节序
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  ////主机字节序到网络字节序IP地址的简便方法转换
	//inet_pton(AF_INET, "192.168.42.100", &serv_addr.sin_addr.s_addr);  //如果给定了IP地址的话

	ret = bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	ret = listen(lfd, 20);
	clie_addr_len = sizeof(clie_addr);
	cfd = accept(lfd, (struct sockaddr*)&clie_addr, &clie_addr_len);  //传出参数和传入传出参数  此时阻塞等待

	printf("client IP:%s, client port:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)),
			ntohs(clie_addr.sin_port));//服务器端端口号是指定的，但客户端端口号是操作系统自动分配的。

	while(1){
	n = read(cfd, buf, sizeof(buf));
	for(i = 0; i < n; i++)
		buf[i] = toupper(buf[i]);
	write(cfd, buf, n);
	}

	close(lfd);
	close(cfd);
	return 0;
}
