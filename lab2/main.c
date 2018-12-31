#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/types.h>
#include<linux/sem.h>

//信号灯、线程句柄定义
static int semid;//缓冲区为空
pthread_t p1, p2;
int a;
int flag = 1;

//线程执行函数定义
void *subp1(void *arg);
void *subp2(void *arg);


void set_N(int semid, int index, int n);
int create_Sem(int key, int size);
void destroy_Sem(int semid);
void P(int semid, int index);
void V(int semid, int index);


void *subp1(void * arg1)
{
	for(int i = 1; i <= 100; ++i)
	{
		P(semid, 1);
		a += i;
		V(semid, 0);
	}
}

void *subp2(void *arg2)
{
	for(int i = 1; i<= 100; ++i)
	{
		P(semid, 0);
		printf("a当前的值为 %d\n", a);
		V(semid, 1);
	}
}

int main(void)
{
	semid = create_Sem(0, 2);
	set_N(semid, 0, 0);
	set_N(semid, 1, 1);
	//创建线程
	if(pthread_create(&p1, NULL, subp1, NULL))
	{
		fprintf(stderr, "线程1创建错误\n");
		exit(-1);
	}
	if(pthread_create(&p2, NULL, subp2, NULL))
	{
		fprintf(stderr, "线程2创建错误\n");
		exit(-2);
	}

	//等待线程退出
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);

	destroy_Sem(semid);
	return 0;
}