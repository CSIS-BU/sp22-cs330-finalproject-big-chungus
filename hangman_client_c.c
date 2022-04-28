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

	char ch,guessWord[100],message[100],check, p;
	int client, num_wrong_guesses, flag;

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

	num_wrong_guesses = 0;
    printf("\nWELCOME TO HANGMAN GAME\n");
	while(1){
		printf("wait until server gives you the word.....\n");
		read(client, guessWord, 60);
		printf("start guessing the word:%s\n",guessWord);

		//infinite guess letter and receive feedback
		while(1){

			printf("put a letter:");
            scanf("%c",&ch);
    	    getchar();
			//send guess to server
			write(client,&ch,1);
			//get word from server
			read(client,guessWord,60);
			printf("%s\n",guessWord);
			//get number of wrong guesses from server
            read(client, &flag, sizeof(int));

			if (&flag == 0){
				num_wrong_guesses++;
			}
			if (num_wrong_guesses >= MAX_TRIES){
				break;
			}
			/* Tell user how many guesses has left. */
            printf("You have %d", MAX_TRIES - num_wrong_guesses);
            printf(" wrong guesses left.\n");
		}
		read(client,message,100);
		printf("You %s\n",message);
		read(client,message,100);
		printf("actual word:%s",message);
		printf("\nPlay again(y/n):");
		scanf("%c",&p);
    	write(client,&p,1);
    	if(p!='y')
        	break;
	}
	close(client);
	return 0;
}