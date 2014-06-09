#include <Wire.h>
#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;//inital rtc

void setup()
{
  Serial.begin(9600);
  vosettime();
}

void loop()
{
  rtc.formatDate();
  rtc.formatTime();
//send time data to port
  Serial.print(rtc.getHour());
  Serial.print(":");
  Serial.print(rtc.getMinute());
  Serial.print(":");
  Serial.println(rtc.getSecond());
}

//inital time
void vosettime()
{
  //rtc.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setDate(4, 1, 6, 0, 14);
  //hr, min, sec
  rtc.setTime(15, 28, 50);
}



