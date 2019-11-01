#ifndef CLIENT_SERVER_COMM_H
#define CLIENT_SERVER_COMM_H

#include "shared.h"
#include "tetris_game.h"
#include "sys/time.h"

struct server_data{
	int sock_fd;
	unsigned int buff_size;
	char buff[BUFF_SIZE];
	char buff_i;
	struct timeval last_message;
	struct tetris_data *t_data;
};

/**
 * Will create server data structure with uninitialized t_data on heap
 * \param sock_fd socket file descriptor for server data
 * \return initialized server data struct
 */
struct server_data create_server_data(int sock_fd);

/**
 * Will try to get a message from the socket
 * \param param pointer to an uninitialized char array that will contain parameter of the message that need to be uninitialized and then freed (look at messages.txt)
 * \return Code for the message that was delivered
 */
int get_message(struct server_data *s_data, char **param);

/**
 * Will connect to the server
 * \param Ip of the server that user wants to connect to
 * \return On success returns socket that will be used to communicate with the server otherwise -1
 */
int connect_to_server(char *ip_addr);

/**
 * Will try to send a message to the server socket
 * \param s_data pointer to the server data
 * \param code message code for the message to be sent
 * \param param from message to be sent or NULL if none (look at messages.txt for messages)
 * \return 1 on success, 0 on failure 
 */
int send_message(struct server_data *s_data, int code, char *param);

/**
 * If last message was some time ago, will send an ack to the server 
 * to not be dropped
 * \param s_data pointer to the server_data struct
 * \return 1 on success 0 on failure
 */
int send_ack(struct server_data *s_data);

#endif
