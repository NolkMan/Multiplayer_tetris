#define false 0
#define true 1

#include "tetris_game.h"
#include "stdlib.h"

int piece(struct tetris_data *data){
	return data->pool[data->pool_index];
}

int can_move_left(char board[26][14], int piece, int rot, int y, int x){
	int cx = x-1;
	for (int i=0 ; i < 4; i++){
		for (int j=0 ; j < 4; j++){
			if (pieces[piece][rot][i][j] != ' '){
			   if (board[y+i][cx+j] != 0){
				   return false;
			   }
			}
		}
	}
	return true;
}

int can_move_right(char board[26][14], int piece, int rot, int y, int x){
	int cx = x+1;
	for (int i=0 ; i < 4; i++){
		for (int j=0 ; j < 4; j++){
			if (pieces[piece][rot][i][j] != ' '){
			   if (board[y+i][cx+j] != 0){
				   return false;
			   }
			}
		}
	}
	return true;
}

int can_rotate(char board[26][14], int piece, int rot, int y, int x){
	int crot = rot + 1;
	crot %= 4;
	for (int i=0 ; i < 4; i++){
		for (int j=0 ; j < 4; j++){
			if (pieces[piece][crot][i][j] != ' '){
			   if (board[y+i][x+j] != 0){
				   return false;
			   }
			}
		}
	}
	return true;
}

int can_fall(char board[26][14], int piece, int rot, int y, int x){
	int cy = y+1;
	for (int i=0 ; i < 4; i++){
		for (int j=0 ; j < 4; j++){
			if (pieces[piece][rot][i][j] != ' '){
			   if (board[cy+i][x+j] != 0){
				   return false;
			   }
			}
		}
	}
	return true;
}

void randomize_pool(int* pool){
	for (int i=0 ; i < 7; i++){
		pool[i] = i;
	}
	for (int i = 6; i >= 1; i--){
		int num = rand() % i;
		int temp = pool[num];
		pool[num] = pool[i];
		pool[i] = temp;
	}
}

void shift_pool(int *pool){
	for (int i =0 ; i < 7; i++){
		pool[i] = pool[i+7];
	}
}

void add_to_board(struct tetris_data *data){
	for (int i=0 ; i < 4; i++){
		for (int j=0; j < 4;j++){
			if (pieces[piece(data)][data->rot][i][j] != ' '){
				data->board[data->y+i][data->x+j] = piece(data)+'a';
			}
		}
	}
}

void clear_line(char board[26][14], int line){
	for (int i=2 ; i < 12; i++){
		board[line][i] = 0;
	}
}

int line_to_clear(char board[26][14], int line){
	for (int i = 2; i < 12; i++){
		if (board[line][i] == 0){
			return false;
		}
	}
	return true;
}

void collapse_line(char board[26][14], int line){
	for (int i=2; i < 12; i++){
		board[line+1][i] = board[line][i];
		board[line][i] = 0;
	}
}

int check_for_lines(char board[26][14]){
	int cleared = 0;
	for (int i=4; i < 24; i++){
		if (line_to_clear(board, i)){
			clear_line(board, i);
			cleared++;
			for (int j=i-1; j>= 4; j--){
				collapse_line(board, j);
			}
		}
	}
	return cleared;
}

int check_if_dead(char board[26][14]){
	for (int i=2; i < 12; i++){
		if (board[3][i] != 0){
			return true;
		}
	}
}

struct tetris_data create_new_game(){
	struct tetris_data new_game;

	for (int i = 0; i < 26; i++){
		for (int j = 0; j < 14; j++){
			new_game.board[i][j] = 0;
		}
	}

	for (int i=0; i < 26;i++){
		new_game.board[i][0] = 'z';
		new_game.board[i][1] = 'z';
		new_game.board[i][12] = 'z';
		new_game.board[i][13] = 'z';
	}
	for (int i=0 ; i < 14; i++){
		new_game.board[24][i] = 'z';
		new_game.board[25][i] = 'z';
	}

	randomize_pool(new_game.pool);
	randomize_pool(new_game.pool + 7);

	new_game.x = 6;
	new_game.y = 0;
	new_game.rot = 0;
	new_game.pool_index = 0;
	new_game.is_dead = false;

	return new_game;
}

void parse_input(struct tetris_data *data, int input){
	switch(input){
		case 'w': 
			if (can_rotate(data->board, piece(data), 
						data->rot, data->y, data->x)) 
				{data->rot++; data->rot%=4;}  
			break;
		case 'a': 
			if (can_move_left(data->board, piece(data), 
						data->rot, data->y, data->x)) 
				data->x--; 
			break;
		case 'd': 
			if (can_move_right(data->board, piece(data), 
						data->rot, data->y, data->x)) 
				data->x++; 
			break;
		case 's': 
			while (can_fall(data->board,piece(data),
						data->rot,data->y,data->x)) 
				data->y++; 
			break;
	}
}

void do_loop(struct tetris_data *data){
	if (can_fall(data->board, piece(data), data->rot, data->y, data->x)){
		data->y++;
	}else{
		add_to_board(data);
		data->pool_index++;
		data->rot = 0;
		data->y = 0;
		data->x = 6;
		if (data->pool_index >= 7){
			
			data->pool_index = 0;
			shift_pool(data->pool);
			randomize_pool(data->pool+7);
		}
		int s = check_for_lines(data->board);
		data->score += s*s;
	}

	if (check_if_dead(data->board)){
		data->is_dead = true;
	}
}
