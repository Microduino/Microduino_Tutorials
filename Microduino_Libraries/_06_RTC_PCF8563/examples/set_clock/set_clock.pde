#include <Wire.h>
#include <Rtc_Pcf8563.h>

//init the real time clock
Rtc_Pcf8563 rtc;

void setup()
{
  Serial.begin(9600);
  //clear out the registers
  rtc.initClock();
  //set a time to start with.
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setDate(29, 4, 8, 0, 13);
  //hr, min, sec
  rtc.setTime(00, 21, 0);
}

void loop()
{
  //both format functions call the internal getTime() so that the 
  //formatted strings are at the current time/date.
  Serial.println("CODE_1:");
  Serial.print(rtc.formatTime());
  Serial.print("     ");
  Serial.print(rtc.formatDate());
  Serial.print("\r\n");

  Serial.println("CODE_2:");
  Serial.print("20");
  Serial.print(rtc.getYear());
  Serial.print("/");
  Serial.print(rtc.getMonth());
  Serial.print("/");
  Serial.print(rtc.getDay());
  Serial.print("     ");
  Serial.print(rtc.getHour());
  Serial.print(":");
  Serial.print(rtc.getMinute());
  Serial.print(":");
  Serial.print(rtc.getSecond());
  Serial.print("\r\n");

  delay(1000);
  Serial.print("\r\n");
}
