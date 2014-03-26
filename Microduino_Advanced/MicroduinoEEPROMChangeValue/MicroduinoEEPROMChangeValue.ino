#include <IRremote.h>
#include <avr/eeprom.h>
#include "U8glib.h"
#define EEPROM_write(address, var) eeprom_write_block((const void *)&(var), (void *)(address), sizeof(var))
#define EEPROM_read(address, var) eeprom_read_block((void *)&(var), (const void *)(address), sizeof(var))

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

int action;//用于记录遥控器按键
int t;//初始值
int RECV_PIN = 11;//定义红外接收器的引脚为11
IRrecv irrecv(RECV_PIN);
decode_results results;

// 定义一个包含 int 类别的 config_type 结构
struct config_type
{
  int num;
};


void draw(int changeNum) {
  // 1) 定义结构变量config，并定义config的内容
  config_type config;
  config.num = changeNum;
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


void showNum() {
  config_type config_readback;
  EEPROM_read(0, config_readback);
  u8g.setFont(u8g_font_unifont);//字体1
  u8g.drawStr( 0, 16, "Num:");
  u8g.setFont(u8g_font_7x13);//字体2
  u8g.setPrintPos(0, 32);
  u8g.print(config_readback.num);
}  


int deckey(unsigned long t)
{
  switch(t){
  case 0xFD08F7://按钮1的编码
    return 1;
    break;
  case 0xFD8877://按钮2的编码
    return 2;
    break;
  case 0xFD48B7://按钮3的编码
    return 3;
    break;
  case 0xFD28D7://按钮4的编码
    return 4;
    break;
  case 0xFDA857://按钮5的编码
    return 5;
    break;
  case 0xFD6897://按钮6的编码
    return 6;
    break;
  case 0xFD18E7://按钮7的编码
    return 7;
    break;
  case 0xFD9867://按钮8的编码
    return 8;
    break;
  case 0xFD58A7://按钮9的编码
    return 9;
    break;
  default:
    return 0;
    break;
  }
} 

void setup()
{
  irrecv.enableIRIn(); // 初始化红外接收器
  t=0;
  showNum();
}

void loop()
{
  if (irrecv.decode(&results))
  {
    action=deckey(results.value);
    u8g.firstPage();  
    do {
      draw(action);
    } 
    while( u8g.nextPage() ); 

    delay(10);
    irrecv.resume();
  }
}



