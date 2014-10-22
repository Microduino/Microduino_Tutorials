#include "Arduino.h"
#include "def.h"
#include "time.h"
#include "joy.h"
#include "eep.h"
#include "data.h"
#include "nrf.h"
#include "mwc.h"
#include "tft.h"

//joypad================================
#include <Joypad.h>
//eeprom================================
#include <EEPROM.h>
//TFT===================================
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
//rf====================================
#include <RF24Network.h>
#include <RF24.h>

//spi===================================
#include <SPI.h>


void setup() 
{
  // initialize serial communication at 115200 bits per second:

  Serial.begin(115200);
  delay(100);

#ifdef Serial_DEBUG
  Serial.println("========hello========");
#endif

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r EEPROM READ...");
#endif
  eeprom_read();

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT INIT...");
#endif
  TFT_init(true);

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT BEGIN...");
#endif
  TIME1=millis();
  while(millis()-TIME1<interval_TIME1)
  {
    TFT_begin();

    boolean correct=!Joypad.readButton(CH_SWITCH_1);
    if(correct)
    {
      correct=false;
      Joypad.tone(200, 200);

#ifdef Serial_DEBUG
      Serial.println("\n\rCorrect IN...");
#endif

      for(int a=0;a<4;a++)
        joy_correct_min[a]=0,joy_correct_max[a]=0;

      //---------------
#ifdef Serial_DEBUG
      Serial.println("\n\r TFT INIT...");
#endif
      TFT_init(false);

      while(1)
      {
        Joy_correct();
        if(!Joypad.readButton(CH_SWITCH_3))
        {
          mode=0;    //QuadCopter
          Joypad.tone(200, 200);
          break;
        }
        if(!Joypad.readButton(CH_SWITCH_4))
        {
          mode=1;    //Robot
          Joypad.tone(200, 200);
          break;
        }
      }
#ifdef Serial_DEBUG
      Serial.println("\n\rCorrect OUT...");
#endif

      //---------------
#ifdef Serial_DEBUG
      Serial.println("\n\r EEPROM WRITE...");
#endif
      eeprom_write();
    }
  }

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT CLEAR...");
#endif
  TFT_clear();

  //---------------
#ifdef Serial_DEBUG
  Serial.println("\n\r TFT READY...");
#endif
  TFT_ready();

  //---------------
  if(mode)    //Robot
  {
    SPI.begin();		//初始化SPI总线
    radio.begin();
    network.begin(/*channel*/ nRF_channal, /*node address*/ this_node);
  }
  else       //QuadCopter
  {
    mwc_port.begin(115200);  
  }
  //---------------
#ifdef Serial_DEBUG
  Serial.println("===========start===========");
#endif
}

void loop() 
{
  unsigned long time = millis();

  //===================================
  data_begin();

  if(millis()<time2) time2=millis();
  if(millis()-time2>interval_time2)
  {
    if(mode)      //Robot
      nrf_send();
    else          //QuadCopter
    data_send();
    time2=millis();
  }

  //节点查错-------------------------------
  vodebug();

  //TFT------------------------------------
  TFT_run();
  //===================================

  time = millis() - time;

  //  Serial.println(time, DEC);    //loop time
}
