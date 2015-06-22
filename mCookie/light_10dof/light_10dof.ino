#include "Wire.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

#include <Adafruit_NeoPixel.h>
#define PIN 4
Adafruit_NeoPixel strip = Adafruit_NeoPixel(12, PIN, NEO_GRB + NEO_KHZ800);

int16_t ax, ay, az;
int16_t gx, gy, gz;

int data_gz;
int num;

uint32_t color[9]=
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0),strip.Color(0, 0, 255),strip.Color(0, 255, 0),strip.Color(255, 0, 255),strip.Color(241, 241, 40),strip.Color(250, 121, 46),strip.Color(0, 255, 237),strip.Color(255, 255, 255)
  };

  void setup()
  {
    Serial.begin(115200);

    // join I2C bus (I2Cdev library doesn't do this automatically)
    Wire.begin();

    // initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");

    strip.begin();	//初始化LED
    strip.show(); // Initialize all pixels to 'off'

    for(int i=0;i<9;i++)
    {
      colorWipe(color[i]);
      delay(300);
    }

    colorWipe(color[0]);
  }

void loop()
{
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
  /*
  Serial.print("a/g:\t");
   Serial.print(ax/180); 
   Serial.print("\t");
   Serial.print(ay/180); 
   Serial.print("\t");
   Serial.print(az/180); 
   Serial.print("\t");
   Serial.print(gx/180); 
   Serial.print("\t");
   Serial.print(gy/180); 
   Serial.print("\t");
   Serial.println(gz/180);
   */
  data_gz=gz/180;

  if(abs(data_gz)>150)
  {
    num=random(0, 10);
    colorWipe(color[num]);
  }
}

void colorWipe(uint32_t c) {
  for(uint16_t i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}
