#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

pthread_mutex_t mutex;
pthread_cond_t condition;
int count = 0;

void* writing()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		count++;
		pthread_cond_broadcast(&condition);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}
}

void* reading()
{
	while(1)
	{
		pthread_mutex_lock(&mutex);
		//pthread_cond_wait(&condition, &mutex);
		printf("My tid: %u. Now count is: %d\n", pthread_self(), count);
		pthread_mutex_unlock(&mutex);
		//pthread_cond_signal(&condition);
		sleep(1);
	}
}

int main(int argc, char** argv)
{
	
	pthread_t readers[10];	
	pthread_t writer;		

	pthread_mutex_init(&mutex, NULL);
	pthread_cond_init(&condition, NULL);
	
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

	pthread_mutex_destroy(&mutex);
	
	return 0;
}
