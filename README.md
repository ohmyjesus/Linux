# Linux
Operating_system_knowledge

此学习内容来源于[Linux黑马教程](https://www.bilibili.com/video/BV1dt411f7TZ?p=171)，库中目前包含以下知识点
### Last year I learn Includes the following parts
1. gcc编译
2. gdb调试
3. makefile
4. 系统IO函数和文件操作函数
5. 进程
6. 函数族
7. 进程间通信
8. 信号
9. 守护进程
10. 线程

---
## form 2021.3 to now study including 

### 1. socket套接字实现客户端与服务器端的通信
&emsp;&emsp;**重要的知识点有** socket套接字的基本概念，网络字节序与主机字节序的转换，IP地址的转换，sockaddr数据结构，网络套接字的函数，Client/Server的通信流程，如何编写客户端和服务器端的代码等。
### 2. 简单的多进程/多线程并发服务器实现
&emsp;&emsp;多进程并发服务器的主要思想为： 每当有一个客户端发起连接时，父进程就产生一个子进程来与客户端进行通信，同时为了避免子进程成为僵尸进程，需要父进程回收子进程，可通过**信号的方式(SIGCHLD)方式**来让内核帮忙回收子线程。

&emsp;&emsp;多线程并发服务器的主要思想为：与多进程类似的，只是子进程的事交给子线程去完成，子线程完成对应的读写操作，同时为了保证子线程不会成为僵尸线程，在create之后应将该线程设置为**线程分离**状态detach。

&emsp;&emsp;但其实在实际开发过程中，并不会使用多进程或者多线程方式，因为这样的开销过大，每有一个客户端发起连接时，就会产生一个server.c，会占用大量资源，有一种好的方法是I/O复用，后续再介绍它。
### 3. I/O复用
### 4. 线程池
