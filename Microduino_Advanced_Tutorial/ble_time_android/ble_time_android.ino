#include <Wire.h>
#include <Rtc_Pcf8563.h>
#include <SoftwareSerial.h>

#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Rtc_Pcf8563 rtc;//初始化实时时钟

SoftwareSerial mySerial(4, 5); //RX,TX

Adafruit_BicolorMatrix matrix1 = Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix2 = Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix3 = Adafruit_BicolorMatrix();
Adafruit_BicolorMatrix matrix4 = Adafruit_BicolorMatrix();

int timehourg, timehours, timeming, timemins, timesec, timeweek;

int Year, Month, Day, Hour, Minute, Second, Weekday;

byte cmdByte;
String msg = "Microduino";
int _year, _month, _day, _hour, _minute, _sec;
String dateStr, ret;

#define DEBUG

char buffer[100];

boolean buffer_sta = false;

int buffer_num = 0;

int sta[6];

unsigned long remote;

boolean print_time, return_time, return_string;

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

  ret = getRtcTimeString();

  if (rtc.getSecond() % 2 == 0 && print_time == true)
  {
    print_time = false;
    mySerial.print(ret);
    Serial.println(ret + " -> " + msg);
  }
  else if (rtc.getSecond() % 2 != 0)
    print_time = true;

  for (int i = 0; i < 1000; i++)
  {
    bluetooth();
    if (return_time)
      return;
    if (return_string)
      vosetlcd(msg, (msg.length() + 3) * 8, LED_GREEN, 80);
    lcdtime();
  }
  for (int i = 0; i < 1; i++)
    vosetlcd(rtc.formatDate(RTCC_DATE_ASIA), 96, LED_RED, 80); //Char length*8
  for (int i = 0; i < 1; i++)
    vosetlcd(msg, (msg.length() + 3) * 8, LED_GREEN, 80); //Char (length+3)*8
}

void lcdtime()
{
  rtc.formatDate();
  rtc.formatTime();
  timeweek = rtc.getWeekday();

  timehours = rtc.getHour() / 10;
  timehourg = rtc.getHour() % 10;
  timemins = rtc.getMinute() / 10;
  timeming = rtc.getMinute() % 10;

  if (rtc.getSecond() % 2 == 0)
    timesec = 1;
  else
    timesec = 0;

  matrix1.setTextSize(1);
  matrix1.setTextWrap(false);
  matrix1.setRotation(0);
  matrix1.clear();
  matrix1.setTextColor(LED_YELLOW);
  matrix1.setCursor(0, 0);
  matrix1.print(timehours);
  matrix1.print(timehourg);
  matrix1.writeDisplay();
  delay(1);
  matrix2.setTextSize(1);
  matrix2.setTextWrap(false);
  matrix2.setRotation(0);
  matrix2.clear();
  matrix2.setTextColor(LED_YELLOW);
  matrix2.setCursor(-2, 0);
  matrix2.print(timehourg);
  //------------------------------------Hour----------------------------//

  if (timesec == 1)
  {
    matrix2.drawPixel(7, 2, LED_RED);
    matrix2.drawPixel(7, 5, LED_RED);
    matrix2.writeDisplay();
  }
  else
  {
    matrix2.setCursor(5, 0);
    matrix2.print(" ");
    matrix2.writeDisplay();
  }
  delay(1);
  matrix3.setTextSize(1);
  matrix3.setTextWrap(false);
  matrix3.setRotation(0);
  matrix3.clear();
  matrix3.setTextColor(LED_GREEN);
  matrix3.setCursor(4, 0);
  matrix3.print(timemins);
  if (timesec == 1)
  {
    matrix3.drawPixel(0, 2, LED_RED);
    matrix3.drawPixel(0, 5, LED_RED);
    matrix3.writeDisplay();
  }
  else
  {
    matrix3.setCursor(0, 0);
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
  matrix4.setCursor(-4, 0);
  matrix4.print(timemins);
  matrix4.print(timeming);
  matrix4.writeDisplay();
  delay(1);
  //------------------------------------min----------------------------//
}

void lcdweek()
{
  switch (timeweek) {
    case 1:
      vosetlcd("Monday", 72, LED_RED, 80); //Char (length+3)*8
      break;
    case 2:
      vosetlcd("Tuesday", 80, LED_RED, 80);
      break;
    case 3:
      vosetlcd("Wednesday", 96, LED_RED, 80);
      break;
    case 4:
      vosetlcd("Thursday", 88, LED_RED, 80);
      break;
    case 5:
      vosetlcd("Friday", 72, LED_RED, 80);
      break;
    case 6:
      vosetlcd("Saturday", 88, LED_RED, 80);
      break;
    case 7:
      vosetlcd("Sunday", 72, LED_RED, 80);
      break;
  }
}

void lcdtemphun(int t, int color1, int color2, char* c, char* r)
{
  matrix1.setTextSize(1);
  matrix1.setTextWrap(false);
  matrix1.setRotation(0);
  matrix1.clear();
  matrix1.setTextColor(color1);
  matrix1.setCursor(0, 0);
  matrix1.print(t / 10);
  matrix1.writeDisplay();
  delay(1);
  matrix2.setTextSize(1);
  matrix2.setTextWrap(false);
  matrix2.setRotation(0);
  matrix2.clear();
  matrix2.setTextColor(color1);
  matrix2.setCursor(0, 0);
  matrix2.print(t % 10);
  matrix2.writeDisplay();
  delay(1);
  matrix3.setTextSize(1);
  matrix3.setTextWrap(false);
  matrix3.setRotation(0);
  matrix3.clear();
  matrix3.setTextColor(color2);
  matrix3.setCursor(0, 0);
  matrix3.print(c);
  matrix3.writeDisplay();
  delay(1);
  matrix4.setTextSize(1);
  matrix4.setTextWrap(false);
  matrix4.setRotation(0);
  matrix4.clear();
  matrix4.setTextColor(color2);
  matrix4.setCursor(0, 0);
  matrix4.print(r);
  matrix4.writeDisplay();
  delay(1);
}

//void vosetlcd(Char* wd,int x,int Color,int time)
void vosetlcd(String wd, int x, int Color, int time)
{
  for (int n = 0; n < x; n++)
  {
    matrix4.setTextSize(1);
    matrix4.setTextWrap(false);
    matrix4.setRotation(0);
    matrix4.clear();
    matrix4.setTextColor(Color);
    matrix4.setCursor(8 - n, 0);
    matrix4.print(wd);
    matrix4.writeDisplay();
    delay(1);
    if (n > 8)
    {
      matrix3.setTextSize(1);
      matrix3.setTextWrap(false);
      matrix3.setRotation(0);
      matrix3.clear();
      matrix3.setTextColor(Color);
      matrix3.setCursor(16 - n, 0);
      matrix3.print(wd);
      matrix3.writeDisplay();
    }
    if (n > 16)
    {
      matrix2.setTextSize(1);
      matrix2.setTextWrap(false);
      matrix2.setRotation(0);
      matrix2.clear();
      matrix2.setTextColor(Color);
      matrix2.setCursor(24 - n, 0);
      matrix2.print(wd);
      matrix2.writeDisplay();
      delay(1);
    }
    if (n > 24)
    {
      matrix1.setTextSize(1);
      matrix1.setTextWrap(false);
      matrix1.setRotation(0);
      matrix1.clear();
      matrix1.setTextColor(Color);
      matrix1.setCursor(32 - n, 0);
      matrix1.print(wd);
      matrix1.writeDisplay();
      delay(1);
    }
    bluetooth();
    if (buffer_sta)
      return;
    delay(time);
  }
}

void bluetooth() {
  if (mySerial.available() > 0) {
    cmdByte = mySerial.read();

    switch (cmdByte) {
      case 't' :
        _year = mySerial.parseInt();
        _month = mySerial.parseInt();
        _day = mySerial.parseInt();
        _hour = mySerial.parseInt();
        _minute = mySerial.parseInt();
        _sec = mySerial.parseInt();
        setRtcTime(_year, _month, _day, _hour, _minute, _sec);
        return_time = true;
        break;

      case 'm' :
        msg = mySerial.readStringUntil('\n');
        return_string = true;
        break;
    }
    buffer_sta = true;
  }
  else
  {
    return_time = false;
    return_string = false;
    buffer_sta = false;
  }
}

void setRtcTime (byte _year, byte _month, byte _day, byte _hour, byte _minute, byte _sec)
{
  //clear out all the registers
  rtc.initClock();
  rtc.setDate(_day, 0, _month, 0, _year);
  rtc.setTime(_hour, _minute, _sec);
}

String getRtcTimeString() {
  dateStr = rtc.formatDate(RTCC_DATE_ASIA);
  dateStr += " ";
  dateStr += rtc.formatTime(RTCC_TIME_HMS);

  return dateStr;
}

