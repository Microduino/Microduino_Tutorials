#include <avr/pgmspace.h>
#include <TVout.h>
#include "poofy.h"
#include "room.h"
#include "level.h"
#include "level_bitmap.h"
#include "map_data.h"
#include "bitmap_funcs.h"

#define SIZEOF_BACKDROP_RECORD 10

extern TVout TV;

//store the current room
char level_current_room = 0;
char level_state = LEVEL_PLAYING;
char level_scroll_x = 0;

void level_update()
{
   //handle normal game play events
   if (level_state == LEVEL_PLAYING)
   {
     room_handle_elements();
     poofy_draw();
   } else {
     //handle when we are scrolling the screen
     level_scroll();
   }
}

void level_advance()
{
  level_current_room++;
  if (level_current_room <= 23)
  {
     level_state = LEVEL_SCROLLING;
     level_scroll_x = 0; 
  } else {
     //if we can't advance anymore we have made it to the end (Yay!)
     poofy_win();
  }
}

//draws the first room on the screen
void level_draw()
{
   level_current_room = 0;
   level_scroll_x = 0;
   TV.clear_screen();
   level_state = LEVEL_PLAYING;
      
   //read the level data and draw the level on the screen
   for (char y=0; y < 10; y++)
   {
      for (char x=0; x < 16; x++)
      {
          char current_block = level_get_block(x, y, -1);
          if (current_block > 0) TV.bitmap(x * 8, y * 8, level_bitmap + ((current_block-1) * SIZEOF_BACKDROP_RECORD));
      }
   }
   
   //load the room elements
   room_load_elements(level_current_room);   
}

//handles scrolling the screen left
void level_scroll()
{
  if (level_scroll_x < 16)
  {
     //shift the display left by 8 pixels
     shift_left(8);
  
     //read the level data and draw the level on the screen
     for (char y=0; y < 10; y++)
     {
         char current_block = level_get_block(level_scroll_x, y, -1);
         if (current_block > 0) TV.bitmap(120, y * 8, level_bitmap + ((current_block-1) * SIZEOF_BACKDROP_RECORD));
   
     }  
     level_scroll_x++; 
  } else {
     //we are done scrolling, so return to playing
     level_state = LEVEL_PLAYING;
     
     //load the room elements
     room_load_elements(level_current_room);
  }
}

char level_check_move_h(char x, char y, char &tile_y)
{
   char tile_x_pixels = (x - (x % 8)); 	//calculate the x position in pixels we are checking against
   char testend = x + 8; //calculate the end of testing
   tile_y = y >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   char tile_x = tile_x_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_x_pixels <= testend){
       if(level_get_block(tile_x, tile_y, -1) > 0)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_x++;		//increase tile x map coordinate
       tile_x_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

char level_check_move_v(char x, char y, char &tile_x)
{
   char tile_y_pixels = (y - (y % 8)); 	//calculate the x position in pixels we are checking against
   char testend = y + 8; //calculate the end of testing
   tile_x = x >> 3; //calculate the y position (map coordinates) of the tiles we want to test   
   char tile_y = tile_y_pixels >> 3; //calculate map x coordinate for first tile
 
    //loop while the start point of the test tile is inside the bounding box
    while(tile_y_pixels <= testend){
       if(level_get_block(tile_x, tile_y, -1) > 0)	//is a solid tile is found at tile_x, tile_y?
			return 1;	
			
       tile_y++;		//increase tile x map coordinate
       tile_y_pixels +=8;	//increase tile x pixel coordinate
	}

	return 0;
}

//return the block at a given 'level' x,y coordinate in the current room
char level_get_block(char level_x, char level_y, char room)
{
  char index_ptr = 0;
  //if no room parameter is passed, assume the current
  //room
  if (room == -1) room = level_current_room; 
  if (level_x > 15) level_x = 15;
  if (level_y > 9) level_y = 9;
  
  //determine the index start for room data 
  index_ptr = pgm_read_byte_near(map_room_data + (room * 16) + level_x);
  
  //determine which pattern is being referenced
  //and return this value
  return pgm_read_byte_near (map_pattern_data + (index_ptr * 10) + level_y);
}
