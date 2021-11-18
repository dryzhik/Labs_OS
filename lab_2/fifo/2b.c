#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

#define FIFO_FILE "fifo"

int main(int argc, char** argv)
{
	unlink(FIFO_FILE);
	int fifo = mkfifo(FIFO_FILE, 0777);
	
	if(fifo == -1)
	{
		printf("%s\n","CREATION ERROR");
	}

	int status = 0;
	pid_t pid = fork();

	switch(pid)
	{
		case -1:
		
			printf("%s\n","FORK ERROR");
			break;

		case 0:
		{
			sleep(1);
			printf("%s\n","CHILD: OH! NOOOO!");
			
			int reader = open(FIFO_FILE, O_RDONLY);
			
			if(reader == -1)
			{
				printf("%s\n","OPENING ERROR");
				return -1;
			}
			
			sleep(5);			

			char buf[256];
			time_t cur_tm = time(NULL);
			
			read(reader,&buf, sizeof(char)*256);
			close(reader);
		
			printf("CHILD'S time: %sRecieved string: %s\n", ctime(&cur_tm), buf);
			
			break;
		}

		default:
		{
			printf("%s\n","PARENT: LUKE, I'm your father!");

			int writer = open(FIFO_FILE, O_WRONLY);

			if(writer == -1)
			{
				printf("%s\n","OPENING ERROR");
				return -1;
			}
			
			char line[256];
			time_t tm = time(NULL);
			int id = getpid();
			char num[10];
			sprintf(num, "%d]", id);                    	

			strcat(line, "[PARENT's time: ");
			strcat(line, ctime(&tm));
			strcat(line, "PARENT's pid: ");
			strcat(line, num);
			
			write(writer, line,sizeof(char)*256); 
			close(writer);                               	
			
			wait(&status);
			
			break;
		}
	}
	
	return 0;
}
