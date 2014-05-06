#include <PKJ.h>

PKJ pkj;

#define pin_left 6
#define pin_right 5
#define pin_up 10
#define pin_down 9

boolean vol_up,vol_down,vol_left,vol_right;


void setup()
{
  pinMode(pin_left, OUTPUT);
  pinMode(pin_right, OUTPUT);
  pinMode(pin_up, OUTPUT);
  pinMode(pin_down, OUTPUT);

  pkj.TIMERdelayBEGIN(5);
}

void loop()
{
  if(pkj.TIMERdelay(0,200))
    led0();
  if(pkj.TIMERdelay(1,500))
    led1();
  if(pkj.TIMERdelay(2,900))
    led2();
  if(pkj.TIMERdelay(3,1500))
    led3();

  digitalWrite(pin_up, vol_up);
  digitalWrite(pin_down, vol_down);
  digitalWrite(pin_left, vol_left);
  digitalWrite(pin_right, vol_right);
}


void led0()
{
  vol_up=!vol_up;
}

void led1()
{
  vol_down=!vol_down;
}

void led2()
{
  vol_left=!vol_left;
}

void led3()
{
  vol_right=!vol_right;
}