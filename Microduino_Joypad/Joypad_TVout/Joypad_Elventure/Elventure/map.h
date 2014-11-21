#ifndef __MAP__
#define __MAP__

#define SCROLL_UP 0
#define SCROLL_DOWN 1
#define SCROLL_LEFT 2
#define SCROLL_RIGHT 3

char checkMapRoomMove(char x, char y);
char getMapBlock(char map_x, char map_y, char room);
void drawMapRoom();
void scrollMap(char direction);
char getMapCurrentRoom();
void setMapRoom(char room);

#endif __MAP__
