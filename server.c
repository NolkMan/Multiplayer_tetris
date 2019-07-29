#define false 0
#define true 1
#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "poll.h"

#include "unistd.h"
#include "string.h"

#include "shared.h"

#define BUFF_SIZE 128

struct client_queue_node{
	int buff_i;
	char buff[BUFF_SIZE];
	int connected;
	int socket;
	struct client_queue_node *next;
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

	bzero(client_queue->buff, BUFF_SIZE);
	client_queue->buff_i = 0;
	client_queue->connected = false;
	client_queue->cli_addr_len = sizeof(client_queue->cli_addr);
	client_queue->next = NULL;

	client_queue_size = 0;
}

void queue_push(){
	struct client_queue_node *new_node = malloc(sizeof(struct client_queue_node));
	bzero(new_node->buff, BUFF_SIZE);
	new_node->buff_i = 0;
	new_node->connected = false;
	new_node->cli_addr_len = sizeof(client_queue->cli_addr);

	last->next = new_node;
	last = new_node;
	new_node->next = NULL;
	client_queue_size++;
}

void server_accept(int server_fd){
	last->socket = accept(server_fd, (struct sockaddr *) &(last->cli_addr), &(last->cli_addr_len));
	if (last->socket < 0){
		perror("ERROR on accept");
		return;
	}
	printf("Connected new client");
	last->connected = true;
	
	queue_push();
}

void server_receive(int server_fd){
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

	int ret = poll(p_fd, client_queue_size, 0);

	node = client_queue;
	int i=0;
	while (node->connected == true){
		// TODO implement more events from revent
		if (p_fd[i].revent&POLLIN == POLLIN){

		}
		
		int n = read(node->socket, node->buff+node->buff_i, BUFF_SIZE - 1 - node->buff_i);
		if (n < 0){
			perror("Error reading from a socket");
			// TODO drop socket
		}
		printf("Message: %s", node->buff);
		node = node->next;
	}
}


int main(){
	init_client_queue();
	int server_fd;
	struct sockaddr_in address;
	
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Cannot create a server socket"); exit(1);
	}
	bzero((char *) &address, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);
	if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0){
		perror("Could not bind to ip"); exit(1);
	}
	listen(server_fd, 5);

	server_accept(server_fd);
	server_receive(server_fd);

}
