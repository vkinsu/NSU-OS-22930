all: client server

client: 30_client.c
	gcc -Wall -Wextra -std=c99 -o client 30_client.c

server: 30_server.c
	gcc -Wall -Wextra -std=c99 -o server 30_server.c

clean:
	rm -f client server
