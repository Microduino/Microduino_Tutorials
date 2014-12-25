#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

int num=0,button,button_down;
long timer;

void setup() {
  Serial.begin(9600);
}

void draw(void) 
{
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(num, 64); 
  u8g.print("Welcome! www.microduino.cc");
  u8g.setPrintPos(0, 16); 
  u8g.print("Key Floor:");
  u8g.print(button);
  u8g.setPrintPos(0, 32); 
  u8g.print("Key Board:");
  u8g.print(button_down);
}

void loop() {
  button = analogRead(A7);
  button_down = analogRead(A6);

  Serial.print(button);
  Serial.print("     ");
  Serial.println(button_down);
  delay(100);

  if(millis()-timer>2)
  {
    timer=millis();
    num-=4;
  }

  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}
