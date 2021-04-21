//借助条件变量模拟 生产者-消费者 问题
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

//链表作为共享数据，需要被互斥量保护
struct msg{
	struct msg* next;
	int num;
};

struct msg* head;
struct msg* mp;

//在全局位置，静态初始化一个条件变量和一个互斥量
pthread_cond_t has_product = PTHREAD_COND_INITIALIZER;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* consumer(void* p)
{
	for(;;){
		pthread_mutex_lock(&lock);
		while(head == NULL){	//头指针为空，说明没有结点，可以为if吗
			pthread_cond_wait(&has_product, &lock);  //调用这个函数时就解锁，并且阻塞等待，直至被唤醒，唤醒之后就加锁,当没有共享资源时，阻塞在这里等待被唤醒。
		}
		mp = head;
		head = mp->next;	//模拟消费掉一个产品,删除一个结点
		pthread_mutex_unlock(&lock);

		printf("-consume ---%d\n", mp->num);
		free(mp);
		sleep(rand() % 5);
	}
}

void* producer(void* p)
{
	for(;;){
		mp = malloc(sizeof(struct msg));
		mp->num = rand() % 1000 + 1;	//模拟生产一个产品
		printf("--produce -- %d\n", mp->num);

		pthread_mutex_lock(&lock);
		mp->next = head;		//头插法插入
		head = mp;
		pthread_mutex_unlock(&lock);

		pthread_cond_signal(&has_product);//发送一个信号，将等待在该条件变量上的一个线程唤醒,有了该共享资源，消费者之间才开始竞争
		sleep(rand() % 5);
	}
}

int main()
{
	pthread_t pid, cid;		//pid 生产者  cid 消费者
	srand(time(NULL));		//种一个随机数种子,随着时间的变化，种子会有变化，产生的随机值也就不一样了

	pthread_create(&pid, NULL, producer, NULL);
	pthread_create(&cid, NULL, consumer, NULL);

	pthread_join(pid, NULL);
	pthread_join(cid, NULL);

	return 0;
}
