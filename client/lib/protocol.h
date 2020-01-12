#ifndef PROTOCOL_H
#define PROTOCOL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "user.h"
#include "question.h"
#include "answer.h"
#include "title.h"

enum{
  CONNECTED, SIGNUP, UNAUTHENTICATE, AUTHENTICATE, READY, PLAYING,TIMEOUT
};

#define WANT_TO_SIGNUP  "i want to signup"
#define ALLOW_TO_SIGNUP "you are allowed to signup"
#define SIGNUP_FAIL "fail signup"
#define SIGNUP_SUCCESS "signup success"
#define WANT_TO_SIGNIN  "i want to signin"
#define ALLOW_TO_SIGNIN "you are allowed to signin"
#define WRONG_USER_ID "user id is not exist"
#define WRONG_PASSWORD "password is not correct"
#define CONNECTED_BY_OTHER "you connected in other device"
#define WANT_TO_SIGNOUT "i want to signout"
#define WANT_TO_PLAY "i want to play"
#define ALLOW_PLAY "you are allowed to play"
#define SEND_TITLE "send title"
#define SEND_ANSWER "send answer"
#define TIMEOUT "you answered too late"

typedef struct protocol{
  int p_state;
  user p_user_info;
  title p_title;
  int p_q_num;
  question p_question;
  int score;
  answer p_answer;
  char p_message[256];
}protocol;

void set_signup_state(protocol *);

//////////////////////////////////////////GUI THONG TIN
void toStringData(protocol *p, char* str){
	char buffer[500];
	//state int
	sprintf(buffer, "%d", p->p_state);
	strcpy(str,buffer);
	strcat(str,"|");
	
	//user_info char char
	strcat(str,p->p_user_info.user_id);
	strcat(str,"|");
	strcat(str,p->p_user_info.password);
	strcat(str,"|");

	//title char char int
	strcat(str,p->p_title.title1);
	strcat(str,"|");
	strcat(str,p->p_title.title2);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_title.pick);
	strcat(str,buffer);
	strcat(str,"|");

	//p_q_num int
	sprintf(buffer, "%d", p->p_q_num);
	strcat(str,buffer);
	strcat(str,"|");

	//question char int charx5 int
	strcat(str,p->p_question.title);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_question.question_id);
	strcat(str,buffer);
	strcat(str,"|");
	strcat(str,p->p_question.question);
	strcat(str,"|");
	strcat(str,p->p_question.answer1);
	strcat(str,"|");
	strcat(str,p->p_question.answer2);
	strcat(str,"|");
	strcat(str,p->p_question.answer3);
	strcat(str,"|");
	strcat(str,p->p_question.answer4);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_question.number_answers);
	strcat(str,buffer);
	strcat(str,"|");

	//score int
	sprintf(buffer, "%d", p->score);
	strcat(str,buffer);
	strcat(str,"|");

	//answer intx4
	sprintf(buffer, "%d", p->p_answer.bet1);
	strcat(str,buffer);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_answer.bet2);
	strcat(str,buffer);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_answer.bet3);
	strcat(str,buffer);
	strcat(str,"|");
	sprintf(buffer, "%d", p->p_answer.bet4);
	strcat(str,buffer);
	strcat(str,"|");

	//message char
	strcat(str,p->p_message);
}

///////////////////////////////////////NHAN THONG TIN
void toObject(protocol *p, char* str){
  char * token;
  //state 
  token = strtok(str,"|");
  p->p_state = atoi(token);
  
  //user_info
  token = strtok(NULL,"|");
  strcpy(p->p_user_info.user_id, token);
  token = strtok(NULL,"|");
  strcpy(p->p_user_info.password, token);

  //title
  token = strtok(NULL,"|");
  strcpy(p->p_title.title1, token);
  token = strtok(NULL,"|");
  strcpy(p->p_title.title2, token);
  token = strtok(NULL,"|");
  p->p_title.pick = atoi(token);

  //p_q_num
  token = strtok(NULL,"|");
  p->p_q_num = atoi(token);

  //question
  token = strtok(NULL,"|");
  strcpy(p->p_question.title,token);
  token = strtok(NULL,"|");
  p->p_question.question_id = atoi(token);
  token = strtok(NULL,"|");
  strcpy(p->p_question.question,token);
  token = strtok(NULL,"|");
  strcpy(p->p_question.answer1,token);
  token = strtok(NULL,"|");
  strcpy(p->p_question.answer2,token);
  token = strtok(NULL,"|");
  strcpy(p->p_question.answer3,token);
  token = strtok(NULL,"|");
  strcpy(p->p_question.answer4,token);
  token = strtok(NULL,"|");
  p->p_question.number_answers = atoi(token);

  //score
  token = strtok(NULL,"|");
  p->score = atoi(token);

  //answer
  token = strtok(NULL,"|");
  p->p_answer.bet1 = atoi(token);
  token = strtok(NULL,"|");
  p->p_answer.bet2 = atoi(token);
  token = strtok(NULL,"|");
  p->p_answer.bet3 = atoi(token);
  token = strtok(NULL,"|");
  p->p_answer.bet4 = atoi(token);

  //message
  token = strtok(NULL,"|");
  strcpy(p->p_message,token);
}

void initProtocol(protocol *p){
	strcpy(p->p_user_info.user_id,"default");
	strcpy(p->p_user_info.password,"default");
	strcpy(p->p_title.title1,"default");
	strcpy(p->p_title.title2,"default");
	strcpy(p->p_question.title,"default");
	strcpy(p->p_question.question,"default");
  	strcpy(p->p_question.answer1,"default");
	strcpy(p->p_question.answer2,"default");
  	strcpy(p->p_question.answer3,"default");
  	strcpy(p->p_question.answer4,"default");
	strcpy(p->p_message,"default");
}
#endif