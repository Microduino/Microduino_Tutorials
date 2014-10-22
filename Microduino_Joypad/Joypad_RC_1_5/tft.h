#include "Arduino.h"

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

Adafruit_ST7735 tft = Adafruit_ST7735(5, 4, -1);    //cs,dc,rst

#define _Q_font_x 2
#define _Q_font_y 23

#define _Q_x 33
#define _Q_y 24
#define _W_x 93
#define _W_y 5
//======================================
void TFT_clear()
{
  tft.fillScreen(TFT_BUT);
}

void TFT_init(boolean _init)
{
  if(_init)
  {
    tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
    //  Serial.println("init");
    tft.fillScreen(TFT_BUT);
  }

  tft.fillRect(0, 0,128, 40, TFT_TOP);  
  tft.setTextColor(TFT_BUT);
  tft.setTextSize(2);
  tft.setCursor(26, 6);
  tft.print("Joypad");
  tft.setTextSize(1);
  tft.setCursor(32, 24);
  tft.print("Microduino");
  tft.fillRect(0, 40,128, 120, TFT_BUT);  
}


void TFT_begin()
{
  /*
  tft.setTextSize(2);
   tft.setTextColor(ST7735_BLUE);
   tft.setCursor(4, 20);
   tft.print("Microduino");
   */

  tft.setTextColor(TFT_TOP);
  tft.setTextSize(1);
  tft.setCursor(_Q_font_x,44);
  tft.println("Press the key1");
  tft.setCursor(_Q_font_x,44+15);
  tft.print("to enter config mode");

  tft.setTextSize(2);
  tft.setCursor(4, 150); 
  tft.setTextColor(TFT_TOP);
  for(int a=0;a<(millis()-TIME1)/(interval_TIME1/10);a++)
  {
    tft.print("-");
  }
  //  delay(50);
}



//------------------

const int _C_x_S=_Q_x+1;
const int _C_x_M=_Q_x+((_W_x+1)/2);
const int _C_x_E=_Q_x+_W_x-1;

String NAME[8]={
  "THROT","ROLL","PITCH","YAW","AUX1","AUX2","AUX3","AUX4"};

void TFT_ready()
{
  tft.setTextSize(0);

  tft.fillRect(0, 0,128, 11, TFT_TOP);  
  tft.setTextColor(TFT_BUT);
  tft.setCursor(_Q_font_x, 2);
  tft.print(mode ? "MODE: Robot" : "MODE: QuadCopter");

  tft.setTextColor(TFT_TOP);
  for(int a=0;a<8;a++)
  {
    int _a=a*15;

    tft.setCursor(_Q_font_x, _Q_font_y+_a);
    tft.print(NAME[a]);
    //------------------------------------------
    tft.drawRect(_Q_x, _Q_y+_a, _W_x, _W_y, TFT_TOP);  
  }
}


long _show_l_c;
void show(long _show_l,int _show_x,int _show_y)  //Like: "show(millis()/1000,_Q_font_x+70,15+9*15)"
{
  tft.setTextColor(TFT_BUT);
  tft.setCursor(_show_x,_show_y);
  tft.print(_show_l_c);

  tft.setTextColor(TFT_TOP);
  tft.setCursor(_show_x,_show_y);
  tft.print(_show_l);

  _show_l_c=_show_l;
}

String correct_NAME[8]={
  "X< ","X> ","Y< ","Y> " ,"X< ","X> ","Y< ","Y> "          };
void Joy_correct()
{
  //  tft.fillScreen(TFT_BUT);

  tft.setTextColor(TFT_TOP);
  tft.setTextSize(1);
  tft.setCursor(_Q_font_x,44);
  tft.print("Key3>MODE:QuadCopter");
  tft.setCursor(_Q_font_x,44+15);
  tft.print("Key4>MODE:Robot");
  tft.setCursor(_Q_font_x,44+15*3);
  tft.print("Please moves Joystick");
  tft.setCursor(_Q_font_x,44+15*4);
  tft.print("to maximum gear");

  //==============================================
  tft.setTextSize(0);

  //----------------------------------------------
  for(int a=0;a<4;a++)
  {
    int _c;
    _c=Joy_i(a,false,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);

    int _x,_y;
    switch(a)
    {
    case 0:
      _x=0;
      _y=0;
      break;
    case 1:
      _x=0;
      _y=1;
      break;
    case 2:
      _x=1;
      _y=0;
      break;
    case 3:
      _x=1;
      _y=1;
      break;
    }

    if(_c>joy_correct_max[a])
    {
      joy_correct_max[a]=_c;

      tft.fillRect(18+_x*60,  120+8+18*_y, 30, 9, TFT_BUT);
    }
    tft.setCursor(2+_x*60, 120+9+18*_y);
    tft.print(correct_NAME[a*2]);
    tft.print(joy_correct_max[a]);

    if(_c<joy_correct_min[a])
    {
      joy_correct_min[a]=_c;

      tft.fillRect(18+_x*60,  120-1+18*_y, 30, 9, TFT_BUT);
    }
    tft.setCursor(2+_x*60, 120+18*_y);
    tft.print(correct_NAME[a*2+1]);
    tft.print(joy_correct_min[a]);
  }
}


void TFT_run()
{
  //------------------
  for(int a=0;a<8;a++)
  {
    int _a=a*15;

    //------------------------------------------
    int _C_x_A0,_C_x_B0,_C_x_A,_C_x_B,_C_x_A1,_C_x_B1;
    int _C_x;

    if(outBuf[a]<Joy_MID)
    {
      _C_x=map(outBuf[a],Joy_MID-Joy_maximum,Joy_MID,_C_x_S,_C_x_M);

      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x-_C_x_S;

      _C_x_A=_C_x;
      _C_x_B=_C_x_M-_C_x;

      _C_x_A1=_C_x_M;
      _C_x_B1=_C_x_E-_C_x_M;
    }
    else if(outBuf[a]>Joy_MID)
    {
      _C_x=map(outBuf[a],Joy_MID,Joy_MID+Joy_maximum,_C_x_M,_C_x_E);

      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x_M-_C_x_S;

      _C_x_A=_C_x_M;
      _C_x_B=_C_x-_C_x_M;

      _C_x_A1=_C_x;
      _C_x_B1=_C_x_E-_C_x;
    }
    else
    {
      _C_x_A0=_C_x_S;
      _C_x_B0=_C_x_M-_C_x_S;

      _C_x_A=_C_x_M;
      _C_x_B=0;

      _C_x_A1=_C_x_M;
      _C_x_B1=_C_x_E-_C_x_M;
    }
    tft.fillRect(_C_x_A0,  _Q_y+_a+1, _C_x_B0, _W_y-2, TFT_BUT);
    tft.fillRect(_C_x_A,  _Q_y+_a+1, _C_x_B, _W_y-2, ST7735_BLUE);
    tft.fillRect(_C_x_A1,  _Q_y+_a+1, _C_x_B1, _W_y-2, TFT_BUT);

    tft.fillRect(_C_x_M,  _Q_y+_a-1, 1, _W_y+2, ST7735_RED);

    //--------------------------    
    //    tft.drawLine(80, 18, 80, 140, ST7735_RED);
  }
  tft.fillRect(0, 158, 128, 2, node_clock_error ? ST7735_RED : ST7735_GREEN);
}




