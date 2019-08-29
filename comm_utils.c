#include "comm_utils.h"

#include "stdlib.h"
#include "stdio.h"

#include "string.h"
#include "shared.h"

int find_code(char *buf){
	if (buf[0] == 'a' && buf[1] == 'k') return MESSAGE_ACK;
	if (buf[0] == 'c'){
		if (buf[1] == 's') return MESSAGE_SCORE;
		if (buf[1] == 't') return MESSAGE_GAME_STARTED;
		if (buf[1] == 'd') return MESSAGE_DEATH;
	}
	if (buf[0] == 's'){
		if (buf[1] == 's') return MESSAGE_GAME_START;
		if (buf[1] == 'c') return MESSAGE_MAX_SCORE;
		if (buf[1] == 'e') return MESSAGE_GAME_END;
	}
	return -1;
}

char * get_str(int code){
	switch(code){
		case MESSAGE_ACK: return "ak";
		case MESSAGE_GAME_STARTED: return "ct";
		case MESSAGE_SCORE: return "cs";
		case MESSAGE_DEATH: return "cd";
		case MESSAGE_GAME_START: return "ss";
		case MESSAGE_GAME_END: return "se";
	}
	return NULL;
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

// param: 123
// 0123456789
// ss 123:0
char * generate_message_with_param(int code, char *param){
	int plen = strlen(param);
	char * mess = malloc((5+plen)*sizeof(char));
	if (mess == NULL){
		perror("Could not allocate memory for message");
		return NULL;
	}
	char * bg = get_str(code);
	mess[0] = bg[0];
	mess[1] = bg[1];
	mess[2] = ' ';
	strcpy(mess+3, param);
	mess[3+plen] = DELIM;
	mess[4+plen] = 0;
	return mess;
}

void get_param(char *buf, int i, char **param){
// 01234567
// cc 123:
	int param_len = i-3;
	(*param) = (char*) malloc((param_len+1)*sizeof(char));
	if (param == NULL){
		return;
	}

	for (int j=0 ; j < param_len; j++){
		(*param)[j] = buf[j+3];
	}
	(*param)[param_len] = '\0';
}

int check_for_message(char *buf, char **param){
	for (int i=0; buf[i] != 0; i++){
		if (buf[i] == DELIM){
			int code = find_code(buf);
			if (code == MESSAGE_SCORE ||
					code == MESSAGE_MAX_SCORE){
				get_param(buf, i, param);
			}
			return code;
		}
	}
	return NO_MESSAGE;
}

int clear_message(char *buf){
	//TODO dont use strcpy
	for (int i=0; buf[i] != 0; i++){
		if (buf[i] == DELIM){
			if (buf[i+1] == '\n') i++;
			strcpy(buf, buf+i+1);
			return i+1;
		}
	}
	return 0;
}
