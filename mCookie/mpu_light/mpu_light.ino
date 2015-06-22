#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>//添加必须的库文件
#include "mpu.h"

#include <Adafruit_NeoPixel.h>

#define PIN A0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

unsigned long safe_ms=millis();

void setup() 
{
  Serial.begin(115200);

  Wire.begin();
  accelgyro.initialize();

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  Serial.println("===========start===========");
}

void loop() 
{
  //===============================================================
  if(safe_ms>millis()) safe_ms=millis();
  if(millis()-safe_ms>3000)
  {
    safe_ms=millis();
    getMPU();
    Angel_accX_c=Angel_accX_send;
    Angel_accY_c=Angel_accY_send;
    Angel_accZ_c=Angel_accZ_send;
  }
  else
  {
    getMPU();
    if(abs(Angel_accX_send-Angel_accX_c)<2&&abs(Angel_accY_send-Angel_accY_c)<2&&abs(Angel_accZ_send-Angel_accZ_c)<2)
    {
      colorWipe(strip.Color(0, 0, 0), 50);
    }
    else
      colorWipe(strip.Color(Angel_accX_send, Angel_accY_send, Angel_accZ_send));
  }
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}








