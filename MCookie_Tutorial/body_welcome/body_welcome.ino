#include "audio.h"
#include "key.h"

#include <Adafruit_NeoPixel.h>
#define PIN A0
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

#include <SoftwareSerial.h>

uint32_t color[9] =
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0), strip.Color(248, 141, 30), strip.Color(255, 255, 0), strip.Color(0, 255, 0), strip.Color(0, 127, 255), strip.Color(0, 0, 255), strip.Color(139, 0, 255), strip.Color(255, 255, 255)
};

#define body_pin 4

int i = 1;
int music_vol = 28; //初始音量0~30

boolean play_pause;
boolean play_stop;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  pinMode(body_pin, INPUT);

  key_init();

  audio_init(DEVICE_Flash, MODE_One_END, music_vol);

  strip.begin();	//初始化LED
  strip.show(); // Initialize all pixels to 'off'

  for (int i = 0; i < 9; i++)
  {
    colorWipe(color[i]);
    delay(300);
  }
  colorWipe(color[0]);
}

void loop() {
  if (key_get(body_pin, 0))
  {
    if (i > 3)
      i = 1;
    colorWipe(color[random(1, 10)]);
    audio_choose(i);
    i = i + 1;
  }
  Serial.println(digitalRead(body_pin));
  if (!digitalRead(body_pin))
    colorWipe(color[0]);
}

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}


