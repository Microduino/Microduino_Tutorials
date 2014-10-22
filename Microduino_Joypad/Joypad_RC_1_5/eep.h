#include "Arduino.h"

#include <EEPROM.h>

#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int eeprom_correct_min[4];
  int eeprom_correct_max[4];
  int eeprom_mode;
};

//======================================
void eeprom_read()
{
  //EEPROM读取赋值
  config_type config_readback;
  EEPROM_read(0, config_readback);

  mode=config_readback.eeprom_mode;

  for(int a=0;a<4;a++)
  {
    joy_correct_min[a]=config_readback.eeprom_correct_min[a];
    joy_correct_max[a]=config_readback.eeprom_correct_max[a];
  }
}

void eeprom_write()
{
  // 定义结构变量config，并定义config的内容
  config_type config;

  config.eeprom_mode = mode;

  for(int a=0;a<4;a++)
  {
    config.eeprom_correct_min[a] = joy_correct_min[a];
    config.eeprom_correct_max[a] = joy_correct_max[a];
  } 

  // 变量config存储到EEPROM，地址0写入
  EEPROM_write(0, config); 	
}




