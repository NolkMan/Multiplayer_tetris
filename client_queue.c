#include "client_queue.h"

#define false 0
#define true 1

#include "stdlib.h"
#include "stdio.h"
#include "string.h"

 struct client_queue * init_client_queue(){
	struct client_queue *queue = malloc(sizeof(struct client_queue));
	struct client_queue_node *node = malloc(sizeof(struct client_queue_node));
	if (queue == NULL || node == NULL){
		perror("Could not create client queue's first node");
		exit(1);
	}
	queue->last = node;
	queue->first = node;

	memset(node->buff,0, sizeof(node->buff));
	node->buff_i = 0;
	node->connected = false;
	node->cli_addr_len = sizeof(node->cli_addr);
	node->next = NULL;
	node->prev = NULL;

	queue->client_queue_size = 0;
	return queue;
}

void queue_push(struct client_queue *queue){
	struct client_queue_node *new_node = malloc(sizeof(struct client_queue_node));
	if (new_node == NULL){
		perror("Could not allocate memory");
		exit(1);
	}
	memset(new_node->buff,0, sizeof(new_node->buff));
	new_node->buff_i = 0;
	new_node->connected = false;
	new_node->cli_addr_len = sizeof(queue->first->cli_addr);

	new_node->prev = queue->last;
	queue->last->next = new_node;
	queue->last = new_node;
	new_node->next = NULL;
	queue->client_queue_size++;
}

void queue_drop(struct client_queue *queue, struct client_queue_node *node){
	if (node->prev == NULL){
		queue->first = node->next;
	}else{
		node->prev->next = node->next;
	}
	node->next->prev = node->prev;
	free(node);
}

void free_client_queue(struct client_queue *queue){
	struct client_queue_node *node = queue->first;
	struct client_queue_node *next_node;
	while (node != NULL){
		next_node = node->next;
		free(node);
		node = next_node;
	}
	free(queue);
}

