#define false 0
#define true 1
#define _XOPEN_SOURCE 700
#include <stdio.h>
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "poll.h"
#include "fcntl.h"
#include "sys/time.h"
#include "errno.h"

#include "unistd.h"
#include "string.h"

#include "shared.h"

#define BUFF_SIZE 128

#define DEBUG_VERBOSITY 0

struct client_queue_node{
	int buff_i;
	char buff[BUFF_SIZE];
	int connected;
	int socket;
	struct client_queue_node *next;
	struct client_queue_node *prev;
	struct sockaddr_in cli_addr;
	int cli_addr_len;
}*client_queue, *last;
int client_queue_size;

void init_client_queue(){
	client_queue = malloc(sizeof(struct client_queue_node));
	if (client_queue == NULL){
		perror("Could not create client queue's first node");
		exit(1);
	}
	last = client_queue;

	memset(client_queue->buff,0, sizeof(client_queue->buff));
	client_queue->buff_i = 0;
	client_queue->connected = false;
	client_queue->cli_addr_len = sizeof(client_queue->cli_addr);
	client_queue->next = NULL;
	client_queue->prev = NULL;

	client_queue_size = 0;
}

void queue_push(){
	struct client_queue_node *new_node = malloc(sizeof(struct client_queue_node));
	memset(new_node->buff,0, sizeof(new_node->buff));
	new_node->buff_i = 0;
	new_node->connected = false;
	new_node->cli_addr_len = sizeof(client_queue->cli_addr);

	new_node->prev = last;
	last->next = new_node;
	last = new_node;
	new_node->next = NULL;
	client_queue_size++;
}

void queue_drop(struct client_queue_node *node){
	if (node->prev == NULL){
		client_queue = node->next;
	}else{
		node->prev->next = node->next;
	}
	node->next->prev = node->prev;
	free(node);
}

void free_client_queue(){
	struct client_queue_node *node = client_queue;
	struct client_queue_node *next_node;
	while (node != NULL){
		next_node = node->next;
		free(node);
		node = next_node;
	}
}

void server_accept(int server_fd){
	last->socket = accept(server_fd, (struct sockaddr *) &(last->cli_addr), &(last->cli_addr_len));
	if (last->socket == -1){
		if (errno == EWOULDBLOCK){
			return;
		}else{
			perror("ERROR on accept");
			return;
		}
	}
	printf("Connected new client\n");
	last->connected = true;
	
	queue_push();
}

void server_receive(){
	struct client_queue_node *node = client_queue;
	struct pollfd *p_fd = malloc(client_queue_size* sizeof(struct pollfd));


	int i = 0;
	while (node->connected == true){
		p_fd[i].fd = node->socket;
		p_fd[i].events = POLLIN | POLLOUT;
		p_fd[i].revents = 0;
		i++;
		node = node->next;
	}

	int ret = poll(p_fd, client_queue_size, 20);

	node = client_queue;
	i = 0;
	while (node->connected == true){
		// TODO make communication
		if (p_fd[i].revents&POLLERR == POLLERR){
			close(node->socket);
			struct client_queue_node *next_node = node->next;
			queue_drop(node);
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
				queue_drop(node);
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


void server_loop(int server_fd){
	char *buffer = NULL;
	size_t buff_size = 0;
	int running = true;
	
	struct timeval last, current;

	gettimeofday(&current, NULL);

	while (running){
		last = current;
		server_accept(server_fd);
		server_receive();

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
	printf("%d\n", PORT);
	init_client_queue();
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
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
		perror("Could not bind to ip"); exit(1);
	}
	if (listen(server_fd, 5) < 0){
		perror("Could not listen");
		exit(1);
	}
	server_loop(server_fd);

	free_client_queue();
}
