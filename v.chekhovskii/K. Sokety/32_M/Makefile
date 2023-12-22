all: client server

client: 31_client_async.c
	gcc -Wall -Wextra -std=c99 -o client 31_client_async.c

server: 31_server_async.c
	gcc -Wall -Wextra -std=c99 -o server 31_server_async.c

clean:
	rm -f client server
