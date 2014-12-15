#include "Arduino.h"

#include <Wire.h>
#include <Rtc_Pcf8563.h>

//init the real time clock
Rtc_Pcf8563 pcf_rtc;

#define TIME_ZONE +8

int Year,Month,Day,Hour,Minute,Second,Weekday;


/*==============================================================================*/
/* Useful Constants */
#define SECS_PER_MIN  (60UL)
#define SECS_PER_HOUR (3600UL)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24UL)
#define DAYS_PER_WEEK (7UL)
#define SECS_PER_WEEK (SECS_PER_DAY * DAYS_PER_WEEK)
#define SECS_PER_YEAR (SECS_PER_WEEK * 52UL)

#define LEAP_YEAR(Y)     ( ((1970+Y)>0) && !((1970+Y)%4) && ( ((1970+Y)%100) || !((1970+Y)%400) ) )

static  const uint8_t monthDays[]=
{
  31,28,31,30,31,30,31,31,30,31,30,31
}; // API starts months from 1, this array starts from 0


//RTC to timeStamp
uint32_t getTimeStamp(int c_Hour,int c_Minute,int c_Second,int c_Day, int c_Month, int c_Year)
{
  c_Year -= 1970;

  // assemble time elements into time_t 
  // note year argument is offset from 1970 (see macros in time.h to convert to other formats)
  // previous version used full four digit year (or digits since 2000),i.e. 2009 was 2009 or 9
  int i;
  uint32_t seconds;

  // seconds from 1970 till 1 jan 00:00:00 of the given year
  seconds= c_Year*(SECS_PER_DAY * 365);
  for (i = 0; i < c_Year; i++) {
    if (LEAP_YEAR(i)) {
      seconds +=  SECS_PER_DAY;   // add extra days for leap years
    }
  }

  // add days for this year, months start from 1
  for (i = 1; i < c_Month; i++) {
    if ( (i == 2) && LEAP_YEAR(c_Year)) { 
      seconds += SECS_PER_DAY * 29;
    } 
    else {
      seconds += SECS_PER_DAY * monthDays[i-1];  //monthDay array starts from 0
    }
  }
  seconds+= (c_Day-1) * SECS_PER_DAY;
  seconds+= c_Hour * SECS_PER_HOUR;
  seconds+= c_Minute * SECS_PER_MIN;
  seconds+= c_Second;

  seconds-= TIME_ZONE * SECS_PER_HOUR;

  //  Serial.print("Current date and time:");
  //  Serial.println(seconds);
  return seconds; 
}

//setRTC
void setRTC()
{
  pcf_rtc.initClock();  //set a time to start with.
  pcf_rtc.setDate(Day, 4, Month, 0, Year-2000);  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  pcf_rtc.setTime(Hour, Minute, 0);  //hr, min, sec  
}

//getRTC
void getRTC()
{
  pcf_rtc.getDate();
  pcf_rtc.getTime();

  //----------------------

  Year=pcf_rtc.getYear()+2000;
  Month=pcf_rtc.getMonth();
  Day=pcf_rtc.getDay();
  Hour=pcf_rtc.getHour();
  Minute=pcf_rtc.getMinute();
  Second=pcf_rtc.getSecond();
}


//timeStamp to RTC
void getDateStamp(uint32_t _getDateStamp) 
{
  _getDateStamp += TIME_ZONE * SECS_PER_HOUR;

  unsigned long _weekday = (((_getDateStamp/3600/24)+4)%7);
  Weekday=_weekday;

  Year = 1970;
  while(1) {
    uint32_t Seconds;
    if(LEAP_YEAR(Year-1970)) Seconds = SECS_PER_DAY * 366;
    else Seconds = SECS_PER_DAY * 365;
    if(_getDateStamp >= Seconds) {
      _getDateStamp -= Seconds;
      Year++;
    } 
    else break;
  }

  Month = 0;
  while(1) {
    uint32_t Seconds = SECS_PER_DAY * monthDays[Month];
    if(LEAP_YEAR(Year-1970) && Month == 1) Seconds = SECS_PER_DAY * 29;
    if(_getDateStamp >= Seconds) {
      _getDateStamp -= Seconds;
      Month++;
    } 
    else break;
  }  
  Month++;

  Day = 1;
  while(1) {
    if(_getDateStamp >= SECS_PER_DAY) {
      _getDateStamp -= SECS_PER_DAY;
      Day++;
    }
    else break;
  }  

  Hour = _getDateStamp / 3600;
  Minute = (_getDateStamp - (uint32_t)Hour * 3600) / 60;
  Second = (_getDateStamp - (uint32_t)Hour * 3600) - Minute * 60;

  /*
  Serial.print("week: ");
   Serial.println(Weekday);
   
   Serial.print(Day);
   Serial.print("/");
   
   Serial.print(Month);
   Serial.print("/");  
   
   Serial.println(Year);
   
   Serial.print(Hour);
   Serial.print(":");
   
   Serial.print(Minute);
   Serial.print(":");
   
   Serial.println(Second);
   
   Serial.println();
   */
}


