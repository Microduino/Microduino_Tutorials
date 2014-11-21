#include <avr/pgmspace.h>
#include <TVout.h>
#include "fruit.h"
#include "fruit_bitmap.h"
#include "bitmap_funcs.h"
#include "room.h"

#define SIZEOF_FRUIT_RECORD 10

void fruit_draw(RoomElement element)
{
   overlaybitmap(element.x, element.y, fruit_bitmap + ( (element.type-50) * SIZEOF_FRUIT_RECORD));
}

RoomElement fruit_hit(RoomElement element)
{
  //erase the old fruit image
  eraseBitmapRect(element.x, element.y, fruit_bitmap);
		 
  element.state = STATE_HIDDEN;
  return element;
}


