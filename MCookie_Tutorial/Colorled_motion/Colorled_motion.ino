#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"

#include <Adafruit_NeoPixel.h>

#define PIN A0

Adafruit_NeoPixel strip = Adafruit_NeoPixel(20, PIN, NEO_GRB + NEO_KHZ800);

int16_t ax, ay, az, gx, gy, gz;
int16_t cx, cy, cz;

MPU6050 accelgyro;

unsigned long safe_ms;

#define timer 10000

void setup() {
  Serial.begin(115200);

  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

  strip.begin();
  strip.show();

}

void loop() {
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  ax = map(abs(ax / 180), 0, 100, 0, 255);
  ay = map(abs(ay / 180), 0, 100, 0, 255);
  az = map(abs(az / 180), 0, 100, 0, 255);

  if (ax - cx >= 10 || ay - cy >= 10 || az - cz >= 10)
  {
    cx = ax;
    cy = ay;
    cz = az;
    safe_ms = millis();
    colorWipe(strip.Color(ax, ay, az));
    /*
        Serial.print("color:\t");
        Serial.print(ax); Serial.print("\t");
        Serial.print(ay); Serial.print("\t");
        Serial.print(az); Serial.println("\t");

        */
  }
  else if (millis() - safe_ms > timer)
  {
    colorWipe(strip.Color(0, 0, 0));
    safe_ms = millis();
  }
}

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
