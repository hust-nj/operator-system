#include "share.h"

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int create_Sem(int key, int size)
{
	int id;
	id = semget(key, size, IPC_CREAT|0666);//创建size个信号量
	if(id < 0) {//判断是否创建成功
		printf("create sem %d,%d error\n", key, size);//创建失败，打印错误
	}
	return id;
}

void destroy_Sem(int semid)
{
	int res = semctl(semid,0,IPC_RMID,0);//从系统中删除信号量
	if (res < 0) {//判断是否删除成功
		printf("destroy sem %d error\n", semid);//信号量删除失败，输出信息
	}
	return;
}

int get_Sem(int key, int size)
{
	int id;
	id = semget(key, size, 0666);//获取已经创建的信号量
	if(id < 0) {
		printf("get sem %d,%d error\n", key, size);
	}
	return id;
}

void set_N(int semid, int index, int n)
{
	union semun semopts; 
	semopts.val = n;//设定SETVAL的值为n
	semctl(semid,index,SETVAL,semopts);//初始化信号量，信号量编号为index
	return;
}

void P(int semid, int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op = -1;//信号量操作，-1为P操作
	sem.sem_flg = 0;//操作标记：0或IPC_NOWAIT等
	semop(semid,&sem,1);//1:表示执行命令的个数
	return;
}

void V(int semid, int index)
{
	struct sembuf sem;//信号量操作数组
	sem.sem_num = index;//信号量编号
	sem.sem_op =  1;//信号量操作，1为V操作
	sem.sem_flg = 0; //操作标记
        semop(semid,&sem,1);//1:表示执行命令的个数
	return;
}

int create_Shm(int key, int size)
{
	int id;
	id = shmget(key, size, IPC_CREAT|0666);//创建size个信号量
	if(id < 0) {//判断是否创建成功
		printf("create shm %d,%d error\n", key, size);//创建失败，打印错误
	}
	return id;
}

int get_Shm(int key, int size)
{
	int id;
	id = shmget(key, size, 0666);//获取已经创建的信号量
	if(id < 0) {
		printf("get shm %d,%d error\n", key, size);
	}
	return id;
}

void destroy_Shm(int shmid)
{
	int res = shmctl(shmid,IPC_RMID,0);//从系统中删除信号量
	if (res < 0) {//判断是否删除成功
		printf("destroy shm %d error\n", shmid);//信号量删除失败，输出信息
	}
	return;
}