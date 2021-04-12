#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <strings.h>
#include <sys/wait.h>

//"192.168.42.100"
#define SERV_PORT 8888
 
void wait_child(int signo){
	while(waitpid(0, NULL, WNOHANG) > 0);
	return ;
}

int main(){
	int lfd, cfd;
	pid_t pid;
	struct sockaddr_in serv_addr, clie_addr;
	socklen_t clie_addr_len;
	char buf[1024];
	char clie_IP[1024];
	int n, i;

	lfd = socket(AF_INET, SOCK_STREAM, 0);

	bzero(&serv_addr, sizeof(serv_addr));	//清空

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);  //8888是主机字节序，需要转换为网络字节序
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);  
	//inet_pton(AF_INET, "192.168.42.100", &serv_addr.sin_addr.s_addr);  //如果给定了IP地址的话

	bind(lfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

	listen(lfd, 120);

	while(1){
		clie_addr_len = sizeof(clie_addr);
		cfd = accept(lfd, (struct sockaddr*)&clie_addr, &clie_addr_len);
		printf("client IP:%s, client port:%d\n", inet_ntop(AF_INET, &clie_addr.sin_addr.s_addr, clie_IP, sizeof(clie_IP)), ntohs(clie_addr.sin_port));//服务器端端口号是指定的，但客户端端口号是操作系统自动分配的。

		pid = fork();
		if(pid < 0){
			perror("pid error");
			exit(1);
		}else if(pid == 0){			//子进程
			close(lfd);
			break;
		}else{					//父进程
			close(cfd);
			signal(SIGCHLD, wait_child);
		}
	}
	if (pid == 0){
		while(1){
			n = read(cfd, buf, sizeof(buf));
			if(n == 0){		//当n读到0时，此时客户端关闭
				close(cfd);
				return 0;
			}else if(n == -1){	//n=-1时，说明读文件失败
				perror("read error");
				exit(1);
			}else {
				for(i = 0; i < n; i++)
					buf[i] = toupper(buf[i]);
				write(cfd, buf, n);					
			}
		}
	}
	return 0;
}
