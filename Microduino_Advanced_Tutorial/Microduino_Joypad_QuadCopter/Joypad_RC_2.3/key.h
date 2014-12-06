#include "arduino.h"

boolean key[4];			//按键
boolean key_cache[4];		//检测按键松开缓存

int key_pin[4]=PIN_key;

boolean vokey(int _vokey)
{
  _vokey--;
  key[_vokey]=Joypad.readButton(key_pin[_vokey]);

  if(key[_vokey] && key_cache[_vokey])		//按下松开后
  {
    key_cache[_vokey]=!key[_vokey];		//缓存作判断用
    return true;
  }
  else
  {
    key_cache[_vokey]=!key[_vokey];		//缓存作判断用
    return false;
  }
}
