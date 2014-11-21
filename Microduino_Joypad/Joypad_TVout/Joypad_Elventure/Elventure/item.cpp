#include <avr/pgmspace.h>
#include <TVout.h>
#include "item.h"
#include "map.h"
#include "map_bitmap.h"
#include "item_bitmap.h"
#include "room.h"
#include "bitmap_funcs.h"

extern TVout TV;

#define STEP_LENGTH  4
#define SIZEOF_ITEM_RECORD 10

RoomElement moveItem(RoomElement element)
{
   if (element.state > STATE_HIDDEN) 
   {
    //erase the old item image (using blank map tile)
    TV.bitmap(element.x, element.y, map_bitmap);
  
    element.step++;
    if (element.step > 2) element.step = 1;
   
    switch (element.state)
    {
	   case STATE_MOVE_UP:
           element.state = STATE_HIDDEN;
           if (element.y > 4)
           {
             if (checkMapRoomMove(element.x, element.y - 4) == 0)
			 {
			   element.y -= STEP_LENGTH;
               element.state = STATE_MOVE_UP;
			 }
		   }
	   break;
	   
	  case STATE_MOVE_DOWN:   
           element.state = STATE_HIDDEN;
           if (element.y < 48)
           {
             if (checkMapRoomMove(element.x, element.y + 8) == 0)
			 {
			   element.y += STEP_LENGTH;
			   element.state = STATE_MOVE_DOWN;
			 }
		   }
	   break;	   

	  case STATE_MOVE_LEFT:
           element.state = STATE_HIDDEN;
           if (element.x > 4)
           {
             if (checkMapRoomMove(element.x - 4, element.y) == 0)
			 {
			   element.x -= STEP_LENGTH;
			   element.state = STATE_MOVE_LEFT;
			 }
		   }
	   break;	   
	   
	  case STATE_MOVE_RIGHT:
           element.state = STATE_HIDDEN;
           if (element.x < 80)
           {
             if (checkMapRoomMove(element.x + 12, element.y) == 0)
			 {
			   element.x += STEP_LENGTH;
			   element.state = STATE_MOVE_RIGHT;
			 }
		   }
	   break;		      
    }

    //draw new item bitmap
    if (element.state > STATE_HIDDEN) overlaybitmap(element.x, element.y, item_bitmap + ( ((element.type - 51) + (element.step)) * SIZEOF_ITEM_RECORD));
	
	//decrement counter, if active
	if (element.counter > 0) element.counter--;	
  }
  return element;
}

RoomElement hitItem(RoomElement element)
{
  //erase the old item image (using blank map tile)
  TV.bitmap(element.x, element.y, map_bitmap);
  
  element.state = STATE_HIDDEN;
  return element;
}


