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
#include "comm_utils.h"


#define DEBUG_VERBOSITY 0

struct game_data{
	int phase;

	int alive;
	int max_score;
};

const int PHASE_LOBBY = 1;
const int PHASE_START = 2;
const int PHASE_GAME = 3;
const int PHASE_END = 4;

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

void manage_clients(struct game_data *g_data, struct client_queue *c_queue){
	struct client_queue_node *node = c_queue->first;

	char *param;
	while (node != c_queue->last){
		int message = check_for_message(node->buff, param);
		if (message != NO_MESSAGE){
			int n = clear_message(node->buff);
			node->buff_i -= n;
		}
		if (message == MESSAGE_SCORE){
			int score = atoi(param);
			free(param);
			param = NULL;
			node->g_data.score = score;
			if (score > g_data->max_score){
				g_data->max_score = score;
			}
		}
		if (message == MESSAGE_DEATH){
			node->g_data.is_dead = true;
			g_data->alive --;
		}
		node = node->next;
		if (message != 0)
			printf("%d\n", message);
	}

	if (g_data->phase == PHASE_GAME && g_data->alive <= 0){
		printf("Changing phase to END\n");
		g_data->phase = PHASE_END;
	}

	if (g_data->phase == PHASE_START){
		node = c_queue->first;
		char * message = generate_message(MESSAGE_GAME_START);
		if (message == NULL){ // TODO change this?
			exit(0);
		}
		int mess_len = strlen(message);
		while (node != c_queue->last){
			write(node->socket, message, mess_len);
			node->g_data.is_dead = false;
			node->g_data.score = 0;
			node = node->next;
		}
		g_data->max_score = 0;
		g_data->alive = c_queue->client_queue_size;
		free(message);
		printf("Changing phase to GAME\n");
		g_data->phase = PHASE_GAME;
	}

	if (g_data->phase == PHASE_END){
		node = c_queue->first;
		char * message = generate_message(MESSAGE_GAME_END);
		if (message == NULL){ 
			fprintf(stderr, "Critical error not enough memory");
			//TODO Close connection with clients
			exit(0);
		}
		int mess_len = strlen(message);
		while (node != c_queue->last){
			write(node->socket, message, mess_len);
			node = node->next;
		}
		free(message);
		printf("Changing phase to LOBBY\n");
		g_data->phase = PHASE_LOBBY;
	}
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
			if (buffer[0] == 'c'){
				running = false;
			}
			if (buffer[0] == 's'){
				printf("Changing phase to START");
				g_data.phase = PHASE_START;
			}
		}

		manage_clients(&g_data, c_queue);

		gettimeofday(&current, NULL);

#if DEBUG_VERBOSITY >= 2
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
