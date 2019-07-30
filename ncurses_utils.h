#ifndef NCURSES_UTILS_H
#define NCURSES_UTILS_H
/**
 * Will draw a single digit
 */
void draw_digit(int y, int x, int digit, int color_fg, int color_bg);

/**
 * Will draw control d-pad on screen with specified controls
 */
void draw_controls(int y, int x, int color_fg, int color_bg, char up, char down, char left, char right);

/**
 * Will draw and color given symbol based on color map
 * \param symbol given symbol to draw
 * \param map of colors for every character in symbol
 */
void draw_symbol(int y, int x, int h, int w, char ** symbol, int *map);

/**
 * Will nicely initialize ncureses
 * \return -1 On fail and 1 on success
 */
int ncurses_init();

/**
 * Will exit with properly closing ncurses
 */
void cexit(int ret);
#endif
