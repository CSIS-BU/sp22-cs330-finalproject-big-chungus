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
	if (argc != 2) {
		fprintf(stderr, "Usage: ./client-c [server IP] [server port]");
		exit(EXIT_FAILURE);
	}
// assigning server ip as argument 1 and server port as argument 2
		server_ip = argv[1];
		server_port = argv[2];

	char ch,guessWord[20],message[100],check;
	int client, num_wrong_guesses;

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

    printf("\n\nWelcome to hangman.");
    printf("\n\nEach letter is represented by an underscore.");
    printf("\n\nYou have to type only one letter in one try");
    printf("\n\nYou have %d tries to try and guess the word or else :( !", MAX_TRIES);
    printf("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
	while(1){
		printf("wait until server give you the word.\n");
		read(client, guessWord, 20);
		printf("start guessing the word:%s\n",guessWord);

		while(1){

			printf("guess a letter:");
            scanf("%c",&ch);
    	    getchar();
			write(client,&ch,1);
			read(client,guessWord,20);
			printf("%s\n",guessWord);

            read(client, &num_wrong_guesses, sizeof(num_wrong_guesses));
			/* Tell user how many guesses has left. */
            printf("You have %d", MAX_TRIES - num_wrong_guesses);
            printf(" guesses left.\n");
            if(num_wrong_guesses == 5)
            {
                break;
            }



	read(client,message,100);
	printf("You %s\n",message);
	read(client,message,100);
	printf("Actual word:%s",message);
	printf("\nPlay again(r):");
	scanf("%c",&check);
	write(client,&check,1);
	if(check!='r'){
		break;
	}
	}
	close(client);
	return 0;
}
}