#ifndef __BITMAP_FUNCS__
#define __BITMAP_FUNCS__

void overlaybitmap(uint8_t x, uint8_t y, const unsigned char * bmp );
void erasebitmap(uint8_t x, uint8_t y, const unsigned char * bmp );
void eraseBitmapRect( uint8_t x, uint8_t y, const unsigned char * bmp );
void shift_left(uint8_t distance);
#endif __BITMAP_FUNCS__
