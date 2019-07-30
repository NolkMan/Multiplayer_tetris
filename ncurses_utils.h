#ifndef NCURSES_UTILS_H
#define NCURSES_UTILS_H
/**
 * Will draw a single digit
 */
void draw_digit(int y, int x, int digit, int color_fg, int color_bg);

void draw_controls(int y, int x, int color_fg, int color_bg, char up, char down, char left, char right);

/**
 * Will nicely initialize ncureses
 */
void ncurses_init();

/**
 * Will exit with properly closing ncurses
 */
void cexit(int ret);
#endif

