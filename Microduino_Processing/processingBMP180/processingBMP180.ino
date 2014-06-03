#include "Wire.h"
#include "I2Cdev.h"
#include "BMP085.h"

BMP085 barometer;

float pressure;
int32_t lastMicros;


void setup ()
{
  Wire.begin();

  // initialize serial communication
  // it's really up to you depending on your project)
  Serial.begin(9600);

  // initialize device
  //Serial.println("Initializing I2C devices...");
  barometer.initialize();

  // verify connection
  //Serial.println("Testing device connections...");
  //Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");
  barometer.testConnection();
}

void loop()
{
  vobmp085();
}


void vobmp085()
{
  // request pressure (3x oversampling mode, high detail, 23.5ms delay)
  barometer.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
  // read calibrated pressure value in Pascals (Pa)
  pressure = barometer.getPressure();

  Serial.println(pressure/1000,3);
  
  delay(100);

}

