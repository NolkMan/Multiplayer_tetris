#include "stdio.h"
#include "stdlib.h"
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "string.h"

#include "shared.h"

int main(int argc, char **argv){
	if (argc < 2){
		printf("Please provide an ip address of a server\n");
		exit(0);
	}
	int sock;
	struct sockaddr_in serv_addr;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		perror("Could not create socket");
		exit(1);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) < 0){
		printf("Please provide a valid server address :)\n");
		exit(0);
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("You've got a problem and connection failed\n");
		exit(0);
	}

	write(sock, "Twoja stara\n", 12);
	
}
