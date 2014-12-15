#include <pgm/space.h>
#include "Micromenu.h"
#include "U8glib.h"

//用户自定义部分------------------------
//nRF------------------------
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

//RTC------------------------
#include <Rtc_Pcf8563.h>
#include <Wire.h>

//EEPROM---------------------
#include <EEPROM.h>

//输入设备-------------------
//#include <Joypad.h>
//用户自定义部分------------------------

#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //RX,TX

String comdata = "";  //显示的字符串

int pinRx = 4;
int pinTx = 5;

void setup() 
{
  Serial.begin(9600);
  mySerial.begin(9600); 
  pinMode(pinTx,OUTPUT);
  pinMode(pinRx,INPUT_PULLUP);

  // u8g.setRot180();  // rotate screen, if required
  osd_smart_init();    //界面条件初始化

  //用户自定义部分------------------------
  //EEPROM---------------------
  eeprom_READ();

  set_alarm(alarm_hour,alarm_minute);  

  delay(500);
  audio_reset();
  audio_device(1);
  audio_vol(alarm_vol);
  audio_mode(1);

  pinMode(3,INPUT_PULLUP);
  pinMode(4,INPUT_PULLUP);
  pinMode(5,INPUT_PULLUP);
  pinMode(6,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
  //用户自定义部分------------------------
}


void loop() 
{  
  while (mySerial.available() > 0)  //判断串口是否有输入
  {
    comdata += char(mySerial.read()); //读取字符
    delay(2);                         //等待串口缓存
  }
  if(comdata=="play")
    audio_play();
  else if(comdata=="stop")
    audio_pause();
  else if(comdata=="up")
    audio_up();
  else if(comdata=="down")
    audio_down();
  else if(comdata=="vol+")
    audio_vol_up();
  else if(comdata=="vol-")
    audio_vol_down();

  comdata = "";

  uiStep();	//检测输入动作

  updateMenu(type);	//根据输入动作更新菜单项

  if(!menu_level)	//运行主界面
  {
    if(millis() < time_drawMain) time_drawMain=millis();
    if(millis() - time_drawMain > init_drawMain)
    {
      osd_smart_run(false); 

      time_drawMain=millis();
    }
  }
  else if(menu_redraw_required)		//根据输入动作运行菜单
  {
    osd_smart_begin();
    osd_smart_run(true);
    menu_redraw_required = false;
  }



  //用户自定义部分------------------------
  //仅在主界时运行===
  if(!menu_level)  
  {

  }

  //除配置模式，任何时候都运行===
  if(!sta)
  {
    //RTC------------------------
    if(millis()<time_rtc) time_rtc=millis();
    if(millis()-time_rtc>interval_rtc)
    {
      getRTC();
      getDateStamp(getTimeStamp(Hour,Minute,Second,Day,Month,Year));

      time_rtc=millis();
    }

    if (alarm_flag==1)
    {
      delay(1000);
      set_alarm(alarm_hour,alarm_minute);

      if(alarm_switch)
      {
        do_alarm(alarm_tone);

        alarm_sta=true;
      }
    }

    if(alarm_sta)
    {
      if(menu_level)
      {
        end_alarm(alarm_tone);

        alarm_sta=false;
      }
    }

  }

  if(test_audio)
  {
    if(num!=num_cache)
    {
      test_alarm(num);
    }
    num_cache=num;
  }

  if(test_audio_vol)
  {
    if(num!=num_cache)
    {
      test_alarm_vol(num);
    }
    num_cache=num;
  }
  //用户自定义部分------------------------
}



