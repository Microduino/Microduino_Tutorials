#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
  
MPU6050 accelgyro;
  
int16_t ax, ay, az;
int16_t gx, gy, gz;
  
char str[512];
  
void setup() {
    Wire.begin();
    Serial.begin(9600);
    accelgyro.initialize();
}
  
void loop() {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    sprintf(str, "%d,%d,%d,%d,%d,%d,%d", ax, ay, az, gx, gy, gz); 
    Serial.print(str);
    Serial.write(byte(10));
    delay(20);
}
