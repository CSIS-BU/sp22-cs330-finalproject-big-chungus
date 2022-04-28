/*****************************************************************************
 * hangman_client_c.c     for Final Project                                                            
 * Name: Gavin Worley
 *****************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
int main(int argc,char *argv[])
{
	int client, num_of_misses, flag;
	char ch,gword[20],mess[100],p;
	struct sockaddr_in seraddr;
	client=socket(AF_INET,SOCK_STREAM,0);
	bzero(&seraddr,sizeof(seraddr));
	seraddr.sin_family=AF_INET;
	seraddr.sin_port=htons(atoi(argv[2]));
	inet_pton(AF_INET,argv[1],&seraddr.sin_addr);
	connect(client,(struct sockaddr*)&seraddr,sizeof(seraddr));
    printf("WELCOME TO HANGMAN GAME\n");
while(1)
{
    
    printf("wait till server give you the word.....\n");
    read(client,gword,20);
    printf("start guessing the word:%s\n",gword);
    
    while(1)
    {
    	printf("put a letter:");
    	scanf("%c",&ch);
    	getchar();
    	write(client,&ch,1);
        read(client,gword,20);
        printf("%s\n",gword);
        read(client,&flag,4);
		
		if(&flag == 0) {
			num_of_misses++;
		}
        
		if(num_of_misses == 6) {
			break;
    
    }
    read(client,mess,100);
    printf("You %s\n",mess);
    read(client,mess,100);
    printf("actual word:%s",mess);
    printf("\nPlay again(y/n):");
    scanf("%c",&p);
    write(client,&p,1);
    if(p!='y')
        break;
}
	close(client);
	return 0;
}
}
