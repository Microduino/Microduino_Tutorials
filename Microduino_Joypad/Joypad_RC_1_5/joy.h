#include "Arduino.h"

#include <Joypad.h>

int mode;    //0 is mwc, 1 is nrf

int joy_correct_max[4],joy_correct_min[4];
int Joy_x,Joy_y,Joy1_x,Joy1_y;

int s_lig,s_mic;

boolean Joy_sw,Joy1_sw;

boolean but1,but2,but3,but4;

boolean sw_l,sw_r;

//======================================
int Joy_dead_zone(int _Joy_vol)
{
  int C=_Joy_vol;
  int Joy_vol=0;
  if(abs(C)>JOYSTICK_DEAD_ZONE)
  {
    if (C>0)
      Joy_vol = C-JOYSTICK_DEAD_ZONE;
    else
      Joy_vol = C+JOYSTICK_DEAD_ZONE;
  }
  return Joy_vol;
}

int Joy_i(int _Joy_i,boolean _Joy_b,int _Joy_MIN,int _Joy_MAX)
{
  int _Joy_a;
  switch(_Joy_i)
  {
  case 0:
    _Joy_a=Joy_dead_zone(Joypad.readJoystickX());
    break;
  case 1:
    _Joy_a=Joy_dead_zone(Joypad.readJoystickY());
    break;
  case 2:
    _Joy_a=Joy_dead_zone(Joypad.readJoystick1X());
    break;
  case 3:
    _Joy_a=Joy_dead_zone(Joypad.readJoystick1Y());
    break;
  }

  if(_Joy_b)
  {
    if(_Joy_a<0)
      _Joy_a=map(_Joy_a, joy_correct_min[_Joy_i],0, _Joy_MAX, Joy_MID);
    else
      _Joy_a=map(_Joy_a, 0,joy_correct_max[_Joy_i], Joy_MID, _Joy_MIN);

    if(_Joy_a<_Joy_MIN) _Joy_a=_Joy_MIN;
    if(_Joy_a>_Joy_MAX) _Joy_a=_Joy_MAX;
  }
  return _Joy_a;
}

void Joy()
{
  sw_l=Joypad.readButton(CH_SWITCH_L);
  sw_r=Joypad.readButton(CH_SWITCH_R);

  //------------------------------------
  //s_lig=Joypad.readLightSensor();
  //s_mic=Joypad.readMicrophone();

  //------------------------------------
  Joy_sw=Joypad.readButton(CH_JOYSTICK_SW);
  Joy1_sw=Joypad.readButton(CH_JOYSTICK1_SW);

  //------------------------------------
  but1=Joypad.readButton(CH_SWITCH_1);
  but2=Joypad.readButton(CH_SWITCH_2);
  but3=Joypad.readButton(CH_SWITCH_3);
  but4=Joypad.readButton(CH_SWITCH_4);

  //====================================
  if(Joypad.readButton(CH_SWITCH_R))
  {
    Joy_x=Joy_i(0,true,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
    Joy_y=Joy_i(1,true,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
    Joy1_x=Joy_i(2,true,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
    Joy1_y=Joy_i(3,true,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
  }
  else
  {
    Joy_x=Joy_i(0,true,Joy_MID-Joy_s_maximum,Joy_MID+Joy_s_maximum);
    Joy_y=Joy_i(1,true,mode ? Joy_MID-Joy_s_maximum : Joy_MID-Joy_maximum,mode ? Joy_MID+Joy_s_maximum : Joy_MID+Joy_maximum);//  Robot,QuadCopter
    Joy1_x=Joy_i(2,true,Joy_MID-Joy_s_maximum,Joy_MID+Joy_s_maximum);
    Joy1_y=Joy_i(3,true,Joy_MID-Joy_s_maximum,Joy_MID+Joy_s_maximum);
  }
}




