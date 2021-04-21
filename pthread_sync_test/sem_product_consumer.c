//信号量实现生产者，消费者模型

#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <semaphore.h>

#define NUM 5

int queue[NUM];				//全局数组实现环形队列
sem_t blank_number, product_number;	//空格子信号量和产品信号量

void* producer(void* arg)
{
	int i = 0;

	while(1){
		sem_wait(&blank_number);	//生产者将空格子数--，为0则阻塞等待
		queue[i] = rand() % 1000 + 1;   //生产一个产品
		printf("----product---%d\n", queue[i]);
		sem_post(&product_number);	//将产品数++

		i = (i+1) % NUM;		//借助下标实现队列
		sleep(rand() % 3);
	}
}

void* consumer(void* arg)
{
        int i = 0;

        while(1){
                sem_wait(&product_number);      //消费者将产品数--，为0则阻塞等待
                printf("-consum--%d\n", queue[i]);
                queue[i] = 0;   		//消费一个产品
                sem_post(&blank_number);      	//消费点以后，将空格子数++

                i = (i+1) % NUM;                //借助下标实现队列
                sleep(rand() % 3); 
        }   
}


int main()
{
	 pthread_t pid, cid;             //pid 生产者  cid 消费者
         srand(time(NULL));              //种一个随机数种子,随着时间的变化，种子会有变化，产生的随机值也就不一样了
 
	 sem_init(&blank_number, 0, NUM);	//初始化空格子数量为5
	 sem_init(&product_number, 0, 0);	//产品数为0

         pthread_create(&pid, NULL, producer, NULL);
         pthread_create(&cid, NULL, consumer, NULL);
 
         pthread_join(pid, NULL);
         pthread_join(cid, NULL);
 
	 sem_destroy(&blank_number);
	 sem_destroy(&product_number);

         return 0;

}
