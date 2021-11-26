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
#include <sys/sem.h>

#define MEM_FILE "mem_file"

struct sembuf sem_lock = {0, -1, 0}, sem_open = {0, 1, 0};

int main(int argc, char** argv)
{
	key_t key = ftok(MEM_FILE, 1);             
	int shid = shmget(key, 32, IPC_CREAT|0666); 
	int semid = semget(key, 1, IPC_CREAT|0666);
							   
	if(shid == -1)
	{
		printf("%s\n","SHID CREATION ERROR");
		return -1;
	}
	
	if(semid == -1)
	{
		printf("%s\n","SEMID CREATION ERROR");
	}
					   
	void* shptr = shmat(shid, NULL, 0);
	
	if(shptr == (void*)-1)
	{
		printf("%s\n","ATTACH ERROR");
		return -1;
	}
	
	semop(semid, &sem_open, 1);
	
	if(semctl(semid, 0, GETVAL) == 0)
	{
		printf("%s\n", "Sender's already exist");
		return -1;
	}

	while(1)
	{
		semop(semid, &sem_lock, 1);
		time_t tm = time(NULL);
		int id = getpid();
		char* line = calloc(100,sizeof(int));
		sprintf(line, "Sender's time: %sSender's pid: %d\n", ctime(&tm), id);	
		memcpy(shptr, line, 100*sizeof(int));
		sleep(1);
		free(line);
		semop(semid, &sem_open, 1);
	}	
	
	semctl(semid, 0, IPC_RMID);
	shmdt(shptr);
	shmctl(shid, IPC_RMID, NULL);
	
	return 0;
}
