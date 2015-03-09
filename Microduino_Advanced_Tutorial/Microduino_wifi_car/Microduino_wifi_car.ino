#include "U8glib.h"
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

#include "def.h"
#include "olde.h"
#include "wifi.h"
#include "robot.h"

unsigned long lcd_time = millis(); //OLED刷新时间计时器 

void setup() {

  Serial.begin(115200);
  pinMode(motor_pin0A,OUTPUT);
  pinMode(motor_pin0B,OUTPUT);
  pinMode(motor_pin1A,OUTPUT);
  pinMode(motor_pin1B,OUTPUT);

  setup_wifi();

}

void loop() {

  wifi_data();
  
  vorobot();
  
  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD) {
    volcd(); //调用显示库
    lcd_time = millis();
  }
}

