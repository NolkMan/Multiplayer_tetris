#define true 1
#define false 0

#define MENU_LOOP 0
#define LOBBY_LOOP 1
#define GAME_LOOP 2

#define _XOPEN_SOURCE 700
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "sys/time.h"

#include "shared.h"

#include "ncurses_utils.h"
#include "tetris_game.h"
#include "color_sheme.h"

#include "client_server_comm.h"
#include "comm_utils.h"

/**
 * A single step of game loop
 * \returns next loop step
 */
int game_loop(struct server_data *s_data){
	struct tetris_data *data = s_data->t_data;

	data->frame ++;
	int input;
	while ((input=getch()) != -1){
		parse_input(data, input);
	}
	
	if (data->frame % 6 == 0){
		int d = data->is_dead;
		if (!d)
			do_loop(data);
		if (data->score_updated){
			char scorestr[10];
			sprintf(scorestr, "%d", data->score);
			int succ = send_message(s_data, MESSAGE_SCORE, scorestr);
			if (!succ) { cexit(0); }
			data->score_updated = 0;
		}
		if (d != data->is_dead){
			int succ = send_message(s_data, MESSAGE_DEATH, NULL);
			if (!succ) { cexit(0); }
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

	int x = 50;
	int y = 2;

	int cx = x;
	int cscore = data->score;
	draw_number(y, x, data->score, 41, 1);

	draw_number(y+9, x, data->max_score, 41, 1);

	refresh();

	usleep(60000);
	return GAME_LOOP;
}

/**
 * A single step of lobby loop
 * \returns next loop step to do
 */
int lobby_loop(struct server_data *s_data){ 
	int input;
	while ((input=getch()) != -1){
	}
	erase();

	write_line(1,1, "This is lobby");

	refresh();
	return LOBBY_LOOP;
}

int menu_loop(struct server_data *s_data){
	int input;
	while ((input=getch()) != -1){
	}
	return MENU_LOOP;
}

/**
 * Manages messages sent from server
 * May change state
 */
void manage_connection(struct server_data *s_data, int *state){
	char *param = NULL;
	int code;
	while ((code = get_message(s_data, &param)) != NO_MESSAGE){
		if (code == MESSAGE_GAME_START){
			*(s_data->t_data) = create_new_game();
			int succ = send_message(s_data, MESSAGE_GAME_STARTED, NULL);
			if (!succ) { cexit(0); }
			*state = GAME_LOOP;
		}
		if (code == MESSAGE_MAX_SCORE){
			s_data->t_data->max_score = atoi(param);
		}
		if (code == MESSAGE_GAME_END){
			*state = LOBBY_LOOP;
		}
		if (code == MESSAGE_TIMEOUT){ // TODO Do this better
			printf("Timed out\n");
			cexit(0);
		}
		if (param != NULL){ 
			free(param);
			param = NULL;
		}
	}
	
	if (!send_ack(s_data)){
		cexit(0);
	}
}

void loop(struct server_data *s_data){
	int running = true;
	int state = LOBBY_LOOP;
	while (running){
		manage_connection(s_data, &state);
		switch (state){
			case MENU_LOOP:
				state = menu_loop(s_data);
				break;
			case LOBBY_LOOP:
				state = lobby_loop(s_data);
				break;
			case GAME_LOOP:
				state = game_loop(s_data);
				break;
		}
	}
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

	struct server_data s_data = create_server_data(sock_fd);

	// TODO Send username
	{
	}

	loop(&s_data);

	free(s_data.t_data);

	cexit(0);
}
