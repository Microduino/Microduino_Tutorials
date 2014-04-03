#include <avr/eeprom.h>
#include "U8glib.h"
#define EEPROM_write(address, var) eeprom_write_block((const void *)&(var), (void *)(address), sizeof(var))
#define EEPROM_read(address, var) eeprom_read_block((void *)&(var), (const void *)(address), sizeof(var))

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//The definition OLED connection

// define a struct for record data
struct config_type
{
  int num;
};


void draw() {

  config_type config;
  config.num = random(3000);//Random Number in 0~3000
  //  record config data to EEPROM, address is 0
  EEPROM_write(0, config);
  // ) read from address 0 of EEPROM, result copy to config_readback
  config_type config_readback;
  EEPROM_read(0, config_readback);
  u8g.setFont(u8g_font_unifont);//font1
  u8g.drawStr( 0, 16, "Num:");
  u8g.setFont(u8g_font_7x13);//font2
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
  
  delay(2000);//delay 2s
  
}

