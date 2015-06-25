//1,屏幕显示部分=============================
#include "U8glib.h"
#include "keyButton.h"
//2,传感器部分================================
#include <Wire.h>
#include "I2Cdev.h"
//温湿度
#include <AM2321.h>
AM2321 am2321;

#include"AirQuality.h"
#include"Arduino.h"

#include <SoftwareSerial.h>

/*-------net------*/
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
//#include <ArduinoJson.h>


#include "def.h"
#include "oled.h"
#include "wifi.h"
#include "weather.h"


void setup(void) {
  //初始化串口波特率
  Serial.begin(115200);
  my_Serial.begin(2400);
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);

  setup_wifi();
  //initWeatherSensor();
  keyButton_init();
}

void loop(void) {


  //按钮采样时间间隔
  // if (button_time > millis()) button_time = millis();
  // if(millis() - button_time > INTERVAL_BUTTON) {
  updateButton();
  //  button_time = millis();
  // }

  //数据上传时间间隔
  if (net_time > millis()) net_time = millis();
  if (millis() - net_time > INTERVAL_NET) {
    updateWeatherData();
    net_time = millis();
  }

  //传感器采样时间间隔
  if (sensor_time > millis()) sensor_time = millis();
  if (millis() - sensor_time > INTERVAL_SENSOR) {
    updateSensorData();
    sensor_time = millis();
  }

  if (lcd_time > millis()) lcd_time = millis();
  if (millis() - lcd_time > INTERVAL_LCD) {
    volcd();                       //调用显示库
    lcd_time = millis();
  }
}



