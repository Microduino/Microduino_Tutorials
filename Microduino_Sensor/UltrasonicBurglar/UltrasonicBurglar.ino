#include <Wire.h>
#include <EEPROM.h>
#include <Rtc_Pcf8563.h>
#include <IRremote.h>
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)


Rtc_Pcf8563 rtc;//Real-time clock initialization

int data;
int enableState;
int led = 13;//led
//EEPROM配置
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

const int TrigPin = 2;
const int EchoPin = 3;
float cm;

int action;//record remote control button
int t;//inital value
int RECV_PIN = 11;//define IR receiver pin 11
IRrecv irrecv(RECV_PIN);
decode_results results;

struct config_type
{
  int EEPROMdata;
};



//show enable state
void draw() {
  u8g.setFont(u8g_font_courB18);//font
  u8g.setPrintPos(0, 16);
  u8g.print("Enable:");
  u8g.setPrintPos(0, 50);


if(enableState==1) {
  u8g.print("Day");
}else if(enableState==2) {
u8g.print("Night
");
}

} 


void setup()
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  
  pinMode(led, OUTPUT);   
  irrecv.enableIRIn(); // inital IR receiver
  t=0;
  vosettime();

  /*EEPROM读取赋值*/
  config_type config_readback;
  EEPROM_read(0, config_readback);
  data= config_readback.EEPROMdata;
  enableState=data;
}




void loop()
{

  rtc.formatDate();
  rtc.formatTime();

  if (irrecv.decode(&results))
  {
    action=deckey(results.value);

    if(action!=0) {
      config_type config;
      config.EEPROMdata = action;
      enableState=action;
      // 2) record config data to EEPROM, address is 0
      EEPROM_write(0, config);
    }


    delay(10);
    irrecv.resume();
  }

  if(enableState==1) {  //enable day state
    //Serial.println("day state");
    if(rtc.getHour()>7&&rtc.getHour()<23) {
      getDistance();
    }
  } 
  else if(enableState==2) {//enable night state
    //Serial.println("night state");
    if(rtc.getHour()>23&&rtc.getHour()<7) {
      getDistance();
    }
  }



    u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);

  /*

   Serial.print("Date: 20");
   Serial.print(rtc.getYear());
   Serial.print("/");
   Serial.print(rtc.getMonth());
   Serial.print("/");
   Serial.println(rtc.getDay());
   Serial.print("TIME:");
   Serial.print(rtc.getHour());
   Serial.print(":");
   Serial.print(rtc.getMinute());
   Serial.print(":");
   Serial.println(rtc.getSecond());
   
   */

}


void getDistance() {

  digitalWrite(TrigPin, LOW); //Low-high-low-fat one short pulse to TrigPin
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  cm = pulseIn(EchoPin, HIGH) / 58.0; //Echo time will be converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Two decimal places
  
  Serial.println(cm);
  
  //if distance less than 30cm, tell someone something happand
  if(cm<=30) {
    digitalWrite(led, HIGH);//led light on alarm
  } else {
    digitalWrite(led, LOW); //led off
  }
  
  
}







//=======================初始化时间函数===========================//
void vosettime()
{
  //rtc.initClock();
  //day, weekday, month, century(1=1900, 0=2000), year(0-99)
  rtc.setDate(19, 6, 04, 0, 14);
  //hr, min, sec
  rtc.setTime(13, 10, 00);
}



int deckey(unsigned long t)
{
  switch(t){
  case 0xFD08F7://button 1's code
    return 1;
    break;
  case 0xFD8877://button 2's code
    return 2;
    break;
  case 0xFD48B7://button 3's code
    return 3;
    break;
  case 0xFD28D7://button 4's code
    return 4;
    break;
  case 0xFDA857://button 5's code
    return 5;
    break;
  case 0xFD6897://button 6's code
    return 6;
    break;
  case 0xFD18E7://button 7's code
    return 7;
    break;
  case 0xFD9867://button 8's code
    return 8;
    break;
  case 0xFD58A7://button 9's code
    return 9;
    break;
  default:
    return 0;
    break;
  }
} 



