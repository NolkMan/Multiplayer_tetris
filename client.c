#define true 1
#define false 0

#define _XOPEN_SOURCE 700
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "shared.h"

#include "ncurses_utils.h"
#include "tetris_game.h"
#include "color_sheme.h"

void game_loop(struct tetris_data *data){
	int running = true;
	int frame = 0;
	while (running){
		frame ++;
		int input;
		while ((input=getch()) != -1){
			parse_input(data, input);
		}
		
		if (frame % 6 == 0){
			do_loop(data);
		}
	}
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

	if (ncurses_init() < 0){
		printf("Open colorful terminal to play");
	}
	setup_colors(color_scheme, color_scheme_size);



	cexit(0);
}
