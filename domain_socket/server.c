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

int main(){
        int lfd, cfd, len, size, i;
        struct sockaddr_un servaddr, cliaddr;  //服务器端和客户端的地址信息
        char buf[4096];

        lfd = socket(AF_UNIX, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
        servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path, SERV_ADDR);	//指定server的文件名

	len = offsetof(struct sockaddr_un, sun_path) + strlen(servaddr.sun_path);//求出文件名在结构体中的偏移量，在加上实际文件名的长度

	unlink(SERV_ADDR);			//确保bind之前serv.sock文件不存在， bind会创建该文件

        bind(lfd, (struct sockaddr*)&servaddr, len);	//参3不能是sizeof(servaddr)

        listen(lfd, 20);
	
	printf("accept ...\n");
	while(1){
		len = sizeof(cliaddr);
		cfd = accept(lfd, (struct sockaddr*)&cliaddr, (socklen_t*)&len);
		
		len -= offsetof(struct sockaddr_un, sun_path);	//得到文件名的长度
		cliaddr.sun_path[len] = '\0';			//确保打印时，没有乱码出现
		
		printf("client bind filename %s\n", cliaddr.sun_path);
		
        	while((size = read(cfd, buf, sizeof(buf))) >0){
        		for(i = 0; i < size; i++)
                		buf[i] = toupper(buf[i]);
        		write(cfd, buf, size);
        	}
		close(cfd);
	}
        close(lfd);
        return 0;
}


