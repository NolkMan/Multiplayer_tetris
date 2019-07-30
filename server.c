#define false 0
#define true 1
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include "stdlib.h"
#include "sys/time.h"

#include "unistd.h"
#include "string.h"
#include "poll.h"

#include "shared.h"
#include "server_utils.h"


#define DEBUG_VERBOSITY 0

int read_input(char **buffer, size_t *buff_size){
	struct pollfd p_fd[1];
	p_fd[0].fd = 0;
	p_fd[0].events = POLLIN;

	int ret = poll(p_fd, 1, 20);

	if (ret == 1){
		int n = getline(buffer, buff_size, stdin);
		return true;
	}
	return false;
}



int main(){
	struct client_queue * c_queue = init_client_queue();
	int listen_fd = create_server(PORT);
	//server_loop(listen_fd);

	free_client_queue(c_queue);
}
