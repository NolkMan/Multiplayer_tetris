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

void game_loop(struct tetris_data *data, struct server_data *s_data){
	int running = true;
	int frame = 0;
	while (running){
		frame ++;
		int input;
		while ((input=getch()) != -1){
			parse_input(data, input);
		}
		int code = get_message(s_data);
		if (code != NO_MESSAGE){
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
				write(s_data->sock_fd, message, strlen(message));
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

void lobby_loop(struct server_data *s_data){
	int running = true;
	while (running){
		int code = get_message(s_data);
		if (code != NO_MESSAGE){
			if (code == MESSAGE_GAME_START){
				struct tetris_data data = create_new_game();
				game_loop(&data, s_data);
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

	struct server_data s_data;
	s_data.sock_fd = sock_fd;
	s_data.buff_size=64;
	s_data.buff_i = 0;
	memset(s_data.buff, '\0', sizeof(s_data.buff));

	lobby_loop(&s_data);

	cexit(0);
}
