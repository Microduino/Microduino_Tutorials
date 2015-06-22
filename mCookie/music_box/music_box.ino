#include "audio.h"
#include "key.h"

#include <SoftwareSerial.h>

int music1_Pin = 4;
int music2_Pin = 6;

boolean play_pause;
boolean play_change;

int music_vol = 25; //初始音量0~30

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(music1_Pin, INPUT);
  pinMode(music2_Pin, INPUT);

  key_init();

  audio_pause();

  audio_init(DEVICE_Flash, MODE_loopOne, music_vol); //初始化mp3模块
}

void loop() {
  if (key_get(music1_Pin, 0))
  {
    delay(200);
    play_pause = !play_pause;
  }
  if (play_change != play_pause)
  {
    play_change = play_pause;
    if (play_pause)
      audio_play();
    else
      audio_pause();
  }
  if (key_get(music2_Pin, 0))
    audio_down();
}
