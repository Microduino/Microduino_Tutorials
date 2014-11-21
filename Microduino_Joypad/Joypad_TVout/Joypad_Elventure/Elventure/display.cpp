#include <avr/pgmspace.h>
#include <TVout.h>
#include "display_bitmap.h"
#include "elf.h"
#include "display.h"
#include "room.h"

extern TVout TV;

#define SIZEOF_DISPLAY_RECORD 6

void updateDisplay(Elf elf)
{
  //draw hearts
  char x_offset = 98;
  for (char i=1; i <= MAX_HEARTS; i++)
  {
    if (i <= elf.hearts)
    {
      //draw hearts
      TV.bitmap(x_offset, 4, display_bitmap + SIZEOF_DISPLAY_RECORD);
    } else {
      //draw blanks
      TV.bitmap(x_offset, 4, display_bitmap);
    }	
    x_offset += 5;
  }
   
  //draw crystals (inventory)
  x_offset = 98;
  for (char j=0; j < MAX_ITEMS; j++)
  {
    switch (elf.items[j])
    {
	  case ITEM_CRYSTAL:
        //draw crystals
        TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 2 ));
		break;
		
	  case ITEM_ARMOR:
        //draw crystals
        TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 3 ));
		break;

	  case ITEM_ORB:
        //draw crystals
        TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 4 ));
		break;

	  case ITEM_STAFF:
        //draw crystals
        TV.bitmap(x_offset, 12, display_bitmap + (SIZEOF_DISPLAY_RECORD * 5 ));
		break;
		
      default:
        //draw blanks
        TV.bitmap(x_offset, 12, display_bitmap);
        break;
    }	
    x_offset += 5;
  }   
}

