#include "comm_utils.h"

#include "string.h"
#include "shared.h"


int check_for_message(char *buf){
	for (int i=0; buf[i] != 0; i++){
		if (buf[i] == DELIM)
			return 1;
	}
}

void clear_message(char *buf){

}
