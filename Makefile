default: c

all: c

CC=gcc

c: hangman_server_c hangman_client_c

client-c: client-c.c
	$(CC) hangman_client_c.c -o hangman_client_c

server-c: server-c.c
	$(CC) hangman_server_c.c -o hangman_server_c

clean:
	rm -f hangman_server_c hangman_client_c *.o
