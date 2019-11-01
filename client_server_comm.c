#include "client_server_comm.h"

#define _XOPEN_SOURCE 700
#include "sys/socket.h"
#include "netinet/in.h"
#include "arpa/inet.h"
#include "unistd.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "sys/time.h"

#include "shared.h"
#include "comm_utils.h"

#include "poll.h"
#include "utils.h"


int get_message(struct server_data *s_data, char **param){
	struct pollfd pfd[1];
	pfd[0].fd = s_data->sock_fd;
	pfd[0].events = POLLIN;

	poll(pfd,1,10);

	if ((pfd[0].revents&POLLIN) == POLLIN){
		int n = read(s_data->sock_fd, 
				s_data->buff + s_data->buff_i, 
				s_data->buff_size);
		s_data->buff_i += n;
	}

	int code = check_for_message(s_data->buff, param);
	if (code != NO_MESSAGE){
		int n = clear_message(s_data->buff);
		s_data->buff_i -= n;
	}
	return code;
}

struct server_data create_server_data(int sock_fd){
	struct timeval curr;
	gettimeofday(&curr, NULL);
	struct server_data s_data;
	s_data.sock_fd = sock_fd;
	s_data.buff_size=BUFF_SIZE;
	s_data.buff_i = 0;
	s_data.last_message = curr;
	memset(s_data.buff, '\0', sizeof(s_data.buff));
	s_data.t_data = (struct tetris_data*) malloc(sizeof(struct tetris_data));
	if (s_data.t_data == NULL){
		exit(0);
	}
	return s_data;
}

int connect_to_server(char *ip_addr){
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

void update_time(struct server_data *s_data){
	struct timeval current;
	gettimeofday(&current, NULL);

	s_data->last_message = current;
}

int send_message(struct server_data *s_data, int code, char *param){
	if (param){
		char * message = generate_message_with_param(code, param);
		if (message == NULL){
			return 0;
		}
		update_time(s_data);
		write(s_data->sock_fd, message, strlen(message));
		free(message);
	}else{
		char * message = generate_message(code);
		if (message == NULL){ return 0; }
		update_time(s_data);
		write(s_data->sock_fd, message, strlen(message));
		free(message);
	}
	return 1;
}

int send_ack(struct server_data *s_data){
	struct timeval curr;
	gettimeofday(&curr, NULL);
	if (mstime(s_data->last_message) + ACK_TIME < mstime(curr)){
		return send_message(s_data, MESSAGE_ACK, NULL);
	}
	return 1;
}
