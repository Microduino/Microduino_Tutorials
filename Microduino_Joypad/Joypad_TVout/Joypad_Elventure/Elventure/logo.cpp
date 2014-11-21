#include <avr/pgmspace.h>
#include <TVout.h>
#include "logo_bitmap.h"
extern TVout TV;

#define SIZEOF_LOGO_RECORD 10

void drawLogo(char start)
{
  //draw hearts
  char x_offset = 48;
  for (char i=start; i < (start+4); i++)
  {
      //draw logo element
      TV.bitmap(x_offset, 16, logo_bitmap + (SIZEOF_LOGO_RECORD * i));
   
    x_offset += 8;
  }   
}

