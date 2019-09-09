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

void drop_client(struct client_queue_node *node){
	printf("Dropping client\n");
	close(node->socket);
}

void server_accept(int server_fd, struct client_queue *queue, struct timeval ctime){
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
	queue->last->last_response_sec = ctime.tv_sec;
	queue->last->last_response_usec = ctime.tv_usec;
	printf("Connected new client\n");
	queue->last->connected = true;
	
	queue_push(queue);
}

void server_receive(struct client_queue *queue, struct timeval ctime){
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
		/*if (p_fd[i].revents&POLLERR == POLLERR){
			close(node->socket);
			struct client_queue_node *next_node = node->next;
			queue_drop(queue, node);
			node = next_node;
			printf("Closed connection due to error");
			continue;
		}*/
		if ((p_fd[i].revents&POLLIN) == POLLIN){
			printf("There is pollin\n");
			node->last_response_sec = ctime.tv_sec;
			node->last_response_usec = ctime.tv_usec;
			int n = read(node->socket, node->buff+node->buff_i, BUFF_SIZE - 1 - node->buff_i);
			node->buff_i += n;
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
		if ((p_fd[i].revents&POLLOUT) == POLLOUT){
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

