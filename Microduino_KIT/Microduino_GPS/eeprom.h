#include "arduino.h"

#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  long l_long;
  float f_float;
  int i_int_idate;		//日期缓存
  int i_int_num;		//文件序号
  char c_char[num_name];	//文件名
};

void eeprom_read()
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("===EEPROM===");
#endif
  config_type config_readback;
  EEPROM_read(0, config_readback);		//从0地址处读取

  //通过串口输出读取回来的资料

  idate_cache=config_readback.i_int_idate;	//EEPROM读出日期（日）缓存
  file_num=config_readback.i_int_num;		//EEPROM读出文件序号

  for(int a=0;a<(num_name-1);a++)			//EEPRO读出文件名
    file_name[a]=config_readback.c_char[a];

  file_name_cache=file_name;		//启用文件名缓存

#ifdef DEBUG
  Serial.print("=== EEPROM file_name:");
  Serial.println(file_name);
  Serial.print("=== EEPROM file_num:");
  Serial.println(file_num);
  Serial.print("=== EEPROM idate_cache:");
  Serial.println(idate_cache);
  Serial.println("=== EEPROM Complete===");
  Serial.println(" ");
#endif
}  

void eeprom_write()
{
  config_type config;

  config.l_long=9999999;
  config.f_float = 3.14;
  config.i_int_idate = idate_cache;		//赋值：日期（日）缓存
  config.i_int_num = file_num;			//赋值：文件序号
  strcpy(config.c_char, file_name);		//赋值：文件名

  EEPROM_write(0, config);		//从0地址处写入
}
