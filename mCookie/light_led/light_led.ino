#include <Adafruit_NeoPixel.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

#define Light_PIN A0

#define Light_value1 400
#define Light_value2 800

int sensorValue;

void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
  sensorValue = analogRead(Light_PIN);
  Serial.println(sensorValue);
  if (sensorValue < Light_value1)
    colorWipe(strip.Color(0, map(sensorValue, 10, 400, 0, 255), 0));
  else if (sensorValue >= Light_value1 && sensorValue < Light_value2)
    colorWipe(strip.Color(0, 0, map(sensorValue, 400, 800, 0, 255)));
  else if (Light_value2 >= 800)
    colorWipe(strip.Color(map(sensorValue, 800, 960, 0, 255), 0, 0));
}

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
