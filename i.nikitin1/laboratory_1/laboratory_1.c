#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ulimit.h>
#include <sys/resource.h>

extern char** environ;

int main(int argc, char*argv[])
{
	// если параметр ожидаемый имеет значение, то ставим перед ним ':'
	char map[] = "ispuU:cC:dvV:";
	long newlimit;
	char buf[256];
	int error = 0;
	int value = 0;
	char ** p;
	struct rlimit rlim;
	if (argc < 2)
	{
		printf("Empty argument!\n");
		return 0;
	}
	// передача коротких параметров (-р) с картой map
	while ((value = getopt(argc, argv, map)) != EOF)
	{
		switch (value)
		{
		case 'i':
		// реальный идентификатор пользователя (или группы) сообщает,
		// кто создал процесс, а эффективный идентификатор пользователя 
		// (или группы) сообщает от чьего лица выполняется процесс, 
		// если эта информация изменяется.
			printf("Реальный ID пользователя: %d\n", getuid());
            printf("Эффективный ID пользователя: %d\n", geteuid());
            printf("Реальный ID группы: %d\n", getgid());
            printf("Эффективный ID группы: %d\n", getegid());
			break;
		
		case 's':
			// присваивает идентификатор группы 
			// процессов pgid тому процессу, который был определен pid
			// int setpgid(pid_t pid, pid_t pgid);
			error = setpgid(0,0);
			error == 0 ? printf("Процесс успешно установлен\n") : printf("Ошибка установки процесса!");
			break;

		case 'p':
			printf("Текущий ID-процесс: %d\n", getpid());
            printf("Родитель ID-процесс: %d\n", getppid());
            printf("Групповой ID-процесс: %d\n", getpgid(0));
			break;

		case 'u':
			//ulimit - ограничения текущего процесса, 
			// например количество одновременно открытых файлов.
			// UL_GETFSIZE - текущее ограничение на размер файла
			printf("ulimit = %ld\n",ulimit(UL_GETFSIZE,0));
			break;

		case 'U':
			printf( "Текущий размер: %ld\n",ulimit(UL_GETFSIZE));
			newlimit = atol(optarg);
			ulimit(UL_SETFSIZE, newlimit);
			printf( "Новый размер: %ld\n",ulimit(UL_GETFSIZE));
			break;

		case 'c':
		    // rlim - структура (буфер) хранения данных - ресурсов процесса
			error = getrlimit(RLIMIT_CORE, &rlim);
			if (error == (-1)) printf("Ошибка!");
			printf("Размер core-файла = %ld\n", rlim.rlim_cur);
			break;

		case 'C':
			error = getrlimit(RLIMIT_CORE, &rlim);
			if (error == (-1)) printf("Ошибка!");
			printf("Текущий размер core-файла = %ld\n", rlim.rlim_cur);
			rlim.rlim_cur = atol(optarg);
			printf("Новый размер core-файла = %ld\n", rlim.rlim_cur);
			break;

		case 'd':
			printf("Current directory: %s\n", getcwd(buf,255));
			break;

		case 'v':
		// вывод переменных сред
            for (p = environ; *p; p++)
            { 
				printf("%s\n", *p); 
			}
            break;

		case 'V':
                putenv(optarg);
                for ( p = environ; *p; p++)
                {
                    printf("%s\n", *p);
                }
                break;


        default:
            printf("Error command\n");
            break;
		}
	}
	
	return 0;
}
