#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

void print_reference()
{
	printf("CHMOD\n");
	printf("Для того, чтобы использовать возможности данной утилиты введите команду в формате: ./chmod -[ugo][ar][rwx][rwx][rwx] file\n");
	printf("Флаги [ugo] используются для задания прав для пользователя, группы и других соответственно ");
	printf("Флаги [ar] используются для добавления и удаления прав соответственно");
	printf("Флаги [rwx] используются для задания прав на чтение, запись и исполнение соответственно");
	printf("File, для которого необходимо сменить права, должен существовать");
	printf("Максимальное количество задаваемых флагов - 5, минимальное - 3. Например:\nchmod -uarwx file_name (добавляет пользователю права на чтение, запись и исполнение для указанного файла)\nchmod -urw file_name (убирает права пользователя на запись для указанного файла)");
	printf("Флаг -h используется для справки");
}

void changeModeForUser(char* file, int oper[2], int attr[3])
{
	struct stat buf;
	stat(file, &buf);
	mode_t modes = buf.st_mode;
	
	if((oper[0] == 1) && (oper[1] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes | S_IRUSR);
			modes |= S_IRUSR;
		}
		if(attr[1] == 1)
		{
			chmod(file, modes | S_IWUSR);
			modes |= S_IWUSR;
		}
		if(attr[2] == 1)
		{
			chmod(file, modes | S_IXUSR);
			modes |= S_IXUSR;
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "1ch Некорректный ввод.\nИспользуйте флаг -h для справки");
		}
	}

	else if((oper[1] == 1) && (oper[0] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes & (~S_IRUSR));
			modes = modes & (~S_IRUSR);
		}
		if(attr[1] == 1)
		{
			chmod(file, modes & (~S_IWUSR));
			modes = modes & (~S_IWUSR);
		}
		if(attr[2] == 1)
		{
			chmod(file, modes & (~S_IXUSR));
			modes = modes & (~S_IXUSR);
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки.");
		}
	}
	
	else
	{
		printf("%s\n","Некорректный ввод.\nИспользуйте флаг -h для справки.");
	}	
}

void changeModeForGroup(char* file, int oper[2], int attr[3])
{
	struct stat buf;
	stat(file, &buf);
	mode_t modes = buf.st_mode;
	
	if((oper[0] == 1) && (oper[1] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes | S_IRGRP);
			modes |= S_IRGRP;
		}
		if(attr[1] == 1)
		{
			chmod(file, modes | S_IWGRP);
			modes |= S_IWGRP;
		}
		if(attr[2] == 1)
		{
			chmod(file, modes | S_IXGRP);
			modes |= S_IXGRP;
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки");
		}
	}

	else if((oper[1] == 1) && (oper[0] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes & (~S_IRGRP));
			modes = modes & (~S_IRGRP);
		}
		if(attr[1] == 1)
		{
			chmod(file, modes & (~S_IWGRP));
			modes = modes & (~S_IWGRP);
		}
		if(attr[2] == 1)
		{
			chmod(file, modes & (~S_IXGRP));
			modes = modes & (~S_IXGRP);
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки.");
		}
	}
	
	else
	{
		printf("%s\n","Некорректный ввод.\nИспользуйте флаг -h для справки.");
	}	
}

void changeModeForOthers(char* file, int oper[2], int attr[3])
{
	struct stat buf;
	stat(file, &buf);
	mode_t modes = buf.st_mode;
	
	if((oper[0] == 1) && (oper[1] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes | S_IROTH);
			modes |= S_IROTH;
		}
		if(attr[1] == 1)
		{
			chmod(file, modes | S_IWOTH);
			modes |= S_IWOTH;
		}
		if(attr[2] == 1)
		{
			chmod(file, modes | S_IXOTH);
			modes |= S_IXOTH;
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки");
		}
	}

	else if((oper[1] == 1) && (oper[0] == 0))
	{
		if(attr[0] == 1)
		{
			chmod(file, modes & (~S_IROTH));
			modes = modes & (~S_IROTH);
		}
		if(attr[1] == 1)
		{
			chmod(file, modes & (~S_IWOTH));
			modes = modes & (~S_IWOTH);
		}
		if(attr[2] == 1)
		{
			chmod(file, modes & (~S_IXOTH));
			modes = modes & (~S_IXOTH);
		}
		if((attr[0] == 0) && (attr[1] == 0) && (attr[2] == 0))
		{
			printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки.");
		}
	}
	
	else
	{
		printf("%s\n","Некорректный ввод.\nИспользуйте флаг -h для справки.");
	}	
}

int main(int argc, char** argv)
{
	if((argc != 3) || (access(argv[2],F_OK) == -1))
	{
		printf("%s\n", "Неккоретный ввод.\nИспользуйте флаг -h для получения справки.\nЕсли вы убедились в правильности ввода,проверьте имя файла и его существование.");
		return -1;
	}
	
	int owner[3] = {0, 0, 0};
	int operation[2] = {0, 0};
	int attribute[3] = {0, 0, 0};
	char* file = argv[2];
	
	int opt = 0;

	while((opt = getopt(argc, argv, "hugoarwx")) != -1)
	{
		switch(opt)
		{
			case 'h':
				print_reference();
				break;
			case 'u':
				owner[0] = 1;
				break;
			case 'g':
				owner[1] = 1;
				break;
			case 'o':
				owner[2] = 1;
				break;
			case 'r':
			{
				if((operation[0] == 0) && (operation[1] == 0))
				{
					operation[1] = 1;
				}
				else
				{
					attribute[0] = 1;
				}
				break;
			}
			case'a':
			{
				if(operation[1] == 0)
				{
					operation[0] = 1;
				}
				break;
			}
			case 'w':
				attribute[1] = 1;
				break;
			case 'x':
				attribute[2] = 1;
				break;
			case'?':
				printf("%s\n", "Некорректный ввод.\nИспользуйте флаг -h для справки.");
		}
	}
	
	if((owner[0] == 1) && (owner[1] == 0) && (owner[2] == 0))
	{
		changeModeForUser(file, operation, attribute);
	}
	
	else if((owner[1] == 1) && (owner[0] == 0) && (owner[2] == 0))
	{
		changeModeForGroup(file, operation, attribute);
	}

	else if((owner[2] == 1) && (owner[0] ==0) && (owner[1] == 0))
	{
		changeModeForOthers(file, operation, attribute);
	}
	
	else
	{
		printf("%s\n","Некорректный ввод.\n Используйте флаг -h для получения справки.");
		return -1;
	}
	return 0;
}
