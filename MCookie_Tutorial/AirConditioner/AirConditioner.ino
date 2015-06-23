
#include <Wire.h>
#include <AM2321.h>
#include <IRremote.h>

IRsend irsend;
AM2321 am2321;

#define POWER_ON 0xC1602680
#define TEMP_MAX 30
#define TEMP_MIN 25
float temp = 0;        //
unsigned long runTime = 0;

void setup()
{
}

void loop() {
    am2321.read();
    temp = am2321.temperature/10.0;
    if(millis()>runTime+180000)
    {
      if(temp > TEMP_MAX)
      {
        irsend.sendNEC(POWER_ON,32);
        irsend.sendCode(0x00000000,16);
        runTime = millis();
      }
      else if(temp <TEMP_MIN)
      {
        irsend.sendNEC(0xC1602680,32);
        irsend.sendCode(0x00000000,16);
        runTime = millis();  
      }
    }
  delay(1000);  
}



