/*
=================================
 本作品采用知识共享 署名-非商业性使用-相同方式共享 3.0 未本地化版本 许可协议进行许可
 访问 http://creativecommons.org/licenses/by-nc-sa/3.0/ 查看该许可协议
 版权所有：老潘orz
 =================================
 
 Microduino维基： wiki.microduino.cc
 
 原帖：http://www.geek-workshop.com/thread-11199-1-1.html 
 */

#include "def.h"
#include "bat.h"
#include "time.h"
#include "gps.h"
#include "gpx.h"
#include "eeprom.h"
#include "sd.h"
#include "osd.h"
#include "key.h"

//osd==========================
#include <U8glib.h>
//GPS==========================
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
//SD===========================
#include <SD.h>
//EEPROM=======================
#include <EEPROM.h>

void setup()
{
  Serial.begin(115200);
  GPS.begin(38400);

  pinMode(PIN_key1,INPUT_PULLUP); 		//上拉
  pinMode(PIN_LED1,OUTPUT); 		//LED
  pinMode(PIN_LED2,OUTPUT); 		//LED

  //------------------------------
  eeprom_read();					//读取EEPROM
  //  vostring();
  Serial.print("file_name: ");  
  Serial.println(file_name);

  //------------------------------
  Serial.println("Initializing SD card...");
  if (!SD.begin(PIN_SD_CS)) 		//初始化SD卡
  {
    sd_sta=false;			//SD卡不正常
    Serial.println("initialization failed!");
    return;
  }
  else
  {
    sd_sta=true;				//SD卡正常
    vosdbegin(file_name);		//SD卡文件初始检测
    Serial.println("initialization done.");
  }

  //------------------------------
  //  u8g.setRot180();
  volcdlogo(0, 10);
  delay(2000);
}

void loop()
{
  //按键检测-------------------------------
  file_updata=vokey(PIN_key1);
  if(file_updata)
  {
    vosd_dataupdata();
    time_oled_updata=millis();
  }
  b_oled_updata=!boolean(millis()-time_oled_updata>init_oled_updata);		//按键动作时

  //GPS-------------------------------
  vogps_databegin();
  vogps_dataread();

  //SD-------------------------------
  if(time_sdwrite>millis()) time_sdwrite=millis();
  if(millis()-time_sdwrite>init_sdwrite)
  {
    vosd_datawrite();
    time_sdwrite=millis();
  }

  //BAT--------------------------------
  if (time_bat > millis()) time_bat = millis();
  if(millis()-time_bat>init_bat)
  {
    vobat();
    time_bat=millis();
  }

  //OLED-------------------------------
  if (time_oled > millis()) time_oled = millis();
  if(millis()-time_oled>init_oled)
  {
    vooled();
    time_oled=millis();
  }
}
