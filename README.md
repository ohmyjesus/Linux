# Linux
Operating_system_knowledge

此学习内容来源于[Linux黑马教程](https://www.bilibili.com/video/BV1dt411f7TZ?p=171)，库中目前包含以下知识点
## from 2020.3 - 2020.10 learning 
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
&emsp;&emsp;多进程并发服务器的主要思想为： 每当有一个客户端发起连接时，父进程就产生一个子进程来与客户端建立连接并进行数据交互，同时为了避免子进程成为僵尸进程，需要父进程回收子进程，可通过**信号捕捉(SIGCHLD)方式**来回收子进程。

&emsp;&emsp;多线程并发服务器的主要思想为：与多进程类似的，每当有一个客户端发起连接时，主控线程就产生一个子线程来与客户端建立连接并进行数据交互，同时为了保证子线程不会成为僵尸线程，应该将子线程设置为**线程分离**状态detach，使子线程自动退出，不会有残留资源存在于内核中。

&emsp;&emsp;但其实在实际开发过程中，并不会使用多进程或者多线程方式，因为这样的开销过大，每有一个客户端发起连接时，就会产生一个server.c，会占用大量资源，有一种好的方法是I/O复用，后续再介绍它。
### 3. I/O复用
&emsp;&emsp;今天学习了IO多路复用，与多线程/多进程并发相比，它不再是靠应用程序server.c自己来监听客户端，而且让内核帮忙完成监听，这样可以减小资源的开销。IO多路复用主要用到三个函数，即select，poll和epoll函数，需要掌握的是这三个函数各自的特点，理解三个函数参数的作用，如何使用这三个函数进行多个客户端和服务器端的通信，其中epoll的IO非阻塞方式是优先推荐的。关于三个函数的特点之后再进行补充。
<p align="right">by2021.4.13</p>

#### select
- 所能同时监听的最大文件描述符个数为1024个
- 监听集合和满足监听集合是同一个集合(因为是传入传出参数)，所以需要将原有集合进行保存
- 由于不知道满足监听的文件描述符是哪几个，所以需要轮询遍历的方式去检查。 时间复杂度是On
#### poll
- 突破了最大监听1024个文件描述符的限制，可通过修改page文件实现
- 实现了监听集合和满足监听集合的分离，自带了一个数组
- 搜索范围变小，只需要轮询遍历数组内的元素即可，但时间复杂度仍是On
#### epoll
- 不再是轮询方式遍历整个满足监听的文件描述符集，而是采用回调方式来遍历就绪的文件描述符的集合，时间复杂度为O1。 其有三个函数，分别是epoll_create创建红黑树的树根，epoll_ctl向树上添加结点，epoll_wait监听事件。epoll有两种模式，水平触发LT和边沿触发ET，水平触发会循环读完缓冲区内的数据，而边沿触发只会读一次。 最推荐的是epoll的非阻塞IO模型，即结合ET模型和设置read非阻塞读，这样也能实现水平触发的功能，但调用epoll函数的次数更少，更高效。

其中select/poll/epoll均在listen之后，accetp之前进行调用。 这一点需要注意。


### 4. epoll反应堆
&emsp;&emsp;我可以说epoll反应堆没太懂么。。 不过大致的思想即，它与epoll的区别是： 1)在写回数据给客户端之前需要判断一下客户端是否可写，原因是如果客户端的滑动窗口缓冲区被填满，则服务器端不能再写数据回去。 2)用到了回调函数，* ptr中的结构体中有一个回调函数，回调函数的参数又是它本身的结构体类型。

### 5. 线程池
&emsp;&emsp;线程池的基本思想是：在多线程并发服务器的基础之上，取而代之的是它不是一个一个的创建线程，而是一次创建多个线程，这多个线程就被称为线程池，线程池是逻辑上的概念，并不是真正存在线程池。线程池运用到了条件变量，锁，管理者线程等机制，线程池提高了程序执行的效率，避免了线程反复创建和销毁的过程。

### 6. 本地套接字
&emsp;&emsp;主要是用于本地的应用程序之间进行通信，与网络套接字有些许不同，它需要绑定客户端和服务器端的文件名，需要注意的点是文件名的长度，用到的函数是offsetof。

### 7. 生产者消费者模型
&emsp;&emsp;生产者消费者模型它的基本思想是：生产者(线程)和消费者(线程)对应着同一块公共资源，它们通过条件变量和锁的机制来实现线程同步，消费者通过pthread_cond_wait函数来阻塞等待公共资源，而生产者则通过pthread_cond_sign/broadcast函数来唤醒正在阻塞中的线程，随后消费者开始抢夺公共资源，先抢到锁的就先访问资源，访问结束后再立即解锁。
<p align="right">by2021.4.15</p>

### 8. 信号量
