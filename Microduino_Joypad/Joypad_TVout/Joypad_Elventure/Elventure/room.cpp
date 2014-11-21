#include <avr/pgmspace.h>
#include <TVout.h>
#include <string.h>
#include "room.h"
#include "room_data.h"
#include "monster.h"
#include "item.h"
#include "elf.h"

RoomElement roomElements[MAX_ELEMENT_RECORDS];
char element_count = 0;

void addRoomElement(char type, char x, char y, char state, char counter)
{
      roomElements[element_count].id = element_count;
      roomElements[element_count].type = type;
      roomElements[element_count].x = x;
      roomElements[element_count].y = y;
      roomElements[element_count].step = 1;
      roomElements[element_count].state = state;
	  roomElements[element_count].counter = counter;
      element_count++;
}

//load room elements (if present)
void loadRoomElemments(char room)
{
  //clear the array of the previous contents
  memset(roomElements, 0, sizeof(roomElements));
  element_count = 0;
  
  //add in the item (sword) room element automatically
  addRoomElement(ITEM_SWORD, 0, 0, STATE_HIDDEN, 0);
  
  int index_ptr = 0;
  
  //determine the index start for room elements
  index_ptr = pgm_read_byte_near(room_element_index_data + room);

  //read until we have reached the max number of element records
  if (index_ptr < 255)
  {
    //set to the actual data position
	index_ptr *= 4;
	while (element_count < MAX_ELEMENT_RECORDS)
    {
	  //only add the item to the room if the elf does not already have this in inventory
	  if (elfHasItem(pgm_read_byte_near(room_element_data + index_ptr)) == false) addRoomElement(pgm_read_byte_near(room_element_data + index_ptr), pgm_read_byte_near(room_element_data + (index_ptr+1)), pgm_read_byte_near(room_element_data + (index_ptr+2)), STATE_VISIBLE, 0);
	
      //look to see if we have reached the end of data for the room
      if (pgm_read_byte_near(room_element_data + (index_ptr+3)) == 255) break;
	
      //otherwise, we continue advancing through the data
	  index_ptr += 4;
    }
  }
}

//handles the room elements
void handleRoomElements()
{
  //retrieve elf information
  Elf elf = getElf();
  
  for (char i=0; i < element_count; i++)
  {
    //if the element is hidden, skip past it
    if (roomElements[i].state > STATE_HIDDEN)
	{	   	   
       //test room elements for a collision with the elf
	   if (testRoomElement(roomElements[i], elf.x, elf.y, 16)) roomElements[i] = hitElf(roomElements[i]);
	  
	   //determine the type of element and handle the behaviors
	   if (roomElements[i].type < 50)
	   {		 
		 //test for monster being hit by the weapon (if it is active)
		 //room element 0 is always the weapon
		 if (roomElements[0].state > STATE_HIDDEN) {
		   if (testRoomElement(roomElements[i], roomElements[0].x, roomElements[0].y, 8))
		   { 
		     roomElements[i] = hitMonster(roomElements[i]);
		     roomElements[0] = hitItem(roomElements[0]);
		   }
		 }
		 
		 //handle monster movement
         roomElements[i] = moveMonster(roomElements[i]);

	   }  else {
         roomElements[i] = moveItem(roomElements[i]);
		 
		 //hide the heart if the timer has run out 
		 if ((roomElements[i].type == ITEM_HEART) && (roomElements[i].counter == 0)) roomElements[i] = hitItem(roomElements[i]);
	   }
	}
  }
}

//test for collision in room elements
bool testRoomElement (RoomElement element, char testX, char testY, char ySize)
{
  bool  is_hit = false;
  if (((element.x >= testX) && (element.x <= testX + 8)) || ((element.x + 8 >= testX) && (element.x + 8 <= testX + 8)))
  {
    //determine the type of element and handle the behaviors
	if (element.type < 50)
	{		 
       if (((element.y >= testY) && (element.y <= testY + ySize )) || ((element.y + ySize >= testY) && (element.y + ySize <= testY + ySize ))) is_hit = true;
    } else {
	   if (((element.y >= testY) && (element.y <= testY + 8 )) || ((element.y + 16 >= testY) && (element.y + 16 <= testY + 8 ))) is_hit = true;
	}
  }

  return is_hit;
}

//handles updating the room element
void updateRoomElement (RoomElement element)
{
  roomElements[element.id] = element;
}

//handles retrieving the room element
RoomElement getRoomElement (char id)
{
  return roomElements[id];
}
