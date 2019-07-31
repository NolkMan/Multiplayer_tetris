#define true 1
#define false 0

#define _XOPEN_SOURCE 700
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

#include "shared.h"

#include "ncurses_utils.h"
#include "tetris_game.h"
#include "color_sheme.h"

#include "client_server_comm.h"
#include "comm_utils.h"

void game_loop(struct tetris_data *data, int sock_fd){
	int running = true;
	int frame = 0;
	size_t buff_size = 64;
	char buf[64];
	while (running){
		frame ++;
		int input;
		while ((input=getch()) != -1){
			parse_input(data, input);
		}
		if (get_message(sock_fd,buf,buff_size)){
			int code = check_for_message(buf);
			if (code == MESSAGE_GAME_END){
				running = false;
			}
		}
		
		if (frame % 6 == 0){
			int d = data->is_dead;
			do_loop(data);
			
			if (d != data->is_dead){
				char * message = generate_message(MESSAGE_DEATH);
				if (message == NULL){
					cexit(0);
				}
				write(sock_fd, message, strlen(message));
				free(message);
			}
		}

		erase();

		for (int i=0 ; i < 26; i ++){
			draw_line(i,0,14, data->board[i], color_map);
		}

		for (int i=0; i < 4; i++){
			draw_line(data->y+i, data->x, 4, 
					pieces[data->pool[data->pool_index]][data->rot][i], color_map);
		}

		refresh();

		usleep(60000);
	}
}

void lobby_loop(int sock_fd){
	size_t buff_size = 64;
	char buf[64];
	int running = true;
	while (running){
		if (get_message(sock_fd,buf,buff_size)){
			int code = check_for_message(buf);
			if (code == MESSAGE_GAME_START){
				struct tetris_data data = create_new_game();
				game_loop(&data, sock_fd);
			}
		}
	}
}

void menu_loop(){
}

int main(int argc, char **argv){
	if (argc < 2){
		printf("Please provide an ip address of a server\n");
		exit(0);
	}

	if (ncurses_init() < 0){
		printf("Open colorful terminal to play");
	}
	setup_colors(color_scheme, color_scheme_size);

	int sock_fd = connect_to_server(argv[1]);
	if (sock_fd < 0){
		cexit(0);
	}
	lobby_loop(sock_fd);

	cexit(0);
}
