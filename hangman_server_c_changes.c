/*****************************************************************************
 * hangman_server_c.c     for Final Project                                                            
 * Name: Gavin Worley
 *****************************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main(int argc,char *argv[])
{
	char ch,word[20],unknown[20],mess[100],p;
	int i,flag=0,,client_socket,clilen,g,h;
	struct sockaddr_in seraddr,cliaddr;
	client_socket=socket(AF_INET,SOCK_STREAM,0);

	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;

	seraddr.sin_port=htons(atoi(argv[1]));
	bind(client_socket,(struct sockaddr*)&seraddr,sizeof(seraddr));

	listen(client_socket,5);
	clilen=sizeof(cliaddr);
	client_socket=accept(client_socket,(struct sockaddr*)&cliaddr,&clilen);

	printf("client connected\n");
while(1)
{
    flag=0;
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


	//variables for game
	char * word;
	int wordLength;
	bool lettersGuessed[26];
	int wrongGuesses = 0;

	//pick random word
	srand(time(NULL));
	int index = rand() % numOfWords;
	word = words[index];
	wordLength = strlen(word);
	printf("Word: %s\n", word);
	
	int length = strlen(word);
	for (i = 0; i < length; i++)
	{
		unknown[i] = '_';
		unknown[i] = '\0';
	}
  
    write(client_socket,unknown,sizeof(unknown));
    while(1)
    {

    	read(client_socket,&ch,sizeof(ch));
    	
        for(i=0;i < length;i++)
        {
        	flag=0;
        	if(unknown[i]=='_'&&ch==word[i])
        	{
        		unknown[i]=ch;
        		flag=1;
        		break;
        	}

        }
        if(flag==0)
        {
            
			wrongGuesses++;
        }
		//hardcoded for now
		if(wrongGuesses == 6){
			
			break;
		}
        write(client_socket,unknown,20);    
        if(strcmp(unknown,word)== 0)
        	{
        		break;
        	}
		

    }
    if(strcmp(unknown,word)==0)
    {
        strcpy(mess,"won the game");   
    }
    else
    {
    	strcpy(mess,"lose the game");
    }
    write(client_socket,mess,sizeof(mess));
    write(client_socket,word,sizeof(word));
    printf("player %s",mess);
    printf("\n");
    read(client_socket,&p,1);
    if(p!='y')
        break;
}
    close(client_socket);
    return 0;
}
