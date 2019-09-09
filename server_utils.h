#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

#include "client_queue.h"
#include "sys/time.h"

void drop_client(struct client_queue_node *node);

void server_accept(int server_fd, struct client_queue *queue, struct timeval ctime);

void server_receive(struct client_queue *queue, struct timeval ctime);

int create_server(int port);

#endif 
