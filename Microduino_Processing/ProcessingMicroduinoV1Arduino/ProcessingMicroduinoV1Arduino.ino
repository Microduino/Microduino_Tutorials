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
  if(button_down>500&&button_down<600)
  key_down=1;
  else if(button_down>600&&button_down<700)
  key_down=2;
  else
  key_down=0;
  if(button<40)
    key_up=2;
  else if(button>50&&button<100)
    key_up=4;
  else if(button>138&&button<141)
    key_up=1;
  else if(button>358&&button<361)
    key_up=3;
  else if(button>228&&button<231)
    key_up=5;
  else
    key_up=0;
  Serial.print(key_up);
  Serial.println(key_down);
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



