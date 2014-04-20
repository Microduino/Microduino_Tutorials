#ifndef _RGBLEDMatrix_H
#define _RGBLEDMatrix_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif
#include <avr/interrupt.h>   
#include <avr/io.h>

#define __spi_clock 13   // SCK - hardware SPI
#define __spi_latch 10
#define __spi_data 11    // MOSI - hardware SPI
#define __spi_data_in 12 // MISO - hardware SPI (unused)
#define __display_enable 9
#define __rows 8
#define __max_row __rows-1
#define __leds_per_row 8
#define __max_led __leds_per_row-1
#define __brightness_levels 15 // 0...15 above 28 is bad for ISR ( move to timer1, lower irq freq ! )
#define __max_brightness __brightness_levels-1
#define __fade_delay 8

#define INIT_TIMER_COUNT 0
#define RESET_TIMER2 TCNT2 = INIT_TIMER_COUNT

class RGBLEDMatrix
{
public:
	RGBLEDMatrix();

  static byte spi_transfer(byte data);
  static void RGBLEDMatrix_ISR();

  virtual void init();
  virtual void clear();
	virtual void set_led_red(byte row, byte led, byte red);
	virtual void set_led_green(byte row, byte led, byte green);
	virtual void set_led_blue(byte row, byte led, byte blue);
	virtual void set_led_rgb(byte row, byte led, byte red, byte green, byte blue);
	virtual void set_led_hue(byte row, byte led, int hue);
	virtual void set_matrix_rgb(byte red, byte green, byte blue);
	virtual void set_matrix_hue(int hue);
	virtual void set_row_rgb(byte row, byte red, byte green, byte blue);
	virtual void set_column_rgb(byte column, byte red, byte green, byte blue);
	virtual void set_row_hue(byte row, int hue);
	virtual void set_column_hue(byte column, int hue);
	virtual void set_row_byte_hue(byte row, byte data_byte, int hue);
	virtual void fader(void);
	virtual void fader_hue(void);
	virtual void matrix_heart(int hue);
	virtual void matrix_image(int x, byte data, int hue);
	virtual void matrix_test(void);
	virtual void matrix_heart_2(void);
	virtual void rainbow(void);
	virtual void color_wave(int width);
	virtual void random_leds(void);
	virtual void smile_on(int hue);
	virtual void smile_off(int hue);
	virtual void smile_blink(int hue, byte times, int pause);
	virtual void explode(int hue, byte pause);

};
#endif //_RGBLEDMatrix_H
