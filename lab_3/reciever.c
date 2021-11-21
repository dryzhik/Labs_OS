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

#define MEM_FILE "mem_file"

int main(int argc, char** argv)
{
	key_t key = ftok(MEM_FILE, 1);
	int shid = shmget(key, 32, 0666);

	if(shid < 0)
	{
		printf("%s\n","CREATION ERROR");
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
		printf("Reciever's time: %sReciever's pid: %d\nRecieved string: %s\n", ctime(&tm), id, shptr);	
		sleep(1);
	}	
	
	shmdt(shptr);

	return 0;
}
