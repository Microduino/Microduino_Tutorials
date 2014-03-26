#include "U8glib.h"
#include "math.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

int x,y,time,i;

void setup(void) 
{

}

void draw(void) 
{
  //================Microduino====================//
  u8g.setFont(u8g_font_fixed_v0r);
  u8g.drawStr(0, 36,"Microduino");
  u8g.drawLine(0,24,60,24);
  u8g.drawLine(0,40,60,40);
  u8g.drawLine(0,0,128,0);
  u8g.drawLine(0,63,128,63);
  //================ 图形===================//
  u8g.drawDisc(8+i,10,8);//(圆实(x,y,Diameter))
  u8g.drawCircle(26+i,10,8);//(圆虚(x,y,Diameter))
  u8g.drawFrame(18+i,46,16,16);//(长方形虚（x,y,long,wide）)
  u8g.drawBox(i,46,16,16);//(长方形实（x,y,long,wide）)

  //================指针表====================//
  u8g.drawRFrame(68,2,60,60,18);//(长方形虚（x,y,long,wide,顶角弯曲程度）)  
  u8g.drawLine(98, 2, 98, 6);//12点
  u8g.drawLine(124, 30, 128,30);//3点
  u8g.drawLine(98, 57, 98,61);//6点
  u8g.drawLine(68, 30, 72,30);//9点 
  u8g.drawLine(98, 30, 98,30);
//================秒针====================//
  x=99+16*sin(3.14*6*time/180);
  y=30-16*cos(3.14*6*time/180);
  u8g.drawLine(99,30, x, y);
}
void loop(void) {
  time=millis()/1000;
  if((millis()-time*1000)<300)
  i+=1;
  if(i>26)
  i=26;
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}



