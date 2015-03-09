#include "U8glib.h"
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

#include "def.h"
#include "oled.h"
#include "wifi.h"

#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

unsigned long lcd_time = millis(); //OLED刷新时间计时器 

void setup() {

  Serial.begin(115200);

  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  setup_wifi();

  colorSet(strip.Color(255, 0, 0));
  delay(200);
  colorSet(strip.Color(0, 255, 0));
  delay(200);
  colorSet(strip.Color(0, 0, 255));
  delay(200);
  colorSet(strip.Color(255, 0, 255));
  delay(200);
}

void loop() {
  wifi_data();

  colorSet(strip.Color(red, green, blue));

  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD) {
    volcd(); //调用显示库
    lcd_time = millis();
  }
}

void colorSet(uint32_t c) {
  // Serial.println(c);
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}

