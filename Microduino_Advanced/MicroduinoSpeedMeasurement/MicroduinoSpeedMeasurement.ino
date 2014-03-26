#include "U8glib.h"


int potPin = A0;//电位器引脚定义
int motorPin = 9;//电机引脚定义
int potValue = 0;//电位器的数值
int motorValue = 0;//电机的数值

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

//显示速度
void draw(int speedNum) {

  u8g.setFont(u8g_font_unifont);//字体1
  u8g.drawStr( 0, 16, "Speed:");
  u8g.setFont(u8g_font_7x13);//字体2
  u8g.setPrintPos(0, 32);
  u8g.print(speedNum);
  u8g.print(" (rpm)");
}  


void setup()
{
}


void loop()
{
  //读取电位器的值
  potValue = analogRead(potPin);  
  //映射为电机的速率值
  motorValue = map(potValue, 0, 1023, 0, 200);
  analogWrite(motorPin, motorValue);

  u8g.firstPage();  
  do {
    draw(motorValue);
  } 
  while( u8g.nextPage() ); 
  delay(2);
}

