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
	int shid = shmget(key, 32, 0666);
	int semid = semget(key, 1, 0666);

	if(shid < 0)
	{
		printf("%s\n","SHID CREATION ERROR");
		return -1;
	}

	if(semid == -1)
	{
		printf("%s\n", "SEMID CREATION ERROR");
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
		semop(semid, &sem_lock, 1);
		time_t tm = time(NULL);
		int id = getpid(); 
		printf("Reciever's time: %sReciever's pid: %d\nRecieved string: %s\n", ctime(&tm), id, shptr);	
		sleep(1);
		semop(semid, &sem_open, 1);
	}	
	
	shmdt(shptr);

	return 0;
}

