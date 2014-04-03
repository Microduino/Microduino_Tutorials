#include "U8glib.h"

#include <Wire.h>
#include <AM2321.h>
AM2321 am2321;

float temperature=0.0;
float humidity=0.0;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//show speed
void draw() {
  u8g.setFont(u8g_font_courB14);//font
  u8g.setPrintPos(0, 20);
  u8g.print("Temp:");
  u8g.print(temperature);
  u8g.print("`c");
  u8g.setPrintPos(0, 54);
  u8g.print("AH:");
  u8g.print(humidity);
  u8g.print("%");
}  


void setup()
{
}
 
 
void loop()
{

  am2321.read();
  
  temperature=(float)am2321.temperature/10.0;
  humidity=(float)am2321.humidity/10.0;
  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);
}

