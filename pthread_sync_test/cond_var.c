#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

//以链表作为公共资源
struct linknode{
	int val;
	struct linknode* next;
};

struct linknode* head = NULL;		//头结点
struct linknode* msg = NULL;		//辅助结点

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void* producer(void* arg)
{
	for(;;){
		//添加节点 头插法，模拟生产
		msg = malloc(sizeof(struct linknode));
		int num = rand() % 1000 ;
		msg->val = num;
		printf(" --- product -- %d\n", num);
		
		pthread_mutex_lock(&mutex);		//公共区域需要加锁
		msg->next = head;
		head = msg;
		pthread_mutex_unlock(&mutex);

		pthread_cond_signal(&cond);
		sleep(rand() % 4);
	}
}

void* consumer(void* arg)
{
	for(;;){
		pthread_mutex_lock(&mutex);
		while(head == NULL){
			pthread_cond_wait(&cond, &mutex);	//这个函数的三个作用，
		}
		//删除结点，模拟消费
		msg = head;
		head = msg->next;
		pthread_mutex_unlock(&mutex);

		printf("--consume --%d\n", msg->val);
		free(msg);
		sleep(rand() % 4);
	}
}

int main()
{
	pthread_t cid,pid;

	srand(time(NULL));

	pthread_create(&cid, NULL, consumer, NULL); 	//子线程的消费者函数
	pthread_create(&pid, NULL, producer, NULL);	//子线程的生产者函数

	pthread_join(cid, NULL);
	pthread_join(pid, NULL);

	return 0;
}
