/*****************************************************************************
 * hangman_server_c.c     for Final Project                                                            
 * Name: Zach Thrall
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>
#include <arpa/inet.h>

#define QUEUE_LENGTH 10
#define BUFFER_SIZE 2048

int main(int argc, char **argv) {
	//creating the socket
	int server_socket, client_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	//error testing for socket creation
	if(server_socket == -1){
		perror("socket");
		exit(1);
	}
	
	//don't have a firm grasp on this line but using a line of code like this was suggested
	//on the internet by those who were having similiar "bind:address already in use" errors
	//......seems to have fixed the problem -> imported from project1
	if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0){
		perror("setsockopt(SO_REUSEADDR) failed");
	}	

	//creating and defining structs for bind function
	struct sockaddr_in serverAddr;
	socklen_t sin_size;
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(80);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	//binding server to port using struct
	int bind_status;
	bind_status = bind(server_socket, (struct sockaddr*) &serverAddr, sizeof(serverAddr));
	//error testing for binding to port
	if(bind_status == -1){
		close(server_socket);
		perror("bind");
		exit(1);
	}

	//listening for client connections to server
	int listen_status;
	listen_status = listen(server_socket, QUEUE_LENGTH);
	//error testing for listening for connections
	if(listen_status == -1){
		perror("listen");
		exit(1);
	}
	
	//opening infinite while-loop in order to sequentially accept and read client connections
	while(1){
		//variables
		sin_size = sizeof(struct sockaddr_in);
		char *message;
		
		//creating another socket to accept connections while old socket continues listening
		client_socket = accept(server_socket, (struct sockaddr*) &serverAddr, &sin_size);
		//error testing for accepting connections
		if(client_socket == -1){
			perror("accept");
			exit(1);
		}

		//*****************ADD HANGMAN CODE FROM TEAMMATES*****************
		//this is the initial server blueprint that will be in place until changes are necessary

		//send initial hangman word + structure
		int send_status;
		send_status = recv(client_socket, message, BUFFER_SIZE - 1, 0);
		//error testing for receiving connections
		if (send_status == -1) {
			perror("recv");
			exit(1);
		}

		//closing socket
		close(client_socket);
        }
	
	return 0;
}
