//bitmap_funcs.cpp from Parachute by pocketscience
//for full source of that sketch, see: http://nootropicdesign.com/hackvision/downloads/games/Parachute.zip
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

//modified shift that preserves the bottom 16 pixels of the screen
void shift_left(uint8_t distance)
{
	uint8_t * src;
	uint8_t * dst;
	uint8_t * end;
	uint8_t shift;
	uint8_t tmp;

	shift = distance & 7;
			
	for (uint8_t line = 0; line < 80; line++) {
		dst = display.screen + display.hres*line;
		src = dst + distance/8;
		end = dst + display.hres-2;
		while (src <= end) {
			tmp = 0;
			tmp = *src << shift;
			*src = 0;
			src++;
			tmp |= *src >> (8 - shift);
			*dst = tmp;
			dst++;
		}
		tmp = 0;
		tmp = *src << shift;
		*src = 0;
		*dst = tmp;
	}
} // end of shift

