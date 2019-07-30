#include "ncurses_utils.h"

#include "ncurses.h"
#include "stdlib.h"

void draw_controls(int y, int x, int primary_color, int secondary_color, char up, char down, char left, char right){
	attron(COLOR_PAIR(secondary_color));
	mvprintw(y,  x+3,"     ");
	mvprintw(y+1,x+3,"     ");
	mvprintw(y+2,x,"           ");
	mvprintw(y+3,x,"           ");
	mvprintw(y+4,x,"           ");
	attron(COLOR_PAIR(primary_color));
	mvprintw(y+1, x+5, "%c", up);
	mvprintw(y+3, x+5, "%c", down);
	mvprintw(y+3, x+2, "%c", left);
	mvprintw(y+3, x+8, "%c", right);
}

const char DIGITS[10][5][6] = {
	{" ### ",
	 "#   #",
	 "#   #",
	 "#   #",
	 " ### "},
	{"  #  ",
	 " ##  ",
	 "  #  ",
	 "  #  ",
	 " ### "},
	{" ### ",
	 "#   #",
	 "  ## ",
	 " #   ",
	 "#####"},
	{" ### ",
	 "#   #",
	 "  ## ",
	 "#   #",
	 " ### "},
	{"#   #",
	 "#   #",
	 " ####",
	 "    #",
	 "    #"},
	{"#####",
	 "#    ",
	 "#### ",
	 "    #",
	 "#### "},
	{" ### ",
	 "#    ",
	 "#### ",
	 "#   #",
	 " ### "},
	{"#####",
	 "   # ",
	 "  #  ",
	 " #   ",
	 "#    "},
	{" ### ",
	 "#   #",
	 " ### ",
	 "#   #",
	 " ### "},
	{" ### ",
	 "#   #",
	 " ####",
	 "    #",
	 " ### "},
};

void draw_digit(int y, int x, int digit, int color_fg, int color_bg){
	for (int i=0 ; i < 5; i++){
		for (int j = 0; j < 5; j++){
			if (DIGITS[digit][i][j] == ' '){
				attron(COLOR_PAIR(color_bg));
			}else{
				attron(COLOR_PAIR(color_fg));
			}
			mvprintw(y+i, x+j, " ");
		}
	}
}

void draw_symbol(int y, int x, int h, int w, char ** symbol, int *map){
	for (int i =0 ; i < h; i++){
		for (int j = 0; j < w; j++){
			attron(COLOR_PAIR(map[symbol[i][j]]));
			mvprintw(y+i, x+j, "  ");
		}
	}
}

int ncurses_init(){
	initscr();
	timeout(0);
	noecho();
	curs_set(0);

	if (has_colors() == FALSE){
		return -1;
	}
	start_color();
	return 1;
}

void setup_colors(int (*color_scheme)[3], int scheme_size){
	for (int i =0 ; i < scheme_size; i++){
		init_pair(color_scheme[i][0], color_scheme[i][1], color_scheme[i][2]);
	}
}

void cexit(int ret){
	endwin();
	exit(ret);
}
