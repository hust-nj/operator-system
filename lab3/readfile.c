#include "share.h"

int main()
{
	int sem_id = get_Sem(sem_key, 2);//分配大小为2的信号灯
	int shm_id = get_Shm(shm_key, sizeof(struct Buf));
	struct Buf *pshm = shmat(shm_id, NULL, SHM_R|SHM_W);
	char *pbuff = pshm->buff;

	FILE *in;
	if((in = fopen("in", "rb")) == NULL)
	{
		fprintf(stderr, "Can't open file \"in\"\n");
		exit(1);
	}
	
	// int start = 0;
	pshm->start = 0;
	while(pshm->once == rwsz)
	{
		pshm->start = (pshm->start + 1) % (bufsz / rwsz);
		// printf("once\n");
		// printf("sem_id1's value = %d\n", semctl(sem_id, 1, GETVAL));
		P(sem_id, 1);
		// printf("sem_id1 = %d\n", sem_id);
		pshm->once = fread(pbuff + pshm->start * rwsz, 1, rwsz, in);
		// printf("%d\n", pshm->once);
		// printf("pshm->once = %d\n", pshm->once);
		V(sem_id, 0);
	}
	fclose(in);
	return 0;
}