#include <string.h>
#include <stdio.h>
#include <termios.h>
#include "protocol.h"
int sockfd;

//cac ham de an mat khau tren terminal

static struct termios old, new;
void initTermios(int echo) {
	tcgetattr(0, &old);                       
	new = old;                                
	new.c_lflag &= ~ICANON;                   
	new.c_lflag &= echo ? ECHO : ~ECHO;       
	tcsetattr(0, TCSANOW, &new);              
}

void resetTermios(void) {
  tcsetattr(0, TCSANOW, &old);
}

char getch_(int echo) {
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

char getch(void) {
    return getch_(0);
}

int get_pass(char pass[]){
	int i=0;

	while(1){
		pass[i]=getch();
		if(pass[i]== '\n') break;
		printf("*");
		i++;
	}
	pass[i]='\0';
	return 1;
}
//---------------------------------------------


int requestSignup(protocol *p){
	char temp[1000];
	p->p_state=CONNECTED;
	strcpy(p->p_message,WANT_TO_SIGNUP);
	toStringData(p,temp);
	send(sockfd,temp,sizeof(temp),0);

	recv(sockfd,temp,sizeof(temp),0);
	toObject(p,temp);
	if (p->p_state==SIGNUP)return 1;
	else return 0;
}
//tao giao dien dang nhap va luu vao giao thuc chuyen di
int signUp(protocol *p)
{
	int i;	//test request signup
	char t;
	char temp[32];//test comfirmation of password
	i=requestSignup(p);
	if (i==0) return -1;
	strcpy(p->p_message,"default");
	do{
		printf("Account: ");
		scanf("%s",p->p_user_info.user_id);while(getchar()!='\n');
		//gets(p->p_user_info.user_id);
		printf("Password: ");
		//scanf("%s",p->p_user_info.password);while(getchar()!='\n');
		get_pass(p->p_user_info.password);
		printf("\nComfirmation:");
		get_pass(temp);
		if(!strcmp(p->p_user_info.password,temp)){
			return 1;
		}
		else {
			printf("%s\n","Signup error!");
			printf("Do you want enter again(y/n)\n");
			t=getchar();
			while(getchar()!='\n');
		}
	}while(t=='y');
	return 0;
}

//reqest sigin
int requestSignin(protocol *p){
	char temp[1000];
	p->p_state = CONNECTED;
	strcpy(p->p_message,WANT_TO_SIGNIN);

	toStringData(p,temp);
	send(sockfd,temp,sizeof(temp),0);

	recv(sockfd,temp,sizeof(temp),0);
	toObject(p,temp);
	if (p->p_state==UNAUTHENTICATE)return 1;
	else return 0;

}
int signIn(protocol *p){
	int i;//bien kiem tra request signin
	i=requestSignin(p);
	if(i==0)return -1;
	strcpy(p->p_message,"default");
	printf("Account: ");
	scanf("%s",p->p_user_info.user_id);while(getchar()!='\n');
	printf("Password: ");
	get_pass(p->p_user_info.password);
	return 1;
}