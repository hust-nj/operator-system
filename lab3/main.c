#include "share.h"

int main()
{
	int shm_id = create_Shm(shm_key, sizeof(struct Buf));
	printf("shm_id = %d\n", shm_id);
	struct Buf *pshm = shmat(shm_id, NULL, SHM_R|SHM_W);
	pshm->once = rwsz;
	int sem_id = create_Sem(sem_key, 2);//大小为2的信号灯
	set_N(sem_id, 0, 0);//内容个数
	set_N(sem_id, 1, bufsz / rwsz);//空闲个数
	pid_t readp, writep;
	if((readp = fork()) == 0)//读进程
	{
		printf("create p1\n");
		execl("./readfile", "readfile", NULL);
	}
	else if((writep = fork()) == 0)//写进程
	{
		printf("create p2\n");
		execl("./writefile", "writefile", NULL);
	}
	else
	{
		waitpid(readp, 0, 0);
		waitpid(writep, 0, 0);
		destroy_Shm(shm_id);
		destroy_Sem(sem_id);
	}

	return 0;
}