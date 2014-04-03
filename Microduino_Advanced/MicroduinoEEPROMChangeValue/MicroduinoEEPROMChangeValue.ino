#include <IRremote.h>
#include <avr/eeprom.h>
#include "U8glib.h"
#define EEPROM_write(address, var) eeprom_write_block((const void *)&(var), (void *)(address), sizeof(var))
#define EEPROM_read(address, var) eeprom_read_block((void *)&(var), (const void *)(address), sizeof(var))

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//The definition OLED connection

int action;//record remote control button
int t;//inital value
int RECV_PIN = 11;//define IR receiver pin 11
IRrecv irrecv(RECV_PIN);
decode_results results;

// define a struct for record data
struct config_type
{
  int num;
};


void draw(int changeNum) {

  config_type config;
  config.num = changeNum;
  // 2) record config data to EEPROM, address is 0
  EEPROM_write(0, config);
  // 3) read from address 0 of EEPROM, result copy to config_readback
  config_type config_readback;
  EEPROM_read(0, config_readback);
  u8g.setFont(u8g_font_unifont);//font1
  u8g.drawStr( 0, 16, "Num:");
  u8g.setFont(u8g_font_7x13);//font2
  u8g.setPrintPos(0, 32);
  u8g.print(config_readback.num);

}  


void showNum() {
  config_type config_readback;
  EEPROM_read(0, config_readback);
  u8g.setFont(u8g_font_unifont);//font1
  u8g.drawStr( 0, 16, "Num:");
  u8g.setFont(u8g_font_7x13);//font2
  u8g.setPrintPos(0, 32);
  u8g.print(config_readback.num);
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

void setup()
{
  irrecv.enableIRIn(); // inital IR receiver
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



