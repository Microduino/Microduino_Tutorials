#include "arduino.h"

boolean key1;			//按键
boolean key1_cache;		//检测按键松开缓存

boolean vokey(int _vokey)
{
  key1=digitalRead(_vokey);

  if(key1 && key1_cache)		//按下松开后
  {
    key1_cache=!key1;		//缓存作判断用
    return true;
  }
  else
  {
    key1_cache=!key1;		//缓存作判断用
    return false;
  }
}
