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

byte sn_vol_up[4] ={
  0x7E,0x02,0x04,0xEF};//vol+
byte sn_vol_down[4] ={
  0x7E,0x02,0x05,0xEF};//vol-
//-----------------------------
void audio_pause()
{
  Serial.write(sn_pause,4);
  delay(50);
}

void audio_play()
{
  Serial.write(sn_play,4);
  delay(50);
}

//play mode     "0:All loop ,1:One loop
void audio_mode(byte _audio_mode)
{
  sn_mode[3]=_audio_mode;
  Serial.write(sn_mode,5);
  delay(100);
}

//device select "tf"   U/TF/AUX/SLEEP/FLASH      00-05
void audio_device(byte _audio_device)
{
  sn_device[3]=_audio_device;
  Serial.write(sn_device,5);
  delay(1500);
}

void audio_vol(byte _audio_vol)
{
  sn_vol[3]=_audio_vol;
  Serial.write(sn_vol,5);
  delay(50);
}

void audio_choose(byte _audio_choose)
{
  sn_choose[4]=_audio_choose;
  Serial.write(sn_choose,6);
  delay(100);
}

void audio_reset()
{
  Serial.write(sn_reset,4);
  delay(500);
}

void audio_up()
{
  Serial.write(sn_up,4);
  delay(500);
}

void audio_down()
{
  Serial.write(sn_down,4);
  delay(500);
}

void audio_vol_up()
{
  Serial.write(sn_vol_up,4);
  delay(500);
}

void audio_vol_down()
{
  Serial.write(sn_vol_down,4);
  delay(500);
}

