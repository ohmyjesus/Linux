#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>

#define SERV_PORT 6666
#define SERV_IP "127.0.0.1"

int main(){
	int cfd;
	struct sockaddr_in serv_addr;
	char buf[1024];
	int n;

	cfd = socket(AF_INET, SOCK_STREAM, 0);
	//memset(&serv_addr, 0, sizeof(serv_addr));	//清空指针为0,防止初始化失败，存放一个无效地址
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET, SERV_IP, &serv_addr.sin_addr.s_addr);

	
	connect(cfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
	
	while(1){
		fgets(buf, sizeof(buf), stdin);  		//从屏幕上获取，标准输入
		write(cfd, buf, strlen(buf));			//从buf里往出写到cfd里，传给服务器
		n = read(cfd, buf, sizeof(buf));			//服务器端变成大写，传给客户端，客户端需要读
		write(1, buf, n);		//写到屏幕上
	}
	
	close(cfd);
	return 0;
}
