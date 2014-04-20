#include "RGBLEDMatrix.h"

byte brightness_red[__leds_per_row][__rows]; 
byte brightness_green[__leds_per_row][__rows];
byte brightness_blue[__leds_per_row][__rows];


ISR(TIMER2_OVF_vect) {
  RGBLEDMatrix::RGBLEDMatrix_ISR();
}
RGBLEDMatrix::RGBLEDMatrix()
{}

void RGBLEDMatrix::init()
{
  randomSeed(555);
  byte ctr1;
  byte ctr2;

  pinMode(__spi_clock,OUTPUT);
  pinMode(__spi_latch,OUTPUT);
  pinMode(__spi_data,OUTPUT);
  pinMode(__spi_data_in,INPUT);
  pinMode(__display_enable,OUTPUT);
  digitalWrite(__spi_latch,LOW);
  digitalWrite(__spi_data,LOW);
  digitalWrite(__spi_clock,LOW);

  byte clr;
  SPCR |= ( (1<<SPE) | (1<<MSTR) ); // enable SPI as master
  //SPCR |= ( (1<<SPR1) | (1<<SPR0) ); // set prescaler bits
  SPCR &= ~( (1<<SPR1) | (1<<SPR0) ); // clear prescaler bits
  clr=SPSR; // clear SPI status reg
  clr=SPDR; // clear SPI data reg
  SPSR |= (1<<SPI2X); // set prescaler bits
  //SPSR &= ~(1<<SPI2X); // clear prescaler bits

  delay(10);

  clear();
  digitalWrite(__display_enable,LOW);

  // set irq to 61 Hz: CS22-bit = 1, CS21-bit = 1, CS20-bit = 1
  TCCR2B |= ( (1<<CS22) | (1<<CS21) | (1<<CS20));      
  // Use normal mode  
  TCCR2A &= ~( (1<<WGM21) | (1<<WGM20) );
  TCCR2B &= ~( (1<<WGM22) );  
  //Timer2 Overflow Interrupt Enable  
  TIMSK2 |= (1<<TOIE2);
  TIMSK2 &= ~( (1<<OCIE2A) | (1<<OCIE2B) );
  RESET_TIMER2;
  // enable all interrupts
  sei(); 
}

void RGBLEDMatrix::RGBLEDMatrix_ISR()
{
  RESET_TIMER2; // precharge TIMER2 to maximize ISR time --> max led brightness
  byte cycle;
  for(cycle = 0; cycle < __max_brightness; cycle++) {
    byte led;
    byte row = B00000000;    // row: current source. on when (1)
    byte red;    // current sinker when on (0)
    byte green;  // current sinker when on (0)
    byte blue;   // current sinker when on (0)
    for(row = 0; row <= __max_row; row++) {
      red = B11111111;    // off
      green = B11111111;  // off
      blue = B11111111;   // off
      for(led = 0; led <= __max_led; led++) {
        if(cycle < brightness_red[row][led]) {
          red &= ~(1<<led);
        }
        if(cycle < brightness_green[row][led]) {
          green &= ~(1<<led);
        }
        if(cycle < brightness_blue[row][led]) {
          blue &= ~(1<<led);
        }
      }
      digitalWrite(__spi_latch,LOW);
      spi_transfer(red);
      spi_transfer(green);
      spi_transfer(blue);
      spi_transfer(B00000001<<row);
      digitalWrite(__spi_latch,HIGH);
      digitalWrite(__spi_latch,LOW);
    }
  }
  // turn off all leds when ISR is not running
  // otherwise leds will flash to full brightness when 1111 is set, which
  // stays on outside the ISR !
  digitalWrite(__spi_latch,LOW);
  spi_transfer(B11111111); // blue off
  spi_transfer(B11111111); // green off
  spi_transfer(B11111111); // red off
  spi_transfer(B00000000); // rows off
  digitalWrite(__spi_latch,HIGH);
  digitalWrite(__spi_latch,LOW);
}

byte RGBLEDMatrix::spi_transfer(byte data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte, we don't need that
}
void RGBLEDMatrix::clear()
{
  set_matrix_rgb(0,0,0);
}

void RGBLEDMatrix::set_led_red(byte row, byte led, byte red) {
  brightness_red[row][led] = red;
}

void RGBLEDMatrix::set_led_green(byte row, byte led, byte green) {
  brightness_green[row][led] = green;
}

void RGBLEDMatrix::set_led_blue(byte row, byte led, byte blue) {
  brightness_blue[row][led] = blue;
}

void RGBLEDMatrix::set_led_rgb(byte row, byte led, byte red, byte green, byte blue) {
  set_led_red(row,led,red);
  set_led_green(row,led,green);
  set_led_blue(row,led,blue);
}

void RGBLEDMatrix::set_led_hue(byte row, byte led, int hue) {

  // see wikipeda: HSV
  float S=100.0,V=100.0,s=S/100.0,v=V/100.0,h_i,f,p,q,t,R,G,B;

    hue = hue%360;
    h_i = hue/60;            
    f = (float)(hue)/60.0 - h_i;
    p = v*(1-s);
    q = v*(1-s*f);
    t = v*(1-s*(1-f));

    if      ( h_i == 0 ) { 
      R = v; 
      G = t; 
      B = p;
    }
    else if ( h_i == 1 ) { 
      R = q; 
      G = v; 
      B = p;
    }
    else if ( h_i == 2 ) { 
      R = p; 
      G = v; 
      B = t;
    }
    else if ( h_i == 3 ) { 
      R = p; 
      G = q; 
      B = v;
    }
    else if ( h_i == 4 ) { 
      R = t; 
      G = p; 
      B = v;
    }
    else                   { 
      R = v; 
      G = p; 
      B = q;
    }

    set_led_rgb(row,led,byte(R*(float)(__max_brightness)),byte(G*(float)(__max_brightness)),byte(B*(float)(__max_brightness)));
}

void RGBLEDMatrix::set_matrix_rgb(byte red, byte green, byte blue) {
  byte ctr1;
  byte ctr2;
  for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
    for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_rgb(ctr2,ctr1,red,green,blue);
    }
  }
}

void RGBLEDMatrix::set_matrix_hue(int hue) {
  byte ctr1;
  byte ctr2;
  for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
    for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_hue(ctr2,ctr1,hue);
    }
  }
}

void RGBLEDMatrix::set_row_rgb(byte row, byte red, byte green, byte blue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_rgb(row,ctr1,red,green,blue);
  }
}

void RGBLEDMatrix::set_column_rgb(byte column, byte red, byte green, byte blue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_row; ctr1++) {
      set_led_rgb(ctr1,column,red,green,blue);
  }
}

void RGBLEDMatrix::set_row_hue(byte row, int hue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
      set_led_hue(row,ctr1,hue);
  }
}

void RGBLEDMatrix::set_column_hue(byte column, int hue) {
  byte ctr1;
  for(ctr1 = 0; ctr1 <= __max_row; ctr1++) {
      set_led_hue(ctr1,column,hue);
  }
}

void RGBLEDMatrix::set_row_byte_hue(byte row, byte data_byte, int hue) {
  byte led;
  for(led = 0; led <= __max_led; led++) {
    if( (data_byte>>led)&(B00000001) ) {
      set_led_hue(row,led,hue);
    }
    else {
      set_led_rgb(row,led,0,0,0);
    }
  }
}

void RGBLEDMatrix::fader(void) {
  byte ctr1;
  byte row;
  byte led;

  for(ctr1 = 0; ctr1 <= __max_brightness; ctr1++) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }
    delay(__fade_delay);
  }
  for(ctr1 = __max_brightness; (ctr1 >= 0) & (ctr1 != 255); ctr1--) {
    for(row = 0; row <= __max_row; row++) {
      for(led = 0; led <= __max_led; led++) {
        set_led_rgb(row,led,ctr1,ctr1,ctr1);
      }
    }
    delay(__fade_delay);
  }
}

void RGBLEDMatrix::fader_hue(void) {
  int ctr1;
  byte row;
  byte led;

  for(ctr1 = 0; ctr1 < 360; ctr1=ctr1+3) {
    set_matrix_hue((float)(ctr1));
    delay(__fade_delay);
  }
}

void RGBLEDMatrix::matrix_heart(int hue) {
  set_row_byte_hue(1,B00110110,hue);
  set_row_byte_hue(2,B01111111,hue);
  set_row_byte_hue(3,B01111111,hue);
  set_row_byte_hue(4,B00111110,hue);
  set_row_byte_hue(5,B00011100,hue);
  set_row_byte_hue(6,B00001000,hue);
}

void RGBLEDMatrix::matrix_image(int x, byte data, int hue) {
      set_row_byte_hue(x,data,hue);
}


void RGBLEDMatrix::matrix_test(void) {
  byte ctr1;
  byte ctr2;
  int hue;

  for(hue = 0; hue < 360; hue=hue+32) {
    for(ctr2 = 0; ctr2 <= __max_row; ctr2++) {
      for(ctr1 = 0; ctr1 <= __max_led; ctr1++) {
        set_led_hue(ctr2,ctr1,hue);
        delay(5);
      }
    }
  }
}

void RGBLEDMatrix::matrix_heart_2(void) {
  int hue;
  for(hue = 0; hue < 360; hue=hue+16) {
    set_row_byte_hue(1,B00110110,hue);
    set_row_byte_hue(2,B01111111,hue);
    set_row_byte_hue(3,B01111111,hue);
    set_row_byte_hue(4,B00111110,hue);
    set_row_byte_hue(5,B00011100,hue);
    set_row_byte_hue(6,B00001000,hue);
    delay(3*__fade_delay);  
  }
}

void RGBLEDMatrix::rainbow(void) {
  byte column;
  for(column = 0; column <= __max_led; column++) {
    set_column_hue(column,column*50);
  } 
}

void RGBLEDMatrix::color_wave(int width) {
  byte column;
  static int shift = 0;
  for(column = 0; column <= __max_led; column++) {
    set_column_hue(column,column*width+shift);
  } 
  shift++;
}

void RGBLEDMatrix::random_leds(void) {
  set_led_hue((byte)(random(__rows)),(byte)(random(__leds_per_row)),(int)(random(360)));
}

void RGBLEDMatrix::smile_on(int hue) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B01100110,hue);
  set_row_byte_hue(2,B01100110,hue);
  set_row_byte_hue(3,B00000000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B10011001,hue);
  set_row_byte_hue(6,B01000010,hue);
  set_row_byte_hue(7,B00111100,hue);
}

void RGBLEDMatrix::smile_off(int hue) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B01100110,hue);
  set_row_byte_hue(3,B00000000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B10011001,hue);
  set_row_byte_hue(6,B01000010,hue);
  set_row_byte_hue(7,B00111100,hue);
}

void RGBLEDMatrix::smile_blink(int hue, byte times, int pause) {
 byte ctr;
 for(ctr = 0; ctr < times; ctr++) {
   delay(pause);
   smile_on(hue);
   delay(pause);
   smile_off(hue);
   delay(pause);
   smile_on(hue);
 }
}

void RGBLEDMatrix::explode(int hue, byte pause) {
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B00000000,hue);
  set_row_byte_hue(3,B00011000,hue);
  set_row_byte_hue(4,B00011000,hue);
  set_row_byte_hue(5,B00000000,hue);
  set_row_byte_hue(6,B00000000,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B00000000,hue);
  set_row_byte_hue(2,B00111100,hue);
  set_row_byte_hue(3,B00100100,hue);
  set_row_byte_hue(4,B00100100,hue);
  set_row_byte_hue(5,B00111100,hue);
  set_row_byte_hue(6,B00000000,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B00000000,hue);
  set_row_byte_hue(1,B01111110,hue);
  set_row_byte_hue(2,B01000010,hue);
  set_row_byte_hue(3,B01000010,hue);
  set_row_byte_hue(4,B01000010,hue);
  set_row_byte_hue(5,B01000010,hue);
  set_row_byte_hue(6,B01111110,hue);
  set_row_byte_hue(7,B00000000,hue);
  delay(pause);
  set_row_byte_hue(0,B11111111,hue);
  set_row_byte_hue(1,B10000001,hue);
  set_row_byte_hue(2,B10000001,hue);
  set_row_byte_hue(3,B10000001,hue);
  set_row_byte_hue(4,B10000001,hue);
  set_row_byte_hue(5,B10000001,hue);
  set_row_byte_hue(6,B10000001,hue);
  set_row_byte_hue(7,B11111111,hue);
  delay(pause);
  set_matrix_rgb(0,0,0);
}