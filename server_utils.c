#include "server_utils.h"

#define _XOPEN_SOURCE 700
#define false 0
#define true 1
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "poll.h"
#include "fcntl.h"
#include "errno.h"

#include "unistd.h"
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

void server_accept(int server_fd, struct client_queue *queue){
	queue->last->socket = accept(server_fd, 
			(struct sockaddr *) &(queue->last->cli_addr), 
			&(queue->last->cli_addr_len));
	if (queue->last->socket == -1){
		if (errno == EWOULDBLOCK){
			return;
		}else{
			perror("ERROR on accept");
			return;
		}
	}
	printf("Connected new client\n");
	queue->last->connected = true;
	
	queue_push(queue);
}

void server_receive(struct client_queue *queue){
	struct client_queue_node *node = queue->first;
	struct pollfd *p_fd = malloc(queue->client_queue_size* sizeof(struct pollfd));


	int i = 0;
	while (node->connected == true){
		p_fd[i].fd = node->socket;
		p_fd[i].events = POLLIN | POLLOUT;
		p_fd[i].revents = 0;
		i++;
		node = node->next;
	}

	int ret = poll(p_fd, queue->client_queue_size, 20);

	node = queue->first;
	i = 0;
	while (node->connected == true){
		// TODO make communication
		if (p_fd[i].revents&POLLERR == POLLERR){
			close(node->socket);
			struct client_queue_node *next_node = node->next;
			queue_drop(queue, node);
			node = next_node;
			printf("Closed connection due to error");
			continue;
		}
		if (p_fd[i].revents&POLLIN == POLLIN){
			printf("There is pollin\n");
			int n = read(node->socket, node->buff+node->buff_i, BUFF_SIZE - 1 - node->buff_i);
			if (n == 0){
				close(node->socket);
				struct client_queue_node *next_node = node->next;
				queue_drop(queue, node);
				node = next_node;
				printf("Connection closed\n");
				continue;
			}
			if (n < 0){
				perror("Error reading from a socket");
				// TODO drop socket
			}
			printf("Message: %s", node->buff);
		}
		if (p_fd[i].revents&POLLOUT == POLLOUT){
			printf("There is pollout\n");
		}
		node = node->next;
	}
}

int create_server(int port){
#if DEBUG_VERBOSITY >= 1
	printf("Creating server on port: %d\n", port);
#endif
	int server_fd;
	struct sockaddr_in address;
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Cannot create a server socket"); exit(1);
	}
	int flags = fcntl(server_fd, F_GETFL);	
	if (flags == -1){
		perror("Could not get server_fd flags");
		exit(1);
	}
	if (fcntl(server_fd, F_SETFL, flags | O_NONBLOCK) == -1){
		perror("Could not set accept to non_blocking");
		exit(1);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
		perror("Could not bind to ip"); exit(1);
	}
	if (listen(server_fd, 5) < 0){
		perror("Could not listen");
		exit(1);
	}
	printf("Created server on port: %d\n", port);
	return server_fd;
}

