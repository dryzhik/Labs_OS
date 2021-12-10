#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

pthread_rwlock_t lock;
int count = 0;

void* writing()
{
	while(1)
	{
		pthread_rwlock_wrlock(&lock);
		count++;
		pthread_rwlock_unlock(&lock);
		sleep(1);
	}
}

void* reading()
{
	while(1)
	{
		pthread_rwlock_rdlock(&lock);
		printf("My tid: %u. Now count is: %d\n", pthread_self(), count);
		pthread_rwlock_unlock(&lock);
		sleep(1);
	}
}

int main(int argc, char** argv)
{
	
	pthread_t readers[10];	
	pthread_t writer;		

	pthread_rwlock_init(&lock, NULL);
	
	pthread_create(&writer, NULL, writing, NULL);
	
	for(int i = 0; i < 10; ++i)
        {
                pthread_create(&readers[i], NULL, reading, NULL);
        }

	for(int i = 0; i < 10; ++i)
        {
                pthread_join(readers[i], NULL);
        }

	pthread_join(writer, NULL);

	pthread_rwlock_destroy(&lock);
	
	return 0;
}
