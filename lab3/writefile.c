#include "share.h"

int main()
{
	int sem_id = get_Sem(sem_key, 2);
	int shm_id = get_Shm(shm_key, sizeof(struct Buf));
	struct Buf *pshm = shmat(shm_id, NULL, SHM_R|SHM_W);
	char *pbuff = pshm->buff;

	FILE *out;
	if((out = fopen("out", "wb")) == NULL)
	{
		fprintf(stderr, "Can't open file \"out\"\n");
		exit(1);
	}

	int end = 0;
	
	while(1)
	{
		end = (end + 1) % (bufsz / rwsz);
		// printf("P0\n");
		P(sem_id, 0);
		// printf("sem_id2 = %d\n", sem_id);
		// if(pshm->once == 0 && cnt == pshm->cnt)
		// {
			// printf("case 1\n");
			// V(sem_id, 1);
			// break;
		// }
		if(pshm->once < rwsz && end == pshm->start)
		{
			// printf("case 2\n");
			fwrite(pbuff + end * rwsz, pshm->once, 1, out);
			V(sem_id, 1);
			break;
		}
		else
		{
			// printf("case 3\n");
			fwrite(pbuff + end * rwsz, rwsz, 1, out);
			V(sem_id, 1);
		}
	}

	fclose(out);
	return 0;
}