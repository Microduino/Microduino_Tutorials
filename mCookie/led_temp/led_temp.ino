#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

//温湿度   
#include <AM2321.h>                             //调用库  
AM2321 am2321;   

int sensor_tem,sensor_hum;

void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");

  matrix1.begin(0x71);
  matrix2.begin(0x70);

  matrix3.begin(0x73);
  matrix4.begin(0x72);
}

void loop() {
  am2321.read();   
  sensor_tem = am2321.temperature / 10;   
  sensor_hum = am2321.humidity / 10;   

  lcdtemphun(sensor_tem,1);
  lcdtemphun(sensor_hum,2);
  
  delay(1000);
}

void lcdtemphun(int t,int num)
{
  switch(num)
  {
  case 1:
    {
      matrix1.setTextSize(1);
      matrix1.setTextWrap(false);
      matrix1.setRotation(0);
      matrix1.clear();
      matrix1.setTextColor(LED_ON);
      matrix1.setCursor(0,0);
      matrix1.print(t/10);
      matrix1.setCursor(6,0);
      matrix1.print(t%10);
      matrix1.writeDisplay();
      delay(1);
      matrix2.setTextSize(1);
      matrix2.setTextWrap(false); 
      matrix2.setRotation(0);
      matrix2.clear();
      matrix2.setTextColor(LED_ON);
      matrix2.setCursor(-2,0);
      matrix2.print(t%10);
      matrix2.drawRect(5,0, 3,3, LED_ON);
      matrix2.writeDisplay();  
      delay(1);
    }
    break;
  case 2:
    {
      matrix3.setTextSize(1);
      matrix3.setTextWrap(false);
      matrix3.setRotation(0);
      matrix3.clear();
      matrix3.setTextColor(LED_ON);
      matrix3.setCursor(0,0);
      matrix3.print(t/10);
      matrix3.setCursor(6,0);
      matrix3.print(t%10);
      matrix3.writeDisplay();
      delay(1);
      matrix4.setTextSize(1);
      matrix4.setTextWrap(false); 
      matrix4.setRotation(0);
      matrix4.clear();
      matrix4.setTextColor(LED_ON);
      matrix4.setCursor(-2,0);
      matrix4.print(t%10);
      matrix4.drawRect(5,0, 2,2, LED_ON);
      matrix4.drawRect(6,6, 2,2, LED_ON);
      matrix4.drawLine(7,2, 5,5, LED_ON);
      matrix4.writeDisplay();  
      delay(1);
    }
    break;
  }
}
