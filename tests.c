#include "stdio.h"
#include "stdlib.h"

#include "comm_utils.h"
#include "utils.h"

void comm_utils_tests(){
	char param[] = "1";
	char * message = generate_message_with_param(MESSAGE_MAX_SCORE, param);
	printf("Message: \"%s\"\n", message);
	char *par = NULL;
	int code = check_for_message(message, &par);
	printf("Code: %d\nParam: \"%s\"\n", code, par);

	free(par);
	free(message);
}

void utils_tests(){
	struct timeval time;
	time.tv_sec = 1234;
	time.tv_usec = 567890;
	if (mstime(time) == 1234567){
		printf("utils passed\n");
	}else{
		printf("utils failed\n");
	}
}


int main(){
	comm_utils_tests();
	utils_tests();
	return 0;
}
