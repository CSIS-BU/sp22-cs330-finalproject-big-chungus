/*****************************************************************************
 * hangman_client_c.c FOR Final Group Project
 * Name: Gavin Worley, Ian Connors
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

#define MAX_TRIES 5

int main(int argc, char **argv) {
    char *server_port;
	char *server_ip;
	// check to see if server has correct amount of input arguments and error message if not 2
	if (argc != 3) {
		fprintf(stderr, "Usage: ./client-c [server IP] [server port]");
		exit(EXIT_FAILURE);
	}
	// assigning server ip as argument 1 and server port as argument 2
		server_ip = argv[1];
		server_port = argv[2];

	char ch,guessWord[100],message[100],check, word[100];
	int client,winner_check = 0;
	int num_wrong_guesses = 0;

    //creating the socket
    client = socket(AF_INET, SOCK_STREAM, 0);
    //error testing for socket creation
    if(client == -1){
        perror("socket");
        exit(1);
    }

    // creating structs to be used for connection
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(atoi(server_port));
    inet_pton(AF_INET,server_ip,&serverAddress.sin_addr);

    // connecting socket to server using struct and error testing for connection to server
	if(connect(client, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
		perror("connect");
	}

    
	while(1){
		printf("\n\nWelcome to hangman.");
		printf("\n\nEach letter is represented by an underscore.");
		printf("\n\nYou have to type only one letter in one try");
		printf("\n\nYou have %d tries to try and guess the word or else :( !", MAX_TRIES);
		printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
		printf("wait until server gives you the word.\n");
		read(client, guessWord, 60);
		printf("start guessing the word:%s\n",guessWord);

		//infinite guess letter and receive feedback
		while(1){

			printf("guess a letter:");
            scanf("%c",&ch);
    	    getchar();
			//send guess to server
			write(client,&ch,sizeof(char));

			read(client,&winner_check,sizeof(int));
			if(winner_check != 0){
				break;
			}

			//get word from server
			read(client, guessWord, strlen(guessWord));
			printf("%s\n",guessWord);
			
			//get number of wrong guesses from server
			read(client,&num_wrong_guesses,sizeof(int));

			/* Tell user how many guesses has left. */
            printf("You have %d", MAX_TRIES - num_wrong_guesses);
            printf(" wrong guesses left.\n");
            if(num_wrong_guesses >= MAX_TRIES)
            {
                break;
            }
			
		}
		read(client,message,sizeof(message));
		printf("You have %s\n",message);
		//send random message just so server doesn't combine two sends
		write(client,"w",sizeof(char));
		read(client,word,sizeof(word));
		printf("The word was %s\n", word);
		break;
		// printf("\nPlay again(y/n):");
		// scanf("%c",&check);
		// if (check != 'y'){
		// 	break;
		// }
		// write(client,&check,sizeof(char));
	}
	close(client);
	return 0;
}