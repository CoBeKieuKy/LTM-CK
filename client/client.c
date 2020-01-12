#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>
#include <errno.h>
#include <arpa/inet.h>
#include <termios.h>
#include <time.h>
#include <signal.h>
#include "lib/protocol.h"
#include "lib/login_signup.h"
#include "lib/play.h"
//-----------------------------
#define FAIL -1

//-----------------------------
protocol p;
//BIEN STRING DATA
char str[1000];

/////////////////////////////////////////////////////////////////////////////////////////////
void  INThandler(int sig)
{
     char  c;
     signal(sig, SIG_IGN);              /* disable Ctrl-C           */
     printf("Do you really want to quit? [y/n] ");
     c = getchar();                     /* read an input character  */
     if (c == 'y' || c == 'Y'){
        p.p_state = AUTHENTICATE;
        strcpy(p.p_message,WANT_TO_SIGNOUT);
		toStringData(&p,str);
		send(sockfd,str,sizeof(str),0);
        exit(0);                      /* exit.  Otherwise,        */
     }
     else
          signal(SIGINT, INThandler);   /* reinstall the handler    */
}
//-----------------------------
int createConnection(int PORT,char IP[10])
{
	struct sockaddr_in serverAddr;
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	if(sockfd<0){
		printf("Can not creat socket!\n");
		return FAIL;
	}
	memset(&serverAddr,0,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(IP);
	if(connect(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr))<0){
	printf("Connect to server fail!\n");
	return FAIL;
	}
	printf("Connected to server!\n");
	return 1;
}
void printMenu()
{
	printf("*************----------------**************\n");
	printf("*****           1.LOGIN               *****\n");
	printf("*****           2.SIGNUP              *****\n");
	printf("*****           3.QUIT                *****\n");
	printf("*************----------------**************\n");
	printf("	Please enter your choice : \n");
	printf("      **: ");
}

int main(int argc, char* argv[])
{	
	initProtocol(&p);
	signal(SIGINT,INThandler);
	int n,t,i;
	char c;
	i=createConnection(5550,argv[1]);

	if (i==FAIL)return 1;
	while(1){
		t=-1; // bien t check trang thai cua signup
		printMenu();
		scanf("%d",&n);
		while(getchar()!='\n');
		if(n==3) return 0;
		switch(n){
			case 1: 
				loop:
				t=signIn(&p);
				if (t==-1) continue;
				toStringData(&p,str);
				send(sockfd,str,sizeof(str),0);

				recv(sockfd,str,sizeof(str),0);
				toObject(&p,str);
				switch(p.p_state){
					case CONNECTED: 
						printf("\n%s\n",p.p_message);
						do{
							printf("Are you want to re-login?(y/n)");
							scanf("%c",&c);while(getchar()!='\n');
						}while(c!='y'&&c!='Y'&&c!='n'&&c!='N');
						if (c=='y'||c=='Y') goto loop;
						continue;
						break;
					case AUTHENTICATE:
						printf("\nLogin success!\n");
						i=play(&p,sockfd);
						if (i==0) continue;
						break;
				}
				break;
			case 2: 
				t=signUp(&p);
				if(t==0||t==-1) continue;
				toStringData(&p,str);
				send(sockfd,str,sizeof(str),0);


				recv(sockfd,&p,sizeof(protocol),0);
				toObject(&p,str);
				if (strcmp(p.p_message,SIGNUP_FAIL)==0){
					printf("\n%s\n","This user_id is not valid or is exist" );			
				}else printf("\n%s\n","Sign up success !!" );
				break;
		}
		
	}
}
