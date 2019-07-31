#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include "client_queue.h"

void drop_client(struct client_queue_node *node);

void server_accept(int server_fd, struct client_queue *queue);

void server_receive(struct client_queue *queue);

int create_server(int port);

#endif 
