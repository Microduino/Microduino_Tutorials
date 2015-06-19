#include "audio.h"
#include <SoftwareSerial.h>
#include "key.h"

#define humanHotSensor 10
#define buttonPin 6
#define val_max 4
#define val_min 1
#define INTERVAL_LCD 10000 //定义播报身体健康状态时间间隔 
unsigned long lcd_time = millis(); //刷新时间计时器
int val=0;
int music_vol=26;  //初始音量0~30
bool humanHotState=false;
boolean on_off;
boolean statusChange;


void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT);
  pinMode(humanHotSensor, INPUT);
  audio_init(DEVICE_Flash,4,music_vol);    //初始化mp3模块
  key_init();
}

void loop() {
  updateButton();
  pirDetector();
}


void updateButton() {
  if(key_get(buttonPin,0)) {
    delay(200);
    on_off=!on_off;
  }
  if(statusChange!=on_off) {
    statusChange=on_off;
    if(on_off) {
      Serial.println("pause");
      audio_pause();
    } 
    else {
      Serial.println("pause");
      audio_pause();
    }
  }
}


void pirDetector() {
  // read the input pin:
  humanHotState = digitalRead(humanHotSensor);
  // print out the state of the button:
  //Serial.println(humanHotState);
  delay(1);        // delay in between reads for stability

  if(humanHotState) {
      Serial.println("play sound.....");
      audio_choose(1);// 2:health,1:noHealth
      audio_play();
  }
}