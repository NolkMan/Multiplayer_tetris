#ifndef CLIENT_SERVER_COMM_H
#define CLIENT_SERVER_COMM_H

struct server_data{
	int sock_fd;
	unsigned int buff_size;
	char buff[64];
	char buff_i;
};


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



#endif
