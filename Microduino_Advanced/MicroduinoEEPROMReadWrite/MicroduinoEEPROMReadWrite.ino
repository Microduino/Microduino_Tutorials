#include <avr/eeprom.h>
#include "U8glib.h"
#define EEPROM_write(address, var) eeprom_write_block((const void *)&(var), (void *)(address), sizeof(var))
#define EEPROM_read(address, var) eeprom_read_block((void *)&(var), (const void *)(address), sizeof(var))

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

// 定义一个包含 int 类别的 config_type 结构
struct config_type
{
  int num;
};


void draw() {
  // 1) 定义结构变量config，并定义config的内容
  config_type config;
  config.num = random(3000);//在0~3000里选定随机数
  // 2) 把变量config存储到EEPROM，从地址0开始写入。
  EEPROM_write(0, config);
  // 3) 从EEPROM地址0开始读取，结果存储在config_readback
  config_type config_readback;
  EEPROM_read(0, config_readback);
  u8g.setFont(u8g_font_unifont);//字体1
  u8g.drawStr( 0, 16, "Num:");
  u8g.setFont(u8g_font_7x13);//字体2
  u8g.setPrintPos(0, 32);
  u8g.print(config_readback.num);

}

void setup()
{

}

void loop()
{
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
  
  delay(2000);//延迟两秒
  
}

