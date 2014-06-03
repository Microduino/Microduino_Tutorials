#include "Wire.h"
#include "I2Cdev.h"
#include "HMC5883L.h"
HMC5883L mag;
int16_t mx, my, mz;

void setup ()
{
  // initialize serial communication
  // it's really up to you depending on your project)
  Serial.begin(9600);
  // initialize device
  //Serial.println("Initializing I2C devices...");
  mag.initialize();
  // verify connection
  //Serial.println("Testing device connections...");
  mag.testConnection();
  //Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");
}

void loop()
{
  mag.getHeading(&mx, &my, &mz);
  Serial.print(mx);
  Serial.print(",");
  Serial.println(my); 
}




