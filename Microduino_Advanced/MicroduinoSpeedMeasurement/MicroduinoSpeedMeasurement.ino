#include "U8glib.h"


int potPin = A0;//Potentiometer pin definitions
int motorPin = 9;//Motor pin definitions
int potValue = 0;//The value of the potentiometer
int motorValue = 0;//Value of the motor

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//The definition OLED connection

//show speed
void draw(int speedNum) {

  u8g.setFont(u8g_font_unifont);//font1
  u8g.drawStr( 0, 16, "Speed:");
  u8g.setFont(u8g_font_7x13);//font2
  u8g.setPrintPos(0, 32);
  u8g.print(speedNum);
  u8g.print(" (rpm)");
}  


void setup()
{
}


void loop()
{
  //Reading the value of the potentiometer
  potValue = analogRead(potPin);  
  //Mapped to the value of the motor speed
  motorValue = map(potValue, 0, 1023, 0, 200);
  analogWrite(motorPin, motorValue);

  u8g.firstPage();  
  do {
    draw(motorValue);
  } 
  while( u8g.nextPage() ); 
  delay(2);
}

