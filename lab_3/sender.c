#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <signal.h>

#define MEM_FILE "mem_file"

int main(int argc, char** argv)
{
	key_t key = ftok(MEM_FILE, 1);
	int shid = shmget(key, 32, IPC_CREAT|0666);

	if(shid == -1)
	{
		perror("CREATION ERROR");
		return -1;
	}

	void* shptr = shmat(shid, NULL, 0);

	if(shptr == (void*)-1)
	{
		printf("%s\n","ATTACH ERROR");
		return -1;
	}

	while(1)
	{
		time_t tm = time(NULL);
		int id = getpid();
		char * line = calloc(100,sizeof(int));
		sprintf(line, "Sender's time: %sSender's pid: %d\n", ctime(&tm), id);	
		memcpy(shptr, line, 100*sizeof(int));
		sleep(1);
		free(line);
	}	
	
	shmdt(shptr);
	shmctl(shid, IPC_RMID, NULL);
	return 0;
}
