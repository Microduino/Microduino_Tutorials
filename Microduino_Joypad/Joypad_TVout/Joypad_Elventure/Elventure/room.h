#ifndef __ROOM__
#define __ROOM__

#define SIZEOF_ELEMENT_RECORD 4
#define MAX_ELEMENT_RECORDS 4

#define ITEM_SWORD   50
#define ITEM_HEART   52
#define ITEM_CRYSTAL 54
#define ITEM_PORTAL  56
#define ITEM_ORB     58
#define ITEM_ARMOR   60
#define ITEM_STAFF   62

#define STATE_HIDDEN     0
#define STATE_VISIBLE    1
#define STATE_MOVE_UP    2
#define STATE_MOVE_DOWN  3
#define STATE_MOVE_LEFT  4
#define STATE_MOVE_RIGHT 5

#define COUNTER_START    20

struct RoomElement
{
  unsigned char id;          //elment identifier
  unsigned char type;        //element type
  unsigned char x;           // y position
  unsigned char y;           // x position
  unsigned char state;       //determines the state of the the element
  unsigned char step;        //used to maintain animation step
  unsigned char counter;     //used for timed events
};

void loadRoomElemments(char room);
void handleRoomElements();
void updateRoomElement (RoomElement element);
RoomElement getRoomElement(char id);
bool testRoomElement (RoomElement element, char testX, char testY, char ySize);
void addRoomElement(char type, char x, char y, char state, char counter);

#endif __ROOM__
