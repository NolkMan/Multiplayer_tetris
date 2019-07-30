#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H
#include "netinet/in.h"

#define BUFF_SIZE 128

struct client_queue_node{
	int buff_i;
	char buff[BUFF_SIZE];
	int connected;
	int socket;
	struct client_queue_node *next;
	struct client_queue_node *prev;
	struct sockaddr_in cli_addr;
	int cli_addr_len;
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

void server_accept(int server_fd, struct client_queue *queue);

void server_receive(struct client_queue *queue);

int create_server(int port);

#endif 
