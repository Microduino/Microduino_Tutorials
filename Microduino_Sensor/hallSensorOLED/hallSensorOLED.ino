#include "U8glib.h"
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

int hallPin=13;

String pole="South";

int statePin=LOW;

//show pole
void draw() {
  u8g.setFont(u8g_font_courB18);//font
  u8g.setPrintPos(0, 16);
  u8g.print("pole:");
  u8g.setPrintPos(0, 50);
  u8g.print(pole);

} 

void setup()
{
  pinMode(hallPin,INPUT);
  Serial.begin(9600);
}

void loop()
{
  statePin=digitalRead(hallPin);

  if( statePin==HIGH)
  {
    Serial.println("North"); 
    pole="North";
  }
  else if(statePin==LOW)
  {
    Serial.println("South"); 
    pole="South";
  }

  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);
}



