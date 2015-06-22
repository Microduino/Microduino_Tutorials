//传感器部分================================
#include <Wire.h>
#include "I2Cdev.h"
//温湿度
#include <AM2321.h>
AM2321 am2321;

#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); // RX, TX

//#define Serial_com mySerial
#define Serial_com Serial1

float sensor_tem, sensor_hum, sensor_lux, sensor_light;

String dataToSend;
char buf[10];

void setup() {
  Serial.begin(9600);

  Serial_com.begin(9600);
}

void loop() {
  //获取温湿度==============================================
  am2321.read();
  sensor_tem = am2321.temperature / 10.0;
  sensor_hum = am2321.humidity / 10.0;

  sensor_light = map(analogRead(A0), 0, 1023, 0, 255);

  dtostrf(sensor_tem, 1, 1, buf);
  dataToSend = "{T:" + String(buf) + "}";
  Serial_com.print(dataToSend);
  Serial.println(dataToSend);
  delay(1000);

  dtostrf(sensor_hum, 1, 1, buf);
  dataToSend = "{H:" + String(buf) + "}";
  Serial_com.print(dataToSend);
  Serial.println(dataToSend);
  delay(1000);

  dtostrf(sensor_light, 1, 1, buf);
  dataToSend = "{L:" + String(buf) + "}";
  Serial_com.print(dataToSend);
  Serial.println(dataToSend);
  delay(1000);

  dataToSend = "";
}
