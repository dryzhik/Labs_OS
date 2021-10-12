#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void exit_msg()
{
	printf("Atexit handler has worked from process %d\n", getpid());
}

int main(int argc, char** argv)
{
	atexit(exit_msg); //регистрирует функцию exit_msg() при нормальном завершении процесса(с помощью системного вызова exit() или возврата результатов функции main())

	int status = 0;

	pid_t pid = fork(); //системный вызов для создания нового процесса(ветвления)
	switch (pid)
	{
	case -1:// в вслучае ошибки fork() возвращает -1, дочерний процесс не создается

		perror("fork");
		exit(1);

	case 0: //успешный запуск  fork() возвращает 0, запускается новый(дочерний) процесс

		printf("CHILD PROCESS: Hi! I'm child.\n");
		printf("CHILD PROCESS: My pid is: %d\n", getpid());          //getpid() возвращает идентификатор вызывающего процесса (не может возвращать ошибку)
		printf("CHILD PROCESS: My parent's pid is: %d\n", getppid());//getppid() возвращает идентификатор родителя вызывающего процесса(не может возвращать ошибку)
		exit(1);// завершение процесса, перед завершением обращается к  функции atexit()

	default:

		printf("PARENT PROCESS: Hi! I'm parent.\n");
		printf("PARENT PROCESS: My  pid is: %d\n", getpid());
		printf("PARENT PROCESS: My parent's pid is: %d\n", getppid());
		printf("PARENT'S CHILD PROCESS: My child's pid is: %d\n", pid);
		wait(&status);//возвращает pid завершенного дочернего процесса или –1 в случае ошибки
			  //если никакого дочернего процесса не было прервано, то вызов блокируется, пока потомок не завершится
			  //если дочерний процесс уже был завершен, то результат возвращается немедленно
		printf("CHILD PROCESS return status: %d\n", WEXITSTATUS(status));
	}

	return 0;
}
