#include <stdio.h>                                                                                                                         
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <strings.h>
#include <sys/un.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <ctype.h>      //toupper的头文件
#include <stddef.h>

#define SERV_ADDR "serv.socket"
#define CLIE_ADDR "clie.socket"

int main(){
        int cfd, len;
        struct sockaddr_un servaddr, cliaddr;  //服务器端和客户端的地址信息
        char buf[4096];

        cfd = socket(AF_UNIX, SOCK_STREAM, 0);

	bzero(&cliaddr, sizeof(cliaddr));
        cliaddr.sun_family = AF_UNIX;
	strcpy(cliaddr.sun_path, CLIE_ADDR);

	len = offsetof(struct sockaddr_un, sun_path) + strlen(cliaddr.sun_path);//需要计算客户端地址有效长度

	unlink(CLIE_ADDR);			//确保bind之前serv.sock文件不存在， bind会创建该文件
        bind(cfd, (struct sockaddr*)&cliaddr, len);	//客户端也需要绑定，不能依赖自动绑定，bind完之后，创建出来一个名字叫做clie.socket的文件

	/////////////////////////////绑定服务器文件名/////////////////////////////////
	
	bzero(&servaddr, sizeof(servaddr));	//构造server 地址,用服务器的文件名来做链接
        servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path, SERV_ADDR);

	len = offsetof(struct sockaddr_un, sun_path) + strlen(servaddr.sun_path);//求出文件名在结构体中的偏移量，在加上实际文件名的长度


        connect(cfd, (struct sockaddr*)&servaddr, len);  //与服务器端建立好链接
	
	while(fgets(buf, sizeof(buf), stdin) != NULL){
		write(cfd, buf, strlen(buf));
		len = read(cfd, buf, sizeof(buf));
		write(1, buf, len);
	}

        close(cfd);
        return 0;
}


