#ifndef CLIENT_SERVER_COMM_H
#define CLIENT_SERVER_COMM_H

/**
 * Will connect to the server
 * \param Ip of the server that user wants to connect to
 * \return On success returns socket that will be used to communicate with the server otherwise -1
 */
int connect_to_server(char *ip_addr);



#endif
