#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char** argv)
{
	int fd[2];            //массив для файловых дескрипторов для чтения и записи
	int pip =  pipe(fd);  //создание pipe
	int status = 0;	

	if(pip == -1) 
	{
		printf("%s\n","CREATION ERROR");
	}
	
	pid_t pid = fork(); 
	
	switch(pid)
	{
		case -1:
		      
			printf("%s\n","FORK ERROR");
			break;

		case 0:
		{
			sleep(5);
			printf("%s\n","CHILD: Hi! I'm child");
			char buf[256];
			time_t cur_tm = time(NULL);
			
			close(fd[1]);
			read(fd[0],&buf, sizeof(char)*256);
			close(fd[0]);
			
			printf("CHILD'S time: %sRecieved string: %s\n", ctime(&cur_tm), buf);
			
			break;
		}

		default:
		{
			printf("%s\n","PARENT: Hi! I'm parent!");
			
			char line[256];
			time_t tm = time(NULL);
			int id = getpid();
			char num[10];
			sprintf(num, "%d]", id);                    //запись результата в строку, а не в поток 			

			strcat(line, "[PARENT's time: ");
			strcat(line, ctime(&tm));
			strcat(line, "PARENT's pid: ");
			strcat(line, num);
			
			close(fd[0]);                               //закрытие дескриптора, открытого на чтение
			write(fd[1], (void*)line, sizeof(char)*256); 
			close(fd[1]);                               //закрытие дескриптора, открытого на запись	
			
			wait(&status);
			
			break;
		}
	}

	return 0;
}
