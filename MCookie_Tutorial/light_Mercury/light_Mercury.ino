#include "key.h"

#include <Adafruit_NeoPixel.h>
#define PIN A0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color[9]=
{
  strip.Color(255, 0, 0),strip.Color(248, 141, 30),strip.Color(255, 255, 0),strip.Color(0, 255, 0),strip.Color(0, 127, 255),strip.Color(0, 0, 255),strip.Color(0, 255, 236),strip.Color(255, 0, 199),strip.Color(0, 0, 0)
  };

  int light_Pin = 6;
int num;
boolean sta;

void setup() {
  // initialize serial:
  Serial.begin(9600);

  pinMode(light_Pin,INPUT_PULLUP);

  key_init();

  strip.begin();	//初始化LED
  strip.show(); // Initialize all pixels to 'off'

  for(int i=0;i<9;i++)
  {
    colorWipe(color[i]);
    delay(300);
  }
  colorWipe(color[random(1,10)]);
}

void loop() {
  if(key_get(light_Pin,0))
  {
    delay(300);
    num+=1;
    if(num>9)
    {
      sta=true;
      num=0;
    }
  }
  if(num<10&&num>0)
    colorWipe(color[num-1]);
  else
    rainbowCycle(1);
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++)
  {
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

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;
  if(sta)
  {
    for(j=0; j<255; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
      if(key_get(light_Pin,0))
      {
        sta=false;
        num=1;
      }
    }
  }
}


uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } 
  else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } 
  else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}



