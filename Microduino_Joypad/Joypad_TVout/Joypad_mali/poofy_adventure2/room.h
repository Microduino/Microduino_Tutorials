#ifndef __ROOM__
#define __ROOM__

#define SIZEOF_ELEMENT_RECORD 4
#define MAX_ELEMENT_RECORDS 4

#define ITEM_APPLE       50
#define ITEM_STRAWBERRY  52
#define ITEM_CHERRY      54

#define STATE_HIDDEN     0
#define STATE_VISIBLE    1
#define STATE_MOVE_LEFT  2
#define STATE_MOVE_RIGHT 3
#define STATE_FALL_LEFT  4
#define STATE_FALL_RIGHT 5

struct RoomElement
{
  unsigned char id;          //elment identifier
  unsigned char type;        //element type
  unsigned char x;           // y position
  unsigned char y;           // x position
  unsigned char state;       //determines the state of the the element
  unsigned char step;        //used to maintain animation step
};

void room_add_element(char type, char x, char y, char state);
void room_load_elements(char room);
void room_handle_elements();
void room_update_element (RoomElement element);
RoomElement room_get_element (char id);
bool room_test_element(RoomElement element, char x, char y);
#endif __ROOM__
