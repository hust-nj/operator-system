#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#define shm_key 1234
#define sem_key 4321
#define rwsz (1<<12)
#define bufsz (1<<19)

int create_Sem(int key, int size);
void destroy_Sem(int semid);
int get_Sem(int key, int size);
void set_N(int semid, int index, int n);
int create_Shm(int key, int size);
int get_Shm(int key, int size);
void destroy_Shm(int shmid);
void P(int semid, int index);
void V(int semid, int index);

struct Buf{
	char buff[bufsz];
	int once;
	int start;//count the read times
};