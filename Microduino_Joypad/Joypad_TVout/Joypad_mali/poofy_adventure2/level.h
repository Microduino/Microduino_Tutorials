#ifndef __LEVEL__
#define __LEVEL__

//state constants
#define LEVEL_PLAYING 0
#define LEVEL_SCROLLING 1

void level_draw();
void level_advance();
void level_scroll();
void level_update();
char level_check_move_h(char x, char y, char &tile_x);
char level_check_move_v(char x, char y, char &tile_x);
char level_get_block(char level_x, char level_y, char room);

#endif __LEVEL__
