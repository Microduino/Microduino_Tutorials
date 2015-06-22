#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <SoftwareSerial.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Rtc_Pcf8563 rtc;//初始化实时时钟

SoftwareSerial mySerial(4, 5); //RX,TX

Adafruit_BicolorMatrix matrix1= Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix2 = Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix3= Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix4 = Adafruit_BicolorMatrix();

int timehourg,timehours,timeming,timemins,timesec,timeweek;

int Year,Month,Day,Hour,Minute,Second,Weekday;

String msg = "";
String msg_display="Microduino";
String msg_change="";

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600); 
  matrix1.begin(0x70);  // pass in the address
  matrix2.begin(0x71);  // pass in the address
  matrix3.begin(0x72);  // pass in the address
  matrix4.begin(0x74);  // pass in the address

  //vosetlcd("Hello Microduino",128,LED_GREEN,100);//Char length*8
}

void loop() {

  for(int i=0;i<500;i++)
  {
    bluetooth();
    lcdtime();
  }
  /*
  for(int i=0;i<1;i++)
   {
   bluetooth();
   //vosetlcd(rtc.formatDate(RTCC_DATE_ASIA),96,LED_RED,80);//Char length*8
   }
   for(int i=0;i<1;i++)
   {
   bluetooth();
   //lcdweek();
   }
   */
}

void bluetooth()
{ 
  if (mySerial.available()>0) 
  {
    char color;
    char cmdByte = mySerial.read();

    if(cmdByte=='R')
      color='R';
    else if(cmdByte=='G')
      color='G';
    else if(cmdByte=='Y')
      color='Y';

    if(cmdByte=='t')
    {
      Year=mySerial.parseInt();
      Month=mySerial.parseInt();
      Day=mySerial.parseInt();
      Hour=mySerial.parseInt();
      Minute=mySerial.parseInt();
      Weekday=mySerial.parseInt();

      setRTC();

      Serial.print("Y:");
      Serial.print(Year);
      Serial.print(" M:");
      Serial.print(Month);
      Serial.print(" D:");
      Serial.print(Day);
      Serial.print(" H:");
      Serial.print(Hour);
      Serial.print(" M:");
      Serial.print(Minute);
      Serial.print(" W:");
      Serial.println(Weekday);
    }
    else
    {
      msg = mySerial.readStringUntil('\n');
      msg_display=msg.substring(1,msg.length());

      Serial.println(msg_display);

      if(color=='R')
        vosetlcd(msg_display,(msg_display.length()+3)*8,LED_RED,80);//Char (length+3)*8
      if(color=='Y')
        vosetlcd(msg_display,(msg_display.length()+3)*8,LED_GREEN,80);//Char (length+3)*8
      if(color=='G')
        vosetlcd(msg_display,(msg_display.length()+3)*8,LED_YELLOW,80);//Char (length+3)*8
    }
  }
}

void lcdweek()
{
  switch (timeweek) {
  case 1:
    vosetlcd("Monday",72,LED_RED,80);//Char (length+3)*8
    break;
  case 2:
    vosetlcd("Tuesday",80,LED_RED,80);
    break;
  case 3:
    vosetlcd("Wednesday",96,LED_RED,80);
    break;
  case 4:
    vosetlcd("Thursday",88,LED_RED,80);
    break;
  case 5:
    vosetlcd("Friday",72,LED_RED,80);
    break;
  case 6:
    vosetlcd("Saturday",88,LED_RED,80);
    break;
  case 7:
    vosetlcd("Sunday",72,LED_RED,80);
    break;
  } 
}

void lcdtime()
{
  rtc.formatDate();
  rtc.formatTime();
  timeweek=rtc.getWeekday();

  timehours=rtc.getHour()/10;
  timehourg=rtc.getHour()%10;
  timemins=rtc.getMinute()/10;
  timeming=rtc.getMinute()%10;

  if(rtc.getSecond()%2==0)
    timesec=1;
  else
    timesec=0;

  matrix1.setTextSize(1);
  matrix1.setTextWrap(false);  
  matrix1.setRotation(0);
  matrix1.clear();
  matrix1.setTextColor(LED_YELLOW);
  matrix1.setCursor(0,0);
  matrix1.print(timehours);
  matrix1.print(timehourg);
  matrix1.writeDisplay();
  delay(1);
  matrix2.setTextSize(1);
  matrix2.setTextWrap(false); 
  matrix2.setRotation(0);
  matrix2.clear();
  matrix2.setTextColor(LED_YELLOW);
  matrix2.setCursor(-2,0);
  matrix2.print(timehourg);
  //------------------------------------Hour----------------------------//

  if(timesec==1)
  {
    matrix2.drawPixel(7, 2, LED_RED); 
    matrix2.drawPixel(7, 5, LED_RED);  
    matrix2.writeDisplay();  
  }
  else
  { 
    matrix2.setCursor(5,0);
    matrix2.print(" ");
    matrix2.writeDisplay();
  }
  delay(1);
  matrix3.setTextSize(1);
  matrix3.setTextWrap(false); 
  matrix3.setRotation(0);
  matrix3.clear();
  matrix3.setTextColor(LED_GREEN);
  matrix3.setCursor(4,0);
  matrix3.print(timemins);
  if(timesec==1)
  {
    matrix3.drawPixel(0, 2, LED_RED); 
    matrix3.drawPixel(0, 5, LED_RED);  
    matrix3.writeDisplay();  
  }
  else
  { 
    matrix3.setCursor(0,0);
    matrix3.print(" ");
    matrix3.writeDisplay();
  }

  //------------------------------------sec----------------------------//
  delay(1);
  matrix4.setTextSize(1);
  matrix4.setTextWrap(false); 
  matrix4.setRotation(0);
  matrix4.clear();
  matrix4.setTextColor(LED_GREEN);
  matrix4.setCursor(-4,0);
  matrix4.print(timemins);
  matrix4.print(timeming);
  matrix4.writeDisplay();
  delay(1);
  //------------------------------------min----------------------------//
}

void lcdtemphun(int t,int color1,int color2,char* c,char* r)
{
  matrix1.setTextSize(1);
  matrix1.setTextWrap(false);  
  matrix1.setRotation(0);
  matrix1.clear();
  matrix1.setTextColor(color1);
  matrix1.setCursor(0,0);
  matrix1.print(t/10);
  matrix1.writeDisplay();
  delay(1);
  matrix2.setTextSize(1);
  matrix2.setTextWrap(false); 
  matrix2.setRotation(0);
  matrix2.clear();
  matrix2.setTextColor(color1);
  matrix2.setCursor(0,0);
  matrix2.print(t%10);
  matrix2.writeDisplay();  
  delay(1);
  matrix3.setTextSize(1);
  matrix3.setTextWrap(false); 
  matrix3.setRotation(0);
  matrix3.clear();
  matrix3.setTextColor(color2);
  matrix3.setCursor(0,0);
  matrix3.print(c);
  matrix3.writeDisplay();
  delay(1);
  matrix4.setTextSize(1);
  matrix4.setTextWrap(false); 
  matrix4.setRotation(0);
  matrix4.clear();
  matrix4.setTextColor(color2);
  matrix4.setCursor(0,0);
  matrix4.print(r);
  matrix4.writeDisplay();
  delay(1);
}

//void vosetlcd(char* wd,int x,int Color,int time)
void vosetlcd(String wd,int x,int Color,int time)
{
  for(int n=0;n<x;n++)
  {
    matrix4.setTextSize(1);
    matrix4.setTextWrap(false);  
    matrix4.setRotation(0);
    matrix4.clear();
    matrix4.setTextColor(Color);
    matrix4.setCursor(8-n,0);
    matrix4.print(wd);
    matrix4.writeDisplay();
    delay(1);
    if(n>8)
    {
      matrix3.setTextSize(1);
      matrix3.setTextWrap(false);  
      matrix3.setRotation(0);
      matrix3.clear();
      matrix3.setTextColor(Color);
      matrix3.setCursor(16-n,0);
      matrix3.print(wd);
      matrix3.writeDisplay();
    }
    if(n>16)
    {
      matrix2.setTextSize(1);
      matrix2.setTextWrap(false);
      matrix2.setRotation(0);
      matrix2.clear();
      matrix2.setTextColor(Color);
      matrix2.setCursor(24-n,0);
      matrix2.print(wd);
      matrix2.writeDisplay();
      delay(1);
    }
    if(n>24)
    {
      matrix1.setTextSize(1);
      matrix1.setTextWrap(false);
      matrix1.setRotation(0);
      matrix1.clear();
      matrix1.setTextColor(Color);
      matrix1.setCursor(32-n,0);
      matrix1.print(wd);
      matrix1.writeDisplay();
      delay(1);
    }
    if (mySerial.available() > 0)
    {
      msg = mySerial.readStringUntil('\n');
      msg_display=msg.substring(2,msg.length());
      if(msg[0]=='R'||msg[0]=='G'||msg[0]=='Y'&&(msg_change!=msg_display))
      {
        msg_change=msg_display;
        time=0;
        matrix1.clear();
        matrix2.clear();
        matrix3.clear();
        matrix4.clear();
        if(msg[0]=='R')
          vosetlcd(msg_display,(msg_display.length()+3)*8,LED_RED,80);//Char (length+3)*8
        if(msg[0]=='Y')
          vosetlcd(msg_display,(msg_display.length()+3)*8,LED_GREEN,80);//Char (length+3)*8
        if(msg[0]=='G')
          vosetlcd(msg_display,(msg_display.length()+3)*8,LED_YELLOW,80);//Char (length+3)*8

        msg="";
      }
      else
        time=150;
    }
    delay(time);
  }
}

//=======================初始化设置时间函数===========================//
void setRTC()
{
  rtc.initClock();  //set a time to start with.
  rtc.setDate(Day, Weekday, Month, 0, Year);  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setTime(Hour, Minute, 0);  //hr, min, sec  
}





