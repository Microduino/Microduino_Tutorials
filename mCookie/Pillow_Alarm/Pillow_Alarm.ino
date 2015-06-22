#include <Wire.h>
#include <Rtc_Pcf8563.h>

#include "audio.h"

Rtc_Pcf8563 rtc;

#include <SoftwareSerial.h>
SoftwareSerial mySerial_ble(4, 5); //RX,TX

#define my_Serial mySerial_ble

#define pin_1 A0
#define pin_2 A2
#define pin_3 A6

int music_vol=26;  //初始音量0~30

byte cmdByte;
String msg = "Microduino";
int _year,_month,_day,_hour,_minute,_sec;
int alarm_hour,alarm_minute;

int delay_minute;

String dateStr, ret;

boolean play_stop=true;
boolean play_music=true;
boolean play_lage;

#define delay_time 10

void setup()
{
  Serial.begin(9600);
  my_Serial.begin(9600);

  pinMode(pin_1,INPUT);
  pinMode(pin_2,INPUT);
  pinMode(pin_3,INPUT);

  audio_init(DEVICE_Flash,MODE_loopOne,music_vol);//初始化mp3模块
}

void loop()
{
  ret = getRtcTimeString();

  ble();

  if(alarm_minute+delay_time>=60)
  {
    delay_minute=alarm_minute+delay_time-60;
    if(rtc.getMinute()>50&&rtc.getMinute()<=59)
    {
      if(rtc.getHour()==alarm_hour&&rtc.getMinute()>=alarm_minute&&(analogRead(pin_1)==0||analogRead(pin_2)==0||analogRead(pin_3)==0))
        play_lage=true;
      else
        play_lage=false;
    }
    if(rtc.getMinute()>=0&&rtc.getMinute()<10)
    {
      if(rtc.getHour()==alarm_hour&&rtc.getMinute()<=delay_minute&&(analogRead(pin_1)==0||analogRead(pin_2)==0||analogRead(pin_3)==0))
        play_lage=true;
      else
        play_lage=false;
    }
  }
  else
  {
    delay_minute=alarm_minute+delay_time;
    if(rtc.getHour()==alarm_hour&&rtc.getMinute()>=alarm_minute&&rtc.getMinute()<=delay_minute&&(analogRead(pin_1)==0||analogRead(pin_2)==0||analogRead(pin_3)==0))
      play_lage=true;
    else
      play_lage=false;
  }

  if(play_music&&play_lage)
  {
    play_music=false;
    play_stop=true;
    audio_play();
    Serial.println("--------------play----------");
  }
  if(play_stop&&analogRead(pin_1)>1000&&analogRead(pin_2)>1000&&analogRead(pin_3)>1000)
  {
    play_stop=false;
    play_music=true;
    audio_pause();
    Serial.println("--------------stop----------");
  }

  Serial.print(analogRead(pin_1));
  Serial.print("  ,  ");
  Serial.print(analogRead(pin_2));
  Serial.print("  ,  ");
  Serial.println(analogRead(pin_3));

}


void ble(){
  if (my_Serial.available() > 0) {
    cmdByte = my_Serial.read();

    switch (cmdByte) {
    case 't' :
      {
        _year = my_Serial.parseInt(); 
        _month = my_Serial.parseInt(); 
        _day = my_Serial.parseInt(); 
        _hour = my_Serial.parseInt();
        _minute = my_Serial.parseInt();
        _sec = my_Serial.parseInt();
        setRtcTime(_year, _month, _day, _hour, _minute, _sec);
        ret = getRtcTimeString();
        my_Serial.print(ret);
      }
      break;
    case 'm' :
      {
        alarm_hour=my_Serial.parseInt();
        alarm_minute=my_Serial.parseInt();
        my_Serial.print("Alarm is ok");
        //msg = my_Serial.readStringUntil('\n');
      }
      break;
    }
    Serial.println(ret + " -> " + alarm_hour+" : "+alarm_minute);
  }
}

void setRtcTime (byte _year, byte _month, byte _day, byte _hour, byte _minute, byte _sec)     
{	 
  //clear out all the registers
  rtc.initClock();
  rtc.setDate(_day, 0, _month, 0, _year);
  rtc.setTime(_hour, _minute, _sec);
}

String getRtcTimeString(){
  dateStr = rtc.formatDate(RTCC_DATE_ASIA);
  dateStr += " ";
  dateStr += rtc.formatTime(RTCC_TIME_HMS);

  return dateStr;
}

















