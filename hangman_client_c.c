/*****************************************************************************
 * hangman_client_c.c FOR Final Group Project
 * Name: Gavin Worley
 *****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>

#define SEND_BUFFER_SIZE 2048

int client(char *server_ip, char *server_port) {
	// Max guess variable
	int MAX_WRONG_GUESS = 0;
	
	// creation of the socket and an if statement to check for an error during creation
	int client;
	if((client = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		perror("socket");
	}
	// creating structs to be used for connection
	struct sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(atoi(server_port));
	serverAddress.sin_addr.s_addr = inet_addr(server_ip);

	// connecting socket to server using struct and error testing for connection to server
	if(connect(client, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
		perror("connect");
	}
	
	//while loop to keep playing while client inputs 'R'
		//while loop while wrong_guesses is < MAX_WRONG_GUESS
			//if letter has been previously guessed 
			//else letter is in word break back in to while loop
			//else increment wrong_guesses by 1 and break into loop
	
	// closing the client socket
	close(client);

	return 0;
}

int main(int argc, char **argv){
	char *server_port;
	char *server_ip;
	// check to see if server has correct amount of input arguments and error message if not 2
	if (argc != 2) {
		fprintf(stderr, "Usage: ./client-c [server IP] [server port]");
		exit(EXIT_FAILURE);
	}
	// assigning server ip as argument 1 and server port as argument 2
	server_ip = argv[1];
	server_port = argv[2];
	// creating client using server ip and server port
	return client(server_ip, server_port);
}
