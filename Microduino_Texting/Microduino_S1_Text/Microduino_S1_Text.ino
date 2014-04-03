#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

int data=10;
int num=0;
long timer;

void setup() {
  Serial.begin(9600);
}

void draw(void) 
{
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(num, 64); 
  u8g.print("Welcome! www.microduino.cc");
  u8g.setFont(u8g_font_6x12);
  u8g.setPrintPos(0, 32); 
  switch (data) {
  case 0:
    u8g.print("No button is pressed ");
    break;
  case 1:
    u8g.print("Press the rignt button");
    break;
  case 2:
    u8g.print("Press the left button");
    break;
  }
}
void loop() {
  int sensorValue = analogRead(A6);
  if(sensorValue<100)
    data=1;
  else if(sensorValue>100&&sensorValue<300)
    data=2;
  else
    data=0;
  Serial.println(data);
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








