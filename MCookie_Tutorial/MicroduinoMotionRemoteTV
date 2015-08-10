
#include "Wire.h"
#include <IRremote.h>

#include "I2Cdev.h"
#include "MPU6050.h"

#define ITEM_N  0x02FDD827  
#define ITEM_U  0x02FDF807
#define VOL_N  0x02FD58A7
#define VOL_U  0x02FD7887

MPU6050 accelgyro;
IRsend irsend;

int16_t ax, ay, az;
float Ax, Ay, Az;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();

    // verify connection
    Serial.println("Testing device connections...");
    Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
    // read raw accel/gyro measurements from device
    accelgyro.getAcceleration(&ax, &ay, &az);   // 
    Ax = ax/16384.00;
    Ay = ay/16384.00;
    Az = az/16384.00;
    
    if(Ay >= 0.6 )
    {
      irsend.sendNEC(ITEM_N,32); 
      delay(1000);
    }
    else if(Ay <= -0.6)
    {
      irsend.sendNEC(ITEM_U,32);   
      delay(1000);
    }
    else if(Az >= 1.5)
    {
      irsend.sendNEC(VOL_N,32);   
      delay(1000);
    }
    else if(Az <= 0.5)
    {
       irsend.sendNEC(VOL_U,32);  
       delay(1000);
    } 
    delay(100);
}
