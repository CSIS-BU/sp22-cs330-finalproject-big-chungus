/*****************************************************************************
 * hangman_server_c.c     for Final Project                                                            
 * Name: Zach Thrall, Ian Connors, Gavin Worley
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
#include <stdbool.h>
#include <time.h>

#define QUEUE_LENGTH 10
#define BUFFER_SIZE 2048
#define MAX_TRIES 5

int main(int argc, char **argv) {

	//test for arguments
	char *server_port;

	if (argc != 2) {
    fprintf(stderr, "Usage: ./server-c [server port]\n");
    exit(EXIT_FAILURE);
  	}
  	server_port = argv[1];

	//creating the socket
	int server_socket, client_socket, number_of_blanks, blanks_filled_in, hold = 0, winner = 1;
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
	serverAddr.sin_port = htons(atoi(server_port));
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
	//variables to load word file
	FILE *fp;
	int numOfWords = 213;
	int maxWordLength = 30;
	char words[numOfWords][maxWordLength];
	char *fileName = "words.txt";
	int i = 0;

	//open word file and save to an array
	fp = fopen(fileName, "r");
	if (fp == NULL)
	{
		perror ("Error loading file");
		exit (EXIT_FAILURE);
	}
	while(fgets(words[i++], maxWordLength, fp));
	fclose(fp);
	
	//variables
	sin_size = sizeof(struct sockaddr_in);
	
	//creating another socket to accept connections while old socket continues listening
	client_socket = accept(server_socket, (struct sockaddr*) &serverAddr, &sin_size);
	//error testing for accepting connections
	if(client_socket == -1){
		perror("accept");
		exit(1);
	}
	//opening infinite while-loop in order to sequentially accept and read client connections
	while(1){
		//variables for game
		char *word, *message;
		char guess, p;
		size_t wordLength;
		bool correct, win;
		int flag, wrongGuesses;
		wrongGuesses = 0;

		//pick random word from words array
		srand(time(NULL));
		int index = rand() % numOfWords;
		word = words[index];
		wordLength = strlen(word) - 2; //strlen was reading the length to be 2 more than the actual length of the word
			//possibly due to formatting in the text document

		//display word in server
		printf("Word length: %ld\n", wordLength);
		printf("Word: %s\n", word);
		//prepare initial message
		char underscores[wordLength];
		for (i = 0; i < wordLength; i++)
		{
			underscores[i] = '_'; //every even character of the message is an underscore
			//underscores[2 * i + 1] = ' '; //every odd character of the message is a space
		}

		//send initial hangman word + structure
		int msg_status;
		msg_status = send(client_socket, underscores, sizeof(underscores), 0);
		//error testing for sending data
		if (msg_status == -1) {
			perror("send");
			exit(1);
		}
		//infinite receive guess and send feedback
		while (1)
		{
			//receive guess
			read(client_socket, &guess, sizeof(char));
			//find out if guess is correct
			correct = false;
			win = false;
			flag = 0;
			//compare guess to every item of array
			for (i = 0; i < wordLength; i++){
				number_of_blanks = wordLength;
				if (word[i] == guess){
					correct = true;
					underscores[i] = guess;
					blanks_filled_in++;
					flag = 1;
				}
				
			}
			printf("number_of_blanks: %d\n", number_of_blanks);
			printf("blanks_filled_in: %d\n", blanks_filled_in);
			if(number_of_blanks <= blanks_filled_in){
				write(client_socket, &winner, sizeof(int));
				win = true;
				break;
			}
			
			write(client_socket, &hold, sizeof(int));
			
			if (flag == 0){
				wrongGuesses++;
			}
			
			//send word again
			write(client_socket, underscores, sizeof(underscores));
			//send number of wrong guesses
			write(client_socket, &wrongGuesses, sizeof(int));
			
			if(wrongGuesses >= MAX_TRIES){
				message = "lost the game :(";
				write(client_socket, message, strlen(message));
                break;
            }
		}
		if (win){
			message = "WON the game";
			write(client_socket, message, strlen(message));
		}
		//send what the word was
		
		printf("player %s\n",message);
		//wait for server to send something so message and word don't get combined
		read(client_socket, message, sizeof(char));
		write(client_socket, word, strlen(word));
		break;
    	//read(client_socket,&p,sizeof(char));
		// printf("player is replaying?: %c\n",p);
    	// if(p!='y'){
		// 	break;
		// }
    }
	
	//closing socket
	close(client_socket);
	return 0;
}