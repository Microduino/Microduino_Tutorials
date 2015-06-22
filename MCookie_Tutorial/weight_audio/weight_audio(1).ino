#include "HX711.h"
#include "U8glib.h"
#include "def.h"
#include "key.h"
#include "Audio.h"
#include "OLED.h"
#include <SoftwareSerial.h>

int music_vol=26;  //初始音量0~30

int i,j;

void setup()
{
  Init_Hx711();

  Serial.begin(9600);
  pinMode(shock_pin,OUTPUT);
  pinMode(key_pin,INPUT_PULLUP);
  analogWrite(shock_pin,0);
  //Serial.println("^-^ Hello! Microduino ^-^");

  pinMode(2,INPUT_PULLUP); 
  //attachInterrupt(0, boy_girl, FALLING);

  delay(500);
  Get_Maopi();		//获取毛皮

  oled_init();
  delay(3000);

  key_init();

  audio_init(DEVICE_Flash,MODE_One_END,music_vol);//初始化mp3模块
}

void loop()
{
  Weight = Get_Weight()/1000.0;	//计算放在传感器上的重物重量
  Serial.println(Weight);

  if(Weight<0)
    Weight=0;
  Weight_cg=Weight;

  if(Weight>=25)
  {
    //Serial.print("We are Writing->>>>>>>");
    delay(1000);
    Weight = Get_Weight()/1000;
    if(abs(Weight-Weight_cg)<5)
      i=1; 
  }
  else
  {
    i=0;
    j=0;
  }
  if(i>j)
  {
    j=i;
    /*
    if(!sta)
     {
     //Serial.print("We are the Girl->>>>>>>");
     if(Weight>=25&&Weight<=34.5)
     audio_choose(2);
     else if(Weight>=35&&Weight<=39.5)
     audio_choose(3);
     else if(Weight>=40&&Weight<=44.5)
     audio_choose(4);
     else if(Weight>=45&&Weight<=52)
     audio_choose(5);
     else if(Weight>=52.5&&Weight<=57)
     audio_choose(6);
     else if(Weight>=57.5&&Weight<=62)
     audio_choose(7);
     else if(Weight>=62.5)
     audio_choose(8);
     }
     else
     {
     */
    //Serial.println("We are the Boy->>>>>>>");
    if(Weight>=25&&Weight<40)
      j=0;
    else if(Weight>=40&&Weight<=45)
      audio_choose(1);
    else if(Weight>=45.5&&Weight<=57.5)
      audio_choose(2);
    else if(Weight>=58&&Weight<=62.5)
      audio_choose(3);
    else if(Weight>=63&&Weight<=67.5)
      audio_choose(4);
    else if(Weight>=68&&Weight<=77.5)
      audio_choose(5);
    else if(Weight>=78&&Weight<90)
      audio_choose(6);
    else if(Weight>=90)
      audio_choose(7);
    //    }
    //audio_play();
  }
  oled();
}



