#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "string.h"

#include "shared.h"

/**
 * Will connect to the server
 * \param Ip of the server that user wants to connect to
 * \return On success returns socket that will be used to communicate with the server otherwise -1
 */
int tetris_connect(char *ip_addr){
	int sock;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Could not create socket");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) < 0){
		printf("Please provide a valid server address :)\n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("You've got a problem and connection failed\n");
		return -1;
	}
	return sock;
}

void game_loop(){
}

void lobby_loop(){
}

void menu_loop(){
}

int main(int argc, char **argv){
	if (argc < 2){
		printf("Please provide an ip address of a server\n");
		exit(0);
	}
}
