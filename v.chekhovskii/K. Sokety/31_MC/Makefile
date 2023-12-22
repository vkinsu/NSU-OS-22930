all: client server

client: 31_client.c
	gcc -Wall -Wextra -std=c99 -o client 31_client.c

server: 31_server.c
	gcc -Wall -Wextra -std=c99 -o server 31_server.c

clean:
	rm -f client server
