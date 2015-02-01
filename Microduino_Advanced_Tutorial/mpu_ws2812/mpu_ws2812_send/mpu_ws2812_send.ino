#include <I2Cdev.h>
#include <MPU6050.h>
#include <Wire.h>//添加必须的库文件
#include "mpu.h"
#include "nrf.h"

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

void setup() {
  Wire.begin();
  Serial.begin(115200);

  Wire.begin();

  accelgyro.initialize();

  SPI.begin();		//初始化SPI总线
  radio.begin();
  network.begin(/*channel*/ nRF_channal, /*node address*/ this_node);
}

void loop() {
  getMPU();
  nrf_send();
}

