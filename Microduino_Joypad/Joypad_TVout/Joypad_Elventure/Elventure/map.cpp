#include <avr/pgmspace.h>
#include <TVout.h>
#include "map.h"
#include "map_bitmap.h"
#include "map_data.h"
#include "room.h"
extern TVout TV;

#define SIZEOF_MAP_RECORD 10

#define SCROLL_UP 0
#define SCROLL_DOWN 1
#define SCROLL_LEFT 2
#define SCROLL_RIGHT 3

//store the current room
char map_curr_room = 0;

//check the current room movement
char checkMapRoomMove(char x, char y)
{
	char map_x = x / 8;
	char map_y = y / 8;
        if ((map_x > 11) || (map_y > 7)) return 0;
	return getMapBlock(map_x, map_y, -1);
}

//return the map block based on map coordinates
char getMapBlock(char map_x, char map_y, char room)
{
  char index_ptr = 0;
  //if no room parameter is passed, assume the current
  //room
  if (room == -1) room = map_curr_room; 
  
  //determine the index start for room data 
  index_ptr = pgm_read_byte_near(map_room_data + (room * 12) + map_x);
  
  //determine which pattern is being referenced
  //and return this value
  return pgm_read_byte_near (map_pattern_data + (index_ptr * 8) + map_y);
}

void drawMapRoom()
{ 
  for (char y=0;y<8; y++)
  {
	  for (char x=0; x<12;x++)
	  {
	     //determine the current block before trying to draw
		 int curr_block = getMapBlock(x,y,-1);
		 TV.bitmap(x * 8, y * 8, map_bitmap + (curr_block * SIZEOF_MAP_RECORD));
	  }
  }
  
  loadRoomElemments(map_curr_room);
}

//initiates the scrolling of the map in a particular direction
void scrollMap(char direction)
{
  switch (direction)
  {
     case SCROLL_UP:
	   if (map_curr_room > 0) map_curr_room -= MAP_WIDTH;
	   break;
	   
	 case SCROLL_DOWN:
	   if (map_curr_room < MAP_ROOM_COUNT) map_curr_room += MAP_WIDTH;
	   break;
	   
	 case SCROLL_LEFT:
	   if (map_curr_room > 0) map_curr_room--;
	   break;
	   
	 case SCROLL_RIGHT:
	   if (map_curr_room < MAP_ROOM_COUNT) map_curr_room++;
	   break;
  }
  
  drawMapRoom();
}

//return the current room value
char getMapCurrentRoom() { return map_curr_room; }

//set the map to a specific room (portals)
void setMapRoom(char room)
{
  map_curr_room = room;
  drawMapRoom();
}

