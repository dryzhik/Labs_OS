#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

void handler_l(DIR* dir, struct dirent* ent, char* path, char buf_path[64], int len)
{
	struct stat stats; //структура для работы с атрибутами файлов

	while ((ent = readdir(dir)) != NULL)//пока не закончатся входы в каталог или не будет ошибки
	{
		if (ent->d_name[0] != '.') //проверка на скрытый файл (имя файла не должно начинаться с точки)
		{
			memset(path, 0, sizeof(int) * len);//очищаем path(sizeof(int) потому что в расширениях файлов могут быть цифры, например, у меня  файл massif.out.6502)
			memcpy(path, buf_path, len + 1);// копируем директорию
			memcpy(path + len + 1, ent->d_name, strlen(ent->d_name));// копируем название файла в буфер
			stat(path, &stats);// функция, возвращающая информацию о файле и записывающая ее в структуру stat
			//тип файла
			switch (stats.st_mode & S_IFMT) // st_mode - режим доступа S_IFMT - битовая маска для полей типа файла
			{
			case S_IFBLK:
				printf("b");// является ли файл блочнымм устройством 
				break;

			case S_IFCHR:
				printf("c");// является ли файл символьным устройством 
				break;

			case S_IFDIR:
				printf("d");// является ли файл каталогом 
				break;

			case S_IFLNK:
				printf("l");// является ли файл символьной ссылкой 
				break;

			case S_IFIFO:
				printf("p");// является ли файл каналом FIFO 
				break;

			case S_IFSOCK:
				printf("s");// является ли файл сокетом 
				break;

			default:
				printf("-");
				break;
			}

			//права доступа
			printf((stats.st_mode & S_IRUSR) ? "r" : "-");// пользователь имеет право чтения?
			printf((stats.st_mode & S_IWUSR) ? "w" : "-");// пользователь имеет право записи?
			printf((stats.st_mode & S_IXUSR) ? "x" : "-");// пользователь имеет право выполнения?
			printf((stats.st_mode & S_IRGRP) ? "r" : "-");// группа имеет право чтения?
			printf((stats.st_mode & S_IWGRP) ? "w" : "-");// группа имеет право записи?
			printf((stats.st_mode & S_IXGRP) ? "x" : "-");// группа имеет право выполнения?
			printf((stats.st_mode & S_IROTH) ? "r" : "-");// другие имеют право чтения?
			printf((stats.st_mode & S_IWOTH) ? "w" : "-");// другие имеют право записи?
			printf((stats.st_mode & S_IXOTH) ? "x" : "-");// другие имеют право выполнения?

			printf(" %-2d", stats.st_nlink);              // количество жестких ссылок

			struct passwd* usr = getpwuid(stats.st_uid); //getpwuid() возвращает указатель на структуру,
									  //содержащую поля строки из файла /etc/passwd для записи,										     //совпадающей с идентификатором пользователя uid											    //st_uid идентификатор пользователя-владельца
									  //если 0, то информация о пользователе не найдена

			struct group* group = getgrgid(stats.st_gid); //getgrgid() то же самое, что и getpwuid(), но для группы

			printf(" %s", (usr != 0) ? usr->pw_name : " ");    //имя пользователя
			printf(" %s", (group != 0) ? group->gr_name : " ");//название группы

			printf(" %5ld", stats.st_size);                    //общий размер в байтах

			char* month[] = { "Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec" };
			struct tm* time;                  //структура для времени
			time = localtime(&stats.st_ctime);//преобразование календарного времени в разделенное время данного часового пояса

			printf(" %s", month[time->tm_mon]);//месяц
			printf(" %-2d", time->tm_mday);      //день	
			printf(" %d:", time->tm_hour);      //часы
			printf("%02d", time->tm_min);        //минуты
			printf(" %s\n", ent->d_name);       //название файла

		}
	}
}

int main(int argc, char** argv)
{
	DIR* dir;             //поток директорий
	struct dirent* ent;   //тип структуры, используемый, чтобы возвратить информацию относительно входов в каталог
	bool flag_l = false;  //флаг для проверки нахождения ключа "-l" в аргументах командной строки

	int o = 0;
	while ((o = getopt(argc, argv, "+l")) != -1) //функция парсинга командной строки, которая ищет опцию -l и останавливается сразу, если находит символ не параметра
	{
		switch (o)
		{
		case 'l':
			flag_l = true;
			break;

		case '?':
			return -1;               // getopt() не распознал символ опции (какой-то другой ключ не "-l")
			break;

		default:
			break;
		}
	}


	char* path = calloc(256, sizeof(char));// выделяем память под буфер, в котором будем хранить путь
	int len = 0;//длина строки названия директории
	char buf_path[64];//буфер для хранения директории
	switch (argc)
	{
	case 1:
		dir = opendir(".");  //открываем текущую директорию,т.к аргумент всего один(название программы)
		break;

	case 2:
	{
		if (flag_l)
		{
			dir = opendir(".");//всего два аргумента командной строки и flag_l = true, значит, второй аргумент - ключ "-l"
				   //открываем текущую директорию
		}
		else
		{
			dir = opendir(argv[1]);//flag_l = false,значит, второй аргумент - директория, открываем ее
		}
	}
	break;

	case 3:
	{
		if (optind == 2) // переменная, указывающая на первый аргумент командной строки, не являющийся операцией
		{
			len = strlen(argv[2]);	 //вычисляем длину директории
			strcat(buf_path, argv[2]);// запоминем директорию
			strcat(buf_path, "/");    //для корректного отображения пути до файла
			dir = opendir(argv[2]);
		}
		else if (optind == 1)
		{
			flag_l = true;// так как аргумента 3(значит, ключ точно есть), но мы не дошли до опции
			len = strlen(argv[1]);
			strcat(buf_path, argv[1]);
			strcat(buf_path, "/");
			dir = opendir(argv[1]);
		}
	}
	break;

	default:
		printf("%s\n", "ERROR");
		break;

	}

	if (dir == NULL)
	{
		printf("%s\n", "No such file or directory");
		return -1;
	}

	if (flag_l)
	{
		handler_l(dir, ent, path, buf_path, len);
		free(path);
	}


	else
	{
		while ((ent = readdir(dir)) != NULL)             //пока не закончатся входы в каталог или не будет ошибки
		{
			if (ent->d_name[0] != '.')               //проверка на скрытый файл
				printf("%s\n ", ent->d_name);  //вывод имен файлов
		}
	}
	closedir(dir);                                 //закрываем текущую директорию

	return 0;
}
