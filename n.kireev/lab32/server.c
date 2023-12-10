#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <aio.h>
#include <errno.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/un.h>
#define SERVER "./qwerty123456"

int num = 0;

void handler(int sig, siginfo_t* info, void* ucontext)
{
	struct aiocb** my_aiocb = info->si_value.sival_ptr;
	for (int i = 0; i < num; i++)
	{
		if (aio_return(my_aiocb[i]) != 0)
		{
			int j = 0;
			while (1)
			{
				if (((char*)my_aiocb[i]->aio_buf)[j] == '\0')
				{
					break;
				}
				putchar(toupper(((char*)my_aiocb[i]->aio_buf)[j]));
				j++;
			}
			memset((void*)my_aiocb[i]->aio_buf, 0, sizeof(my_aiocb[i]->aio_buf));
		}
	}
	return;
}

int main()
{
	unlink(SERVER);
	int sock, listener;
	struct sockaddr_un addr;

	struct sigaction sa;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	sa.sa_sigaction = handler;
	sigaction(SIGALRM, &sa, NULL);

	listener = socket(AF_UNIX, SOCK_STREAM, 0);
	if (listener < 0)
	{
		perror("Socket creation error");
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	strcpy(addr.sun_path, SERVER);
	if (bind(listener, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		perror("Binding of listener error");
		exit(2);
	}

	if (listen(listener, 78) < 0)
	{
		perror("Listening error");
		exit(3);
	}

	struct aiocb** all_aiocb = (struct aiocb**)malloc((num + 2) * sizeof(struct aiocb*));

	struct sigevent my_sigev;
	my_sigev.sigev_notify = SIGEV_SIGNAL;
	my_sigev.sigev_signo = SIGALRM;
	my_sigev.sigev_value.sival_ptr = all_aiocb;

	while (1)
	{
		sigaction(SIGALRM, &sa, NULL);
		if (num > 0)
		{
			lio_listio(LIO_NOWAIT, all_aiocb, num, &my_sigev);
		}
		sock = accept(listener, 0, 0);
		if (sock < 0)
		{
			continue;
		}

		all_aiocb = (struct aiocb**)realloc(all_aiocb, (num + 2) * sizeof(struct aiocb*));

		printf("SOCKET: %d\n", sock);
		all_aiocb[num] = (struct aiocb*)calloc(1, sizeof(struct aiocb));
		all_aiocb[num]->aio_buf = (char*)calloc(1024, sizeof(char));
		all_aiocb[num]->aio_fildes = sock;
		all_aiocb[num]->aio_nbytes = 1024;
		all_aiocb[num]->aio_offset = 0;
		all_aiocb[num]->aio_lio_opcode = LIO_READ;

		num = num + 1;
	}
}
