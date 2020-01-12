#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "login_helper.h"

void do_signup(protocol *p){
  user u = p->p_user_info;
  char f_name[256] = "users/";
  strcat(f_name,u.user_id);
  if(access(f_name, F_OK) != -1){ // User is exist
    p->p_state = CONNECTED;
    set_empty_user(&(p->p_user_info));
    memset(p->p_message,'\0',sizeof(p->p_message));
    strcat(p->p_message,SIGNUP_FAIL);
  } else{
    FILE *f;
    f = fopen(f_name,"w");
    if(f){
      fprintf(f, "%s", u.password);
    } else{
      printf("Opening file error \n");
      //TODO sign up fail
    }
    fclose(f);
    p->p_state = CONNECTED;
    memset(p->p_message,'\0',sizeof(p->p_message));
    strcat(p->p_message,SIGNUP_SUCCESS);
    write_user_state(u,p->p_state);   //write CONNECTED state in userstate/u_id
  }
}

void do_signin(protocol *p){
  user u = p->p_user_info;
  char f_name[256] = "users/";
  strcat(f_name,u.user_id);
  FILE *f = fopen(f_name,"r");
  if(f != NULL){
    char pass[32];
    fscanf(f,"%s",pass);
    if(!strcmp(pass,u.password)){
      if(!match_connected_state(u)){
        p->p_state = CONNECTED;
        strcpy(p->p_message,CONNECTED_BY_OTHER);
      } else{
        p->p_state = AUTHENTICATE;  /*sign in success*/
        write_user_state(u,AUTHENTICATE);
      }
    } else{
      p->p_state = CONNECTED;
      strcpy(p->p_message,WRONG_PASSWORD);
    }
  } else{
      p->p_state = CONNECTED;
      strcpy(p->p_message,WRONG_USER_ID);
  }
}

