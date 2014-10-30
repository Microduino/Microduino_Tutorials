#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

int num=0,key_up,key_down;
long timer;

void setup() {
  Serial.begin(9600);
}

void draw(void) 
{
  u8g.setFont(u8g_font_7x14);
  u8g.setPrintPos(num, 64); 
  u8g.print("Welcome! www.microduino.cc");
  
  if(key_down==1)
  u8g.drawBox(0,0,26,15);
  if(key_down==2)
  u8g.drawBox(100,0,26,15);
  
  if(key_up==1)
    u8g.drawBox(55,0,16,16);
  if(key_up==3)
    u8g.drawBox(55,18,16,16);
  if(key_up==2)
    u8g.drawBox(37,18,16,16);
  if(key_up==4)
    u8g.drawBox(73,18,16,16);
  if(key_up==5)
    u8g.drawBox(55,36,16,16);

  u8g.drawFrame(0,0,26,15);
  u8g.drawFrame(100,0,26,15);
  u8g.drawFrame(55,0,16,16);
  u8g.drawFrame(55,18,16,16);
  u8g.drawFrame(37,18,16,16);
  u8g.drawFrame(73,18,16,16);
  u8g.drawFrame(55,36,16,16);

}

void loop() {
  int button = analogRead(A7);
  int button_down = analogRead(A6);
  if(button_down>300&&button_down<600)
  key_down=1;
  else if(button_down>200&&button_down<300)
  key_down=2;
  else
  key_down=0;
  
  if(button>50&&button<100)
    key_up=4;
  else if(button>0&&button<50) 
    key_up=2;
  else if(button>100&&button<155) 
    key_up=1;
  else if(button>200&&button<280) 
    key_up=5;
  else if(button>340&&button<420) 
    key_up=3;
  else
    key_up=0;   
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







