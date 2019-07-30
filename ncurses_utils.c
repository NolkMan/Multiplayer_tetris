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

void ncurses_init(){
}

void cexit(int ret){
	endwin();
	exit(ret);
}
