#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

void changeModeForUser(int len, char buf[len], char** argv)
{
	const char* file = argv[2];

	printf("%s  %s\n","user", argv[1]);	
	
	if(buf[3] == 'a')
	{
		
	}
}

void changeModeForGroup(int len, char buf[len], char** argv)
{
	printf("%s\n","group");
}

void changeModeForOthers(int len, char buf[len], char** argv)
{
	printf("%s\n","others");
}

int main(int argc, char** argv)
{

	int len = strlen(argv[1]);
	char buf[len+1];
	strcat(buf, argv[1]);
	if(len > 6)
	{
		printf("%s\n", "Слишком много флагов!\nДля справки воспользуйтесь флагом -h");
	}
	else if(len < 4)
	{
		int o = 0;
		while((o = getopt(argc, argv,"h")) != -1)
		{
			switch(o)
			{
				case 'h':
					printf("%s\n","Справка");
					break;
				case '?':
					printf("%s\n","Неверный ключ.Посмотрите справку");
					break;
				default:
					printf("%s  %s  %s\n","Ошибка! Посмотрите справку", optarg,argv[1]);
			}
		}
	}
	else if(buf[1] == '-')
	{		
		switch(buf[2])
		{
			case 'u':
				changeModeForUser(len, buf, argv);
				break;
		
			case 'g':
				changeModeForGroup(len, buf, argv);
				break;
		
			case 'o':
				changeModeForOthers(len, buf, argv);
				break;
		
			default:
		       		printf("%c\n%s\n",buf[2],"Неверный владелец.\nВладельцами могут быть только:\nuser (u), group(g), others (o)\n Откройте справку для уточнения деталей");
				break;
		}
	}
	
	else 
	{
		printf("%s\n","Откройте справку!");
	}		

	return 0;
}
