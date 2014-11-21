#include <avr/pgmspace.h>
#include <string.h>
#include <TVout.h>
#include "elf.h"
#include "elf_bitmap.h"
#include "map_bitmap.h"
#include "map.h"
#include "room.h"
#include "item.h"
#include "display.h"
#include "sound.h"
#include "bitmap_funcs.h"
extern TVout TV;

#define SIZEOF_ELF_RECORD 10

Elf elf = {FACING_DOWN, 1, 36, 24, 3, {0,0,0,0}, ELFSTATE_PLAYING};

void resetElf(bool reset_items)
{
  elf.facing = FACING_DOWN;
  elf.step = 1;
  elf.x =  36;
  elf.y = 24;
  elf.hearts = 3;
  elf.state = ELFSTATE_PLAYING;
  if (reset_items) memset(elf.items, 0, sizeof(elf.items));
}

void showElf()
{
  moveElf(elf.facing);
}

void moveElf(unsigned char facing)
{
  //erase the old elf image (using blank map tile)
  TV.bitmap(elf.x, elf.y, map_bitmap);
  TV.bitmap(elf.x, elf.y + 8, map_bitmap);
  
  //if it is a new facing, then reset the step
  if (facing != elf.facing)
  {
     elf.step = 1;
  } else {
     elf.step++;
	 if (elf.step > 2) elf.step = 1;
  }
  
  elf.facing = facing;
  
  switch (facing)
  {
     case FACING_DOWN:
	   if (elf.y < 48)
           {
             if (checkMapRoomMove(elf.x, elf.y + 16) == 0) 
	        if (checkMapRoomMove(elf.x+4, elf.y + 16) == 0) elf.y += STEP_LENGTH;
           } else {
	     scrollMap(SCROLL_DOWN);
    	     elf.x = 36;
             elf.y = 8;
             elf.facing = FACING_DOWN;
	   }
	   break;
		
	case FACING_UP:
           if (elf.y > 4)
           {
             if (checkMapRoomMove(elf.x, elf.y - 4) == 0)
	        if (checkMapRoomMove(elf.x + 4, elf.y - 4) == 0) elf.y -= STEP_LENGTH;
           } else {
	     scrollMap(SCROLL_UP);
	     elf.x = 36;
	     elf.y = 32;
	     elf.facing = FACING_UP;
	   }
	   break;
	   
	case FACING_LEFT:
	   if (elf.x > 4)
           {
             if (checkMapRoomMove(elf.x - 4, elf.y) == 0)
                if (checkMapRoomMove(elf.x - 4, elf.y + 12) == 0) elf.x -= STEP_LENGTH;
           } else {
	       scrollMap(SCROLL_LEFT);
	       elf.x = 64;
	       elf.y = 24;
	       elf.facing = FACING_LEFT;
	   }
           break;

        case FACING_RIGHT:
           if (elf.x < 80)
           {
             if (checkMapRoomMove(elf.x + 12, elf.y) == 0)
               if (checkMapRoomMove(elf.x + 12, elf.y + 12) == 0) elf.x += STEP_LENGTH;	
           } else {
	       scrollMap(SCROLL_RIGHT);
	       elf.x = 16;
	       elf.y = 24;
	       elf.facing = FACING_RIGHT;
	   }
           break;
  }
  
  //draw new elf bitmap
  overlaybitmap(elf.x, elf.y, elf_bitmap + ( elf.facing * SIZEOF_ELF_RECORD));
  overlaybitmap(elf.x, elf.y+8, elf_bitmap + ((elf.facing + elf.step) * SIZEOF_ELF_RECORD)); 
}

void throwSword()
{
  //retrieve the sword room element
  RoomElement element = getRoomElement(0);

  //if it is already active, do nothing, otherwise
  //initiate the sword being thrown
  if (element.state == STATE_HIDDEN)
  {
	  switch (elf.facing)
	  {
		case FACING_DOWN:
			element.state = STATE_MOVE_DOWN;
			element.x = elf.x;
			element.y = elf.y + 16;
			break;
			
		case FACING_UP:
			element.state = STATE_MOVE_UP;
			element.x = elf.x;
			element.y = elf.y - 8;			
			break;

		case FACING_LEFT:
			element.state = STATE_MOVE_LEFT;
			element.x = elf.x - 8;
			element.y = elf.y;				
			break;
		
		case FACING_RIGHT:
			element.state = STATE_MOVE_RIGHT;
			element.x = elf.x + 8;
			element.y = elf.y;	
			break;
	  }  
	  updateRoomElement(element);
  }
}

RoomElement hitElf(RoomElement element)
{
  //hit by a monster
  if (element.type < 50)
  {
    //check the counter, so hearts are not 
	//removed unless the monster has not been 'hit'
	//already
    if (element.counter == 0)
	{
      element.counter = COUNTER_START;
      elf.hearts--;
      if (elf.hearts < 1)
      {
        //game over
		elf.state = ELFSTATE_DEAD;
      }
	}
	
	//when the elf and a monster 'bump,' move the monster
	//in the opposite direction
	switch (element.state)
	{
	  case STATE_MOVE_UP:
	     element.state = STATE_MOVE_DOWN;
		 break;
		 
	  case STATE_MOVE_DOWN:
	     element.state = STATE_MOVE_UP;
		 break;

	  case STATE_MOVE_LEFT:
	     element.state = STATE_MOVE_RIGHT;
		 break;		 

	  case STATE_MOVE_RIGHT:
	     element.state = STATE_MOVE_LEFT;
		 break;	 
	}
  } else {
	switch (element.type)
	{
       case ITEM_HEART:
         if (elf.hearts < MAX_HEARTS) elf.hearts++;
         //handle the rest of the item hit
         element = hitItem(element);
         play_sfx(5);		 
		 break;
		 
       case ITEM_CRYSTAL:
	   case ITEM_ORB:
	   case ITEM_ARMOR:
	   case ITEM_STAFF:
	      addElfItem(element.type);
          //handle the rest of the item hit
          element = hitItem(element);			  
	      break;
		  
	   case ITEM_PORTAL:
             //handle the rest of the item hit
              element = hitItem(element);
              
	      if (getMapCurrentRoom() > 63)
		  {
		    //go to the bottom half of the map (underworld)
		    setMapRoom(0);
		    play_song(0);
		  } else {
		    //back to top half of the map (overworld)
		    setMapRoom(64);
	            play_song(1);
		  }
		  elf.x = 36;
		  elf.y = 24;
		  elf.facing = FACING_DOWN;
                  showElf();
	      break;
	} 	
  }
  
  //update the display
  updateDisplay(elf);
  
  return element;
}
  
Elf getElf()
{
    return elf;
}

//adds the item in the elf's inventory
void addElfItem(char type)
{
  char count = 0;
  for (char i=0; i< MAX_ITEMS; i++)
  {
    if (elf.items[i] == 0)
	{
	  elf.items[i] = type;
	  break;
	} else {
	  count++;
	}
  }
  
  if (count == MAX_ITEMS)
  {
    //won game
	elf.state = ELFSTATE_WON;
  } else {
    //otherwise, play the sound effect
    play_sfx(6);  
  }
}

//check the elf's inventory for a specific item
bool elfHasItem(char type)
{
  for (char i=0; i< MAX_ITEMS; i++)
  {
    if ((elf.items[i] > 50) && (elf.items[i] == type)) return true;
  } 
  return false;  
}

//check the elf's current state
char getElfState()
{
  return elf.state;
}



