#include "Arduino.h"

//USER-----------------------------
#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int EEPROM_TEMP[4];
  int EEPROM_alarm_hour;
  int EEPROM_alarm_minute;
  int EEPROM_alarm_switch;
  int EEPROM_alarm_tone;
  int EEPROM_MENU_FONT;
  int EEPROM_alarm_vol;
};


void eeprom_WRITE()
{
  config_type config;  		// 定义结构变量config，并定义config的内容
  for(int a=0;a<4;a++)
    config.EEPROM_TEMP[a] = temp_SET[a];
  config.EEPROM_alarm_hour=alarm_hour;
  config.EEPROM_alarm_minute=alarm_minute;
  config.EEPROM_alarm_switch=alarm_switch;
  config.EEPROM_alarm_tone=alarm_tone;
  config.EEPROM_MENU_FONT=MENU_FONT;
  config.EEPROM_alarm_vol=alarm_vol;

  EEPROM_write(0, config); 	// 变量config存储到EEPROM，地址0写入
}

void eeprom_READ()
{
  config_type config_readback;
  EEPROM_read(0, config_readback);
  for(int a=0;a<4;a++)
    temp_SET[a] = config_readback.EEPROM_TEMP[a];
  alarm_hour=config_readback.EEPROM_alarm_hour;
  alarm_minute=config_readback.EEPROM_alarm_minute;
  alarm_switch=config_readback.EEPROM_alarm_switch;
  alarm_tone=config_readback.EEPROM_alarm_tone;
  MENU_FONT=config_readback.EEPROM_MENU_FONT;
  alarm_vol=config_readback.EEPROM_alarm_vol;
}

unsigned int freeRam () {
  extern unsigned int __heap_start, *__brkval; 
  unsigned int v; 
  return (unsigned int) &v - (__brkval == 0 ? (unsigned int) &__heap_start : (unsigned int) __brkval); 
}

//USER-----------------------------




