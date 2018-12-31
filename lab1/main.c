#include<stdio.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<string.h>

pid_t p1, p2;

void my_func(int sig_no)
{
    if(sig_no == SIGUSR1)
    {
    	printf("Child process1 is killed by parents.\n");
    	exit(0);
    }
    if(sig_no == SIGUSR2)
   	{
   		printf("Child process2 is killed by parents.\n");
   		exit(0);
   	}
    if(sig_no == SIGINT)
    {
    	kill(p1, SIGUSR1);
    	kill(p2, SIGUSR2);
    }
}



int main(void)
{
    int pipefd[2];
    pipe(pipefd);//创建无名管道

    
    p1 = fork();

    if(p1 == 0)//子进程1
    {
        if(signal(SIGINT, SIG_IGN) == SIG_ERR)
			printf("Can't catch SIGINT\n");

	    if(signal(SIGUSR1, my_func) == SIG_ERR)
	        printf("Can't catch SIGUSR1.\n");


        //传递"I send you x times"
        int cnt = 0;
        char str[50];
        while(1)
        {
        	sleep(1);
        	sprintf(str, "I send you %d times", ++cnt);
        	if(write(pipefd[1], str, strlen(str)) == -1)
        		printf("write is interrupted\n");
        }

        
    }
    else
    {
        p2 = fork();
        if(p2 == 0)//子进程2
        {
	    	if(signal(SIGINT, SIG_IGN) == SIG_ERR)
	    		printf("Can't catch SIGINT\n");
		    if(signal(SIGUSR2, my_func) == SIG_ERR)
		        printf("Can't catch SIGUSR2.\n");

		    char buf[50];
		    while(1)
		    {
		    	if(read(pipefd[0], buf, sizeof(buf)) == -1)
		    		printf("read is interrupted\n");
		    	printf("%s\n", buf);
		    }
        }
        else//父进程
        {
		    if(signal(SIGINT, my_func) == SIG_ERR)
		        printf("Can't catch SIGINT.\n");
		    wait(0);
		    wait(0);
		    close(pipefd[0]);
		    close(pipefd[1]);
	        printf("Parent process is killed!\n");
        }
    }

    return 0;

}
