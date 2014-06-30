
#include <Wire.h>
#include <Rtc_Pcf8563.h>

/* get a real time clock object */
Rtc_Pcf8563 rtc;
/* a flag for the interrupt */
volatile int alarm_flag=0;
long timer=millis();

/* the interrupt service routine */
void blink()
{
  alarm_flag=1;
}

void setup()
{
  pinMode(2, INPUT);           // set pin to input

  Serial.begin(9600);

  /* clear out all the registers */
  rtc.initClock();
  /* set a time to start with.
   * day, weekday, month, century, year */
  rtc.setDate(16, 1, 12, 0, 13);
  //hr, min, sec
  rtc.setTime(10, 2, 50);
  //  Serial.println("debug set time");
  /* set an alarm for 10 secs later...
   * alarm pin goes low when match occurs
   * this triggers the interrupt routine
   * min, hr, day, weekday 
   * 99 = no alarm value to be set
   */
  rtc.clearAlarm();   
  rtc.setAlarm(3, 10, 99, 99);
  Serial.println("debug set alarm");

  /* setup int on pin 2 of arduino */
  attachInterrupt(0, blink, FALLING);

}

void loop()
{
  if(millis()-timer>500)
  {
    /* each sec update the display */
    Serial.print(rtc.formatTime());
    Serial.print("  ");
    Serial.print(rtc.formatDate());
    Serial.print("  0x");
    Serial.print(rtc.getStatus2(), HEX);
    Serial.print("\r\n");
    delay(200);
    timer=millis();  
  }

  if (alarm_flag==1){
    clr_alarm();
  }
}

void clr_alarm()
{
  delay(2000);
  Serial.print("blink!\r\n");

  rtc.clearAlarm();   
  detachInterrupt(0);
  alarm_flag=0;
  attachInterrupt(0, blink, FALLING);
}