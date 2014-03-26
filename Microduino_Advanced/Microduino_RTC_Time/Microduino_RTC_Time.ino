#include <Wire.h>
#include <Rtc_Pcf8563.h>

Rtc_Pcf8563 rtc;//初始化实时时钟

#define Duan1 0
#define Duan2 3
#define Duan3 4
#define Duan4 8
#define Colon1 6
#define Colon2 7

int timehourg,timehours,timeming,timemins;
int timeyear1,timeyear2;
int timemouth1,timemouth2;
int timeday1,timeday2;

long  data[10]=
{
  0xfc,0x60,0xDA,0xF2,0x66,0xB6,0xBE,0xE0,0xFE,0xF6
};

void setup()
{
  // Serial.begin(115200);
  for(int x=0;x<14;x++) 
  {
    pinMode(x,OUTPUT);//设置输出引脚
  }  
  vosettime();
}
void loop()
{
  rtc.formatDate();
  rtc.formatTime();

  timehours=rtc.getHour()/10;
  timehourg=rtc.getHour()%10;
  timemins=rtc.getMinute()/10;
  timeming=rtc.getMinute()%10;

  timeyear1=rtc.getYear()/10;
  timeyear2=rtc.getYear()%10;

  timemouth1=rtc.getMonth()/10;
  timemouth2=rtc.getMonth()%10;

  timeday1=rtc.getDay()/10;
  timeday2=rtc.getDay()%10;  

  daytime();
/*
  for(int i=0;i<500;i++)
    datetime();
  for(int i=0;i<500;i++)
    datatime();
*/
  /*
  Serial.print("TIME:");
   Serial.print("20");
   Serial.print(rtc.getYear());
   Serial.print("/");
   Serial.print(rtc.getMonth());
   Serial.print("/");
   Serial.println(rtc.getDay());
   Serial.print(rtc.getHour());
   Serial.print(":");
   Serial.print(rtc.getMinute());
   Serial.print(":");
   Serial.println(rtc.getSecond());
   delay(1000);
   */
}
void daytime()
{
  duanma(1);
  leddisplay(data[timehours]);
  delay(3); 
  duanma(2);
  leddisplay(data[timehourg]);
  delay(3);
  duanma(3);
  leddisplay(data[timemins]);
  delay(3);
  duanma(4);
  leddisplay(data[timeming]);
  delay(3);

  if(rtc.getSecond()%2==0)
  {
    digitalWrite(Colon1, LOW);  
    digitalWrite(Colon2, HIGH); 
  }
  else
  {
    digitalWrite(Colon1, HIGH);  
    digitalWrite(Colon2, LOW); 
  }
}

void datatime()
{
  duanma(1);
  leddisplay(data[timemouth1]);
  delay(3); 
  duanma(2);
  leddisplay(data[timemouth2]);
  delay(3);
  duanma(3);
  leddisplay(data[timeday1]);
  delay(3);
  duanma(4);
  leddisplay(data[timeday2]);
  delay(3);
}

void datetime()
{
  duanma(1);
  leddisplay(data[2]);
  delay(3); 
  duanma(2);
  leddisplay(data[0]);
  delay(3);
  duanma(3);
  leddisplay(data[timeyear1]);
  delay(3);
  duanma(4);
  leddisplay(data[timeyear2]);
  delay(3);
}
//=======================初始化设置时间函数===========================//
void vosettime()
{
 // rtc.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setDate(21, 6, 12, 0, 13);
  //hr, min, sec
  rtc.setTime(20,50, 50);
}

void leddisplay(int num)    // 将花样字模对应到端口显示
{
  /*=====================================================================
   先将16进制数向右移x位（num>>x），x代表了microduinoI/O口对应16进制的某位，
   0是最低位，7是最高位。再将移的数据与0x01按位与，就可以得到16进制数某位的
   数据（0或1），再将其值赋值给microduino的I/O就行。 
   ======================================================================*/
  digitalWrite(11, ((num>>0)&0x01));
  digitalWrite(9, ((num>>1)&0x01));
  digitalWrite(2, ((num>>2)&0x01));
  digitalWrite(13, ((num>>3)&0x01));
  digitalWrite(12, ((num>>4)&0x01));
  digitalWrite(10, ((num>>5)&0x01));
  digitalWrite(5, ((num>>6)&0x01));
  digitalWrite(1,((num>>7)&0x01));
}
void duanma(int x)
{
  switch(x)
  {
  case 1:
    {
      digitalWrite(Duan1, LOW);  
      digitalWrite(Duan2, HIGH);  
      digitalWrite(Duan3, HIGH);  
      digitalWrite(Duan4, HIGH);  
    }
    break;
  case 2:
    {
      digitalWrite(Duan1, HIGH);  
      digitalWrite(Duan2, LOW);  
      digitalWrite(Duan3, HIGH);  
      digitalWrite(Duan4, HIGH);  
    }
    break;
  case 3:
    {
      digitalWrite(Duan1, HIGH);  
      digitalWrite(Duan2, HIGH);  
      digitalWrite(Duan3, LOW);  
      digitalWrite(Duan4, HIGH);  
    }
    break;
  case 4:
    {
      digitalWrite(Duan1, HIGH);  
      digitalWrite(Duan2, HIGH);  
      digitalWrite(Duan3, HIGH);  
      digitalWrite(Duan4, LOW);  
    }
    break;
  }
}











