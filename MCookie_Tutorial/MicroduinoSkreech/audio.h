#include "arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3); // RX, TX

//#define AUDIO_PORT Serial1    //Core+
#define AUDIO_PORT mySerial  //Core

byte sn_reset[4]=
{
  0x7E,0x02,0x0C,0xEF
};

byte sn_choose[6]=
{
  0x7E,0x04,0x03,0x00,0x01,0xEF
};

byte sn_vol[5]=
{
  0x7E,0x03,0x06,0x18,0xEF
};

byte sn_device[5]=
{
  0x7E,0x03,0x09,0x01,0xEF  
};

byte sn_pause[4]=
{
  0x7E,0x02,0x0E,0xEF
};

byte sn_play[4]=
{
  0x7E,0x02,0x0D,0xEF
};

byte sn_mode[5]=
{
  0x7E,0x03,0x11,0x00,0xEF
};

byte sn_down[4]=
{
  0x7E,0x02,0x01,0xEF
};

byte sn_up[4]=
{
  0x7E,0x02,0x02,0xEF
};

byte sn_eq[5]=
{
  0x7E,0x03,0x07,0x01,0xEF
};

//-----------------------------
void audio_pause()
{
  AUDIO_PORT.write(sn_pause,4);
  delay(50);
}

void audio_play()
{
  AUDIO_PORT.write(sn_play,4);
  delay(50);
}

//play eq    (Normal/Pop/Rock/Jazz/Classic/Base)  0-5
void audio_eq(byte _audio_eq)
{
  sn_mode[3]=_audio_eq;
  AUDIO_PORT.write(sn_eq,5);
  delay(100);
}

#define MODE_loopAll 0
#define MODE_loopOne 1
#define MODE_One_STOP 2
#define MODE_One_END  4
//play mode    (ALL/FOL/ONE/RAM/ONE_STOP)  0-4
void audio_mode(byte _audio_mode)
{
  sn_mode[3]=_audio_mode;
  AUDIO_PORT.write(sn_mode,5);
  delay(100);
}

#define DEVICE_Flash  5
#define DEVICE_TF  1
//device select    (U/TF/AUX/SLEEP/FLASH)  0-4
void audio_device(byte _audio_device)
{
  sn_device[3]=_audio_device;
  AUDIO_PORT.write(sn_device,5);
  delay(1500);
}

void audio_down()
{
  AUDIO_PORT.write(sn_down,4);
  delay(500);
}

void audio_up()
{
  AUDIO_PORT.write(sn_up,4);
  delay(500);
}

void audio_vol(byte _audio_vol)
{
  sn_vol[3]=_audio_vol;
  AUDIO_PORT.write(sn_vol,5);
  delay(50);
}

void audio_choose(byte _audio_choose)
{
  sn_choose[4]=_audio_choose;
  AUDIO_PORT.write(sn_choose,6);
  delay(100);
}

void audio_reset()
{
  AUDIO_PORT.write(sn_reset,4);
  delay(500);
}

void audio_init(int _audio_init_device,int _audio_init_mode,int _audio_init_vol)
{
  AUDIO_PORT.begin(9600);
  delay(500);
  audio_reset();
  audio_device(_audio_init_device);
  audio_mode(_audio_init_mode);
  audio_vol(_audio_init_vol);
  //audio_choose(1);
  //audio_pause();
}
