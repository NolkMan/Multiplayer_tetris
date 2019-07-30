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

struct game_data{
	int phase;

	int max_score;
};

const int PHASE_LOBBY = 1;
const int PHASE_GAME = 2;

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

void manage_clients(){

}

void server_loop(int server_fd, struct client_queue *c_queue){
	char *buffer = NULL;
	size_t buff_size = 0;
	int running = true;
	
	struct timeval last, current;

	struct game_data g_data;
	g_data.phase = PHASE_LOBBY;
	g_data.max_score = 0;

	gettimeofday(&current, NULL);

	while (running){
		last = current;

		server_accept(server_fd, c_queue);
		server_receive(c_queue);

		if (read_input(&buffer, &buff_size)){
			if (buffer[0] = 'c'){
				running = false;
			}
		}

		gettimeofday(&current, NULL);



#if DEBUG_VERBOSITY >= 1
		printf("Loop took %lu miliseconds\n", current.tv_usec - last.tv_usec);
#endif
	}
}

int main(){
	struct client_queue * c_queue = init_client_queue();
	int listen_fd = create_server(PORT);
	server_loop(listen_fd, c_queue);

	free_client_queue(c_queue);
}
