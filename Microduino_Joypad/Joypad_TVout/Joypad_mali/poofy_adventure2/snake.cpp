#include <avr/pgmspace.h>
#include <TVout.h>
#include "snake.h"
#include "snake_bitmap.h"
#include "bitmap_funcs.h"
#include "room.h"
#include "level.h"

#define SIZEOF_SNAKE_RECORD 10
#define SNAKE_VELOCITY 2

RoomElement snake_move(RoomElement element)
{
   char facing = 0;
   char tile_adjust;
   
   //erase the old snake image
   eraseBitmapRect(element.x, element.y, snake_bitmap);
   switch (element.state)
   {
      case STATE_MOVE_LEFT:
	    facing = 0;
		if (level_check_move_v(element.x - SNAKE_VELOCITY, element.y, tile_adjust) > 0)
		{
		   element.x = ((tile_adjust + 1) * 8) + 1;
		   element.state = STATE_MOVE_RIGHT;
		} else {
           if (element.x - SNAKE_VELOCITY > 0)
		   {
		      element.x -= SNAKE_VELOCITY;
		   } else {
		      //otherwise off the map, so hide it.
              element = snake_hit(element);
		   }
		}
		
	    //test for gravity pulling down (if nothing is below)
		if (level_check_move_h(element.x, element.y+9, tile_adjust) == 0) element.state = STATE_FALL_LEFT;		
	  break;
	  
	  case STATE_MOVE_RIGHT:
	    facing = 2;
		if (level_check_move_v(element.x + SNAKE_VELOCITY + 8, element.y, tile_adjust) > 0)
		{
		   element.x = (tile_adjust * 8) - 9;
		   element.state = STATE_MOVE_LEFT;
		} else {
           if (element.x + SNAKE_VELOCITY < 118)
		   {
		      element.x += SNAKE_VELOCITY;
		   } else {
		      //otherwise off the map, so hide it.
              element = snake_hit(element);
		   }
		}
		
	    //test for gravity pulling down (if nothing is below)
		if (level_check_move_h(element.x, element.y+9, tile_adjust) == 0) element.state = STATE_FALL_RIGHT;				
	  break;
	  
	  case STATE_FALL_LEFT:
	    facing = 0;	
		if (level_check_move_h(element.x, element.y+9, tile_adjust) > 0)
		{
		   element.y = (tile_adjust * 8) - 9;
		   element.state = STATE_MOVE_LEFT;
		} else {
		   if (element.y < 80)
		   {
		     element.y += SNAKE_VELOCITY;
		   } else {
		     //fall off map, so hide the snake
		     element = snake_hit(element);
		   }
		}
	  break;
	  
	  case STATE_FALL_RIGHT:
	    facing = 2;
		if (level_check_move_h(element.x, element.y+9, tile_adjust) > 0)
		{
		   element.y = (tile_adjust * 8) - 9;
		   element.state = STATE_MOVE_RIGHT;
		} else {
		   if (element.y < 80)
		   {
		     element.y += SNAKE_VELOCITY;
		   } else {
		     //fall off map, so hide the snake
		     element = snake_hit(element);
		   }
		}		
	  break;
	  
	  case STATE_VISIBLE:
        element.state = STATE_MOVE_LEFT;
		facing = 0;
	  break;
   }
   element.step++;
   if (element.step > 1) element.step = 0;
  
   if (element.state > STATE_HIDDEN) overlaybitmap(element.x, element.y, snake_bitmap + (element.step + facing) * SIZEOF_SNAKE_RECORD);
   return element;
}

RoomElement snake_hit(RoomElement element)
{
  //erase the old snake image
  eraseBitmapRect(element.x, element.y, snake_bitmap);
		 
  element.state = STATE_HIDDEN;
  return element;
}


