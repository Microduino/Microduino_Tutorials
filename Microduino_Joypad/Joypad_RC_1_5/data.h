#include "Arduino.h"

boolean node_STA=false;

byte inBuf[16]={
  0xDC,0x05,0xDC,0x05,0xD0,0x07,0xEF,0x03,0xDC,0x05,0xDC,0x05,0xDC,0x05,0xDC,0x05
};

int outBuf[8]={
  Joy_MID,Joy_MID,Joy_MID,Joy_MID,Joy_MID,Joy_MID,Joy_MID,Joy_MID};


boolean AUX3=false,AUX4=false;
boolean AUX3_STA=false,AUX4_STA=false;
boolean AUX3_STA_C=false,AUX4_STA_C=false;
//======================================
void data_begin()
{
  Joy();

  if(mode)    //Robot
  {
    node_STA=!Joy1_sw;
    
    if(!sw_l)
    {
      Joy_x=Joy_MID;
      Joy_y=Joy_MID;
      Joy1_x=Joy_MID;
      Joy1_y=Joy_MID;
    }
  }
  else        //QuadCopter
  {
    if(!sw_l)
      Joy_y=Joy_MID-Joy_maximum;
  }


  //but---------------------------------
  AUX3_STA=but3;
  if(AUX3_STA && AUX3_STA_C)		//按下松开后
  {
    AUX3=!AUX3;
    delay(5);
  }
  AUX3_STA_C=!AUX3_STA;		//缓存作判断用

  //but---------------------------------
  AUX4_STA=but4;
  if(AUX4_STA && AUX4_STA_C)		//按下松开后
  {
    AUX4=!AUX4;
    delay(5);
  }
  AUX4_STA_C=!AUX4_STA;		//缓存作判断用

  outBuf[0]=Joy1_x;
  outBuf[1]=Joy1_y;
  outBuf[2]=Joy_x;
  outBuf[3]=Joy_y;
  outBuf[4]=map(sw_l,0,1,Joy_MID+Joy_maximum,Joy_MID-Joy_maximum);
  outBuf[5]=map(sw_r,0,1,Joy_MID+Joy_maximum,Joy_MID-Joy_maximum);
  outBuf[6]=map(AUX3,0,1,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
  outBuf[7]=map(AUX4,0,1,Joy_MID-Joy_maximum,Joy_MID+Joy_maximum);
}

