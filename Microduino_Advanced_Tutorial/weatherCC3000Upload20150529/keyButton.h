#include "arduino.h"

boolean keyButton[NUM_DIGITAL_PINS];			//按键
boolean keyButton_cache[NUM_DIGITAL_PINS];		//检测按键松开缓存

void keyButton_init()
{
  for(int a=0;a<NUM_DIGITAL_PINS;a++)
  {
    keyButton[a]=LOW;
    keyButton_cache[a]=HIGH;
  }
}

boolean keyButton_get(int _keyButton_pin,boolean _keyButton_type)
{
  keyButton_cache[_keyButton_pin]=keyButton[_keyButton_pin];		//缓存作判断用

  keyButton[_keyButton_pin]=!digitalRead(_keyButton_pin);	//触发时

  switch(_keyButton_type)
  {
  case 0:
    if(!keyButton[_keyButton_pin] && keyButton_cache[_keyButton_pin])		//按下松开后
      return true;
    else
      return false;
    break;
  case 1:
    if(keyButton[_keyButton_pin] && !keyButton_cache[_keyButton_pin])		//按下松开后
      return true;
    else
      return false;
    break;
  }
}
