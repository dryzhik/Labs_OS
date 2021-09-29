#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void exit_msg()
{
	printf("Atexit handler has worked from process %d\n", getpid());
}

int main(int argc, char** argv)
{	
	atexit(exit_msg);
	
	int status = 0;
		
	pid_t pid = fork();
	switch(pid)
	{
		case -1:
		     
		     perror("fork");
		     exit(1);
		
		case 0:
		     
		     printf("CHILD PROCESS: Hi! I'm child.\n");
		     printf("CHILD PROCESS: My pid is: %d\n", getpid());     
		     printf("CHILD PROCESS: My parent's pid is: %d\n", getppid());
		     exit(1);

		default:
		    
		     printf("PARENT PROCESS: Hi! I'm parent.\n");
		     printf("PARENT PROCESS: My  pid is: %d\n", getpid());
		     printf("PARENT'S CHILD PROCESS: My child's pid is: %d\n", pid);
		     wait(&status);
	}
	
	return 0;
}
