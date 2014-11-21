#include <avr/pgmspace.h>
#include <TVout.h>
#include "bitmap_funcs.h"
extern TVout TV;

void overlaybitmap(uint8_t x, uint8_t y, const unsigned char * bmp ) {

	uint8_t temp, lshift, rshift, save, xtra;
	uint16_t si = 0;
	uint16_t i;
        uint8_t width;
        uint8_t lines;
        
	rshift = x&7;
	lshift = 8-rshift;
	width = pgm_read_byte((uint32_t)(bmp) + i);
	i++;
	lines = pgm_read_byte((uint32_t)(bmp) + i);
	i++;
	
	if (width&7) {
	  xtra = width&7;
	  width = width/8;
	  width++;
	}
	else {
	  xtra = 8;
	  width = width/8;
	}
	
	for (uint8_t l = 0; l < lines; l++) {
	  si = (y + l)*display.hres + x/8;
	  save = display.screen[si];
	  temp = pgm_read_byte((uint32_t)(bmp) + i++);
	  display.screen[si++] |= temp >> rshift;
	  for ( uint16_t b = i + width-1; i < b; i++) {
	    save = display.screen[si];
	    display.screen[si] |= temp << lshift;
	    temp = pgm_read_byte((uint32_t)(bmp) + i);
	    display.screen[si++] |= temp >> rshift;
	  }
	  display.screen[si] |= temp << lshift;
	}
} // end of bitmap

void erasebitmap(uint8_t x, uint8_t y, const unsigned char * bmp )
{
	uint8_t temp, lshift, rshift, save, xtra;
	uint16_t si = 0;
	uint16_t i = 0;
        uint8_t width;
        uint8_t lines;
        
	rshift = x&7;
	lshift = 8-rshift;
	width = pgm_read_byte((uint32_t)(bmp) + i);
	i++;
	lines = pgm_read_byte((uint32_t)(bmp) + i);
	i++;
	if (width&7) {
		xtra = width&7;
		width = width/8;
		width++;
	}
	else {
		xtra = 8;
		width = width/8;
	}
	
	for (uint8_t l = 0; l < lines; l++) {
	  si = (y + l)*display.hres + x/8;
		save = display.screen[si];
		temp = pgm_read_byte((uint32_t)(bmp) + i++);
		display.screen[si++] &= ~(temp >> rshift);
		for ( uint16_t b = i + width-1; i < b; i++) {
			save = display.screen[si];
			display.screen[si] &= ~(temp << lshift);
			temp = pgm_read_byte((uint32_t)(bmp) + i);
			display.screen[si++] &= ~(temp >> rshift);
		}
		if (rshift + xtra - 8 > 0)
{
		  display.screen[si] &= ~(temp << lshift);
}
	}
} // end of bitmap

// erases a rectangle on the screen the size of the passed bitmap
void eraseBitmapRect( uint8_t x, uint8_t y, const unsigned char * bmp )
{
  uint8_t w, h;
  
  // extract width and height from bitmap
  w = pgm_read_byte((uint32_t)(bmp));
  h = pgm_read_byte((uint32_t)(bmp) + 1);
  
  TV.draw_rect( x, y, w, h, BLACK, BLACK ); 
}

