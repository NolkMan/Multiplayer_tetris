#ifndef TETRIS_GAME_H
#define TETRIS_GAME_H

#include "pieces.h"

struct tetris_data{
	char board[26][14];
	int x, y;
	int rot;
	int pool[14]; 
	int pool_index;
	int score;
	int score_updated;
	int max_score;
	int is_dead;
	int frame;
};

struct tetris_data create_new_game();

void parse_input(struct tetris_data *data, int input);

void do_loop(struct tetris_data *data);


#endif
