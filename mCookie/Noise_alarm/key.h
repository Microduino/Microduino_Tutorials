#include "arduino.h"

boolean key[NUM_DIGITAL_PINS];			//按键
boolean key_cache[NUM_DIGITAL_PINS];		//检测按键松开缓存

void key_init()
{
  for (int a = 0; a < NUM_DIGITAL_PINS; a++)
  {
    key[a] = LOW;
    key_cache[a] = HIGH;
  }
}

boolean key_get(int _key_pin, boolean _key_type)
{
  key_cache[_key_pin] = key[_key_pin];		//缓存作判断用

  key[_key_pin] = !digitalRead(_key_pin);	//触发时

  switch (_key_type)
  {
    case 0:
      if (!key[_key_pin] && key_cache[_key_pin])
        return true;
      else
        return false;
      break;
    case 1:
      if (key[_key_pin] && !key_cache[_key_pin])
        return true;
      else
        return false;
      break;
  }
}
