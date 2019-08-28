#include "comm_utils.h"

#include "stdlib.h"
#include "stdio.h"

#include "string.h"
#include "shared.h"

int find_code(char *buf){
	if (buf[0] == 'a' && buf[1] == 'k') return MESSAGE_ACK;
	if (buf[0] == 'c'){
		if (buf[1] == 's') return MESSAGE_SCORE;
		if (buf[1] == 'd') return MESSAGE_DEATH;
	}
	if (buf[0] == 's'){
		if (buf[1] == 's') return MESSAGE_GAME_START;
		if (buf[1] == 'e') return MESSAGE_GAME_END;
	}
	return -1;
}

char * get_str(int code){
	switch(code){
		case MESSAGE_ACK: return "ak";
		case MESSAGE_SCORE: return "cs";
		case MESSAGE_DEATH: return "cd";
		case MESSAGE_GAME_START: return "ss";
		case MESSAGE_GAME_END: return "se";
	}
	return NULL;
}

char * generate_message_with_param(int code, int param){


}

char * generate_message(int code){
	char * mess = malloc(4*sizeof(char));
	if (mess == NULL){
		perror("Could not allocate memory for message");
		return NULL;
	}
	char * bg = get_str(code);
	mess[0]=bg[0];
	mess[1]=bg[1];
	mess[2]=DELIM;
	mess[3]=0;
	return mess;
}

int check_for_message(char *buf){
	for (int i=0; buf[i] != 0; i++){
		if (buf[i] == DELIM)
			return find_code(buf);
	}
	return NO_MESSAGE;
}

int clear_message(char *buf){
	for (int i=0; buf[i] != 0; i++){
		if (buf[i] == DELIM){
			if (buf[i+1] == '\n') i++;
			strcpy(buf, buf+i+1);
			return i+1;
		}
	}
	return 0;
}
