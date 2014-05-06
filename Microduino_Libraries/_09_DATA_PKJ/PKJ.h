#ifndef PKJ_h
#define PKJ_h
 
#include "Arduino.h"
 
class PKJ
{
  public:
    int VOLxianzhi(int,int,int);
    int VOLtiaobian(int,int,int);

    boolean TIMERdelay(unsigned char,unsigned long);
    void TIMERdelayBEGIN(unsigned char);
    unsigned char RUNmax;
	unsigned long RUNmillis[];

  private:
    int _num;
    int _volL;
    int _volH;

    unsigned char _RUNId;
    unsigned long _RUNDelay;
    unsigned char _RUNmax;
};

#endif