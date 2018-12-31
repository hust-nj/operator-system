#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

union semun {
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
};

int create_Sem(int key, int size)
{
	int id;
	id = semget(key, size, IPC_CREAT|0666);//脗麓脗麓脗陆脗篓size脗赂脙露脙聬脙鈥γ偮好冣γ兟伱偮
	if(id < 0) {//脙鈥γ兟惷偮睹兟徝兣犆冣∶偮访兟泵偮疵偮疵偮矫偮偮趁冣懊偮姑偮
		printf("create sem %d,%d error\n", key, size);//脗麓脗麓脗陆脗篓脙艩脗搂脗掳脙艙脗拢脗卢脗麓脙虏脙鈥溍偮∶偮疵兟兣矫兟
	}
	return id;
}

void destroy_Sem(int semid)
{
	int res = semctl(semid,0,IPC_RMID,0);//脗麓脙鈥溍兟徝偮得兟嵜偮趁冣撁兟惷冣懊偮久偮趁兟矫兟惷冣γ偮好冣γ兟伱偮
	if (res < 0) {//脙鈥γ兟惷偮睹兟徝兣犆冣∶偮访兟泵冣懊偮久偮趁兟矫偮趁冣懊偮姑偮
		printf("destroy sem %d error\n", semid);//脙聬脙鈥γ偮好冣γ兟伱偮棵冣懊偮久偮趁兟矫兣犆偮偮懊兣撁偮Ｃ偮兣犆兟っ偮趁兟睹兟惷冣γ兟徝偮
	}
	return;
}

int get_Sem(int key, int size)
{
	int id;
	id = semget(key, size, 0666);//脗禄脙卤脙藛脗隆脙鈥櫭冣樏偮久偮偮疵偮疵偮矫偮偮得冣灻兟惷冣γ偮好冣γ兟伱偮
	if(id < 0) {
		printf("get sem %d,%d error\n", key, size);
	}
	return id;
}

void set_N(int semid, int index, int n)
{
	union semun semopts; 
	semopts.val = n;//脙鈥懊兟偮睹偮⊿ETVAL脗碌脙鈥灻冣撁偮得兣矫偮猲
	semctl(semid,index,SETVAL,semopts);//脗鲁脙碌脙艩脗录脗禄脗炉脙聬脙鈥γ偮好冣γ兟伱偮棵偮Ｃ偮兟惷冣γ偮好冣γ兟伱偮棵偮泵兟犆偮好冣γ兣矫偮猧ndex
	return;
}

void P(int semid, int index)
{
	struct sembuf sem;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮裁冣劉脙鈥斆兟访兣犆兟矫冣斆兟
	sem.sem_num = index;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮泵兟犆偮好冣
	sem.sem_op = -1;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮裁冣劉脙鈥斆兟访偮Ｃ偮-1脙沤脗陋P脗虏脙鈩⒚冣斆兟
	sem.sem_flg = 0;//脗虏脙鈩⒚冣斆兟访偮泵兟偮济冣∶偮Ｃ偮0脗禄脙虏IPC_NOWAIT脗碌脙藛
	semop(semid,&sem,1);//1:脗卤脙颅脙艩脗戮脙鈥撁偮疵兟惷兟惷兤捗兟济兟伱兟偮得冣灻偮该兟睹兣犆兟
	return;
}

void V(int semid, int index)
{
	struct sembuf sem;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮裁冣劉脙鈥斆兟访兣犆兟矫冣斆兟
	sem.sem_num = index;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮泵兟犆偮好冣
	sem.sem_op =  1;//脙聬脙鈥γ偮好冣γ兟伱偮棵偮裁冣劉脙鈥斆兟访偮Ｃ偮1脙沤脗陋V脗虏脙鈩⒚冣斆兟
	sem.sem_flg = 0; //脗虏脙鈩⒚冣斆兟访偮泵兟偮济冣
    semop(semid,&sem,1);//1:脗卤脙颅脙艩脗戮脙鈥撁偮疵兟惷兟惷兤捗兟济兟伱兟偮得冣灻偮该兟睹兣犆兟
	return;
}

