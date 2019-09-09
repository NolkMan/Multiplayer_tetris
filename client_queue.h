#ifndef CLIENT_QUEUE_H
#define CLIENT_QUEUE_H

#include "shared.h"

#include "netinet/in.h"

struct client_game_data{
	int is_dead;
	int started;
	int score;
};

struct client_queue_node{
	int buff_i;
	char buff[BUFF_SIZE];
	int connected;
	int socket;
	unsigned long long last_response_sec;
	unsigned int last_response_usec;
	struct client_queue_node *next;
	struct client_queue_node *prev;
	struct sockaddr_in cli_addr;
	int cli_addr_len;

	struct client_game_data g_data;
};

struct client_queue{
	struct client_queue_node *first;
	struct client_queue_node *last;
	int client_queue_size;
};

struct client_queue * init_client_queue();

void queue_push(struct client_queue *queue);

void queue_drop(struct client_queue *queue, struct client_queue_node *node);

void free_client_queue(struct client_queue *queue);

#endif
