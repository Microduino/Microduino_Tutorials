#include "Arduino.h"

//输入设备-------------------
//#include <Joypad.h>

//键值定义-------------------
#define KEY_NONE 0
#define KEY_PREV 1
#define KEY_NEXT 2
#define KEY_SELECT 3
#define KEY_BACK 4

uint8_t uiKeyCodeFirst = KEY_NONE;
uint8_t uiKeyCodeSecond = KEY_NONE;
uint8_t uiKeyCode = KEY_NONE;
uint8_t last_key_code = KEY_NONE;

//检测输入动作---------------
void uiStep(void) {
  uiKeyCodeSecond = uiKeyCodeFirst;
  /*
  if (!digitalRead(3))
   uiKeyCodeFirst = KEY_PREV;
   else if (!digitalRead(4))
   uiKeyCodeFirst = KEY_NEXT;
   else if (!digitalRead(5))
   uiKeyCodeFirst = KEY_BACK;
   else if (!digitalRead(6))
   uiKeyCodeFirst = KEY_SELECT;
   */
  if (analogRead(A6)>300 && analogRead(A6)<400)
    uiKeyCodeFirst = KEY_NEXT;
  else if (analogRead(A6)>200 && analogRead(A6)<300)
    uiKeyCodeFirst = KEY_PREV;
  else if (analogRead(A6)<100)
    uiKeyCodeFirst = KEY_SELECT;
  else 
    uiKeyCodeFirst = KEY_NONE;

  if ( uiKeyCodeSecond == uiKeyCodeFirst )
    uiKeyCode = uiKeyCodeFirst;
  else
    uiKeyCode = KEY_NONE;
}



