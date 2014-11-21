#include <avr/pgmspace.h>
#include <TVout.h>
#include <string.h>
#include "room.h"
#include "room_data.h"
#include "snake.h"
#include "fruit.h"
#include "poofy.h"

RoomElement roomElements[MAX_ELEMENT_RECORDS];
char element_count = 0;

void room_add_element(char type, char x, char y, char state)
{
      roomElements[element_count].id = element_count;
      roomElements[element_count].type = type;
      roomElements[element_count].x = x;
      roomElements[element_count].y = y;
      roomElements[element_count].step = 1;
      roomElements[element_count].state = state;
	  
      //if this is a fruit, draw it on the screen
      if (type > 49) fruit_draw(roomElements[element_count]);
      element_count++;
}

//load room elements (if present)
void room_load_elements(char room)
{
  //clear the array of the previous contents
  memset(roomElements, 0, sizeof(roomElements));
  element_count = 0;
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
	  room_add_element(pgm_read_byte_near(room_element_data + index_ptr), pgm_read_byte_near(room_element_data + (index_ptr+1)), pgm_read_byte_near(room_element_data + (index_ptr+2)), STATE_VISIBLE);
      
      //look to see if we have reached the end of data for the room
      if (pgm_read_byte_near(room_element_data + (index_ptr+3)) == 255) break;
	
      //otherwise, we continue advancing through the data
	  index_ptr += 4;
    }
  }
}

//handles the room elements
void room_handle_elements()
{
  for (char i=0; i < element_count; i++)
  {
    //if the element is hidden, skip past it
    if (roomElements[i].state > STATE_HIDDEN)
	{	   	   
       //test room elements for a collision with the elf
	   if (room_test_element(roomElements[i], poofy_get_x(), poofy_get_y())) roomElements[i] = poofy_hit(roomElements[i]);
	  
	   //determine the type of element and handle the behaviors
	   if (roomElements[i].type <= 49) roomElements[i] = snake_move(roomElements[i]);	 
	}
  }
}

//handles updating the room element
void room_update_element (RoomElement element)
{
  roomElements[element.id] = element;
}

//handles retrieving the room element
RoomElement room_get_element (char id)
{
  return roomElements[id];
}

//test for element collision using bounding box
bool room_test_element(RoomElement element, char x, char y)
{
	if ((element.y + 8) < y) return false;
	if (element.y > (y + 8)) return false;
	if ((element.x + 8) < x) return false;
	if (element.x > (x + 8)) return false;
	return true;	
}
