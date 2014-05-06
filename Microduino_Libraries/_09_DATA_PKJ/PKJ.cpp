#include "Arduino.h"
#include "PKJ.h"
 
int PKJ::VOLxianzhi(int _num,int _volL,int _volH)
{
	if (_num>_volH)
	_num=_volH;
	if(_num<_volL)
	_num=_volL;
	return _num;
}

int PKJ::VOLtiaobian(int _num,int _volL,int _volH)
{
	if (_num>_volH)
	_num=_volL;
	if(_num<_volL)
	_num=_volH;
	return _num;
}

void PKJ::TIMERdelayBEGIN(unsigned char _RUNmax)
{
  RUNmax=_RUNmax+8;
  for(int a=0;a<RUNmax;a++)
  {
    RUNmillis[a]=millis();
  }
}

boolean PKJ::TIMERdelay(unsigned char _RUNId,  unsigned long _RUNDelay)
{
  if(_RUNId+8<RUNmax)
  {
    if(millis()-RUNmillis[_RUNId+8]>_RUNDelay)
    {
      RUNmillis[_RUNId+8]=millis();
      return true;
    }
    else
    {
      return false;
    }
  }
}
