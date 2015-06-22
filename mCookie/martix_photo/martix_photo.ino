#include "Wire.h"

#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"

Adafruit_8x8matrix matrix1 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix2 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix3 = Adafruit_8x8matrix();
Adafruit_8x8matrix matrix4 = Adafruit_8x8matrix();

static uint8_t PROGMEM
love0_bmp[] ={
  0x00,0x66,0xFF,0xFF,0xFF,0x7E,0x3C,0x18}
,
love1_bmp[] ={
  0x00,0x66,0x9D,0x81,0x81,0x42,0x24,0x18}
,
//3-1-4-2
love_1_1_bmp[] ={
  0xFF,0xFF,0xFE,0xFC,0xF8,0xF0,0xE0,0xC0}
,
love_2_1_bmp[] ={
  0xFF,0xFF,0x7F,0x3F,0x1F,0x0F,0x07,0x03}
,
love_3_1_bmp[] ={
  0x00,0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0xFF}
,
love_4_1_bmp[] ={
  0x00,0x3C,0x7E,0xFF,0xFF,0xFF,0xFF,0xFF}
,
love_1_2_bmp[] ={
  0x0F,0x0F,0x1E,0x3C,0x78,0xF0,0xE0,0xC0}
,
love_2_2_bmp[] ={
  0xF0,0xF0,0x78,0x3C,0x1E,0x0F,0x07,0x03}
,
love_3_2_bmp[] ={
  0x00,0x3C,0x7E,0xFF,0xFF,0xFF,0x9F,0x0F}
,
love_4_2_bmp[] ={
  0x00,0x3C,0x7E,0xFF,0xFF,0xFF,0xF9,0xF0}
,
logo_1_bmp[] ={
  0x8C,0x8C,0x8C,0x8C,0x8C,0x00,0x00,0x00}
,
logo_2_bmp[] ={
  0x31,0x31,0x31,0x31,0x31,0x00,0x00,0x00}
,
logo_3_bmp[] ={
  0x00,0x00,0x00,0x70,0x70,0x8C,0x8C,0x8C}
,
logo_4_bmp[] ={
  0x00,0x00,0x00,0x0E,0x0E,0x31,0x31,0x31}
,
temp_bmp[] ={
  0x1C,0x18,0x1C,0x18,0x18,0x3E,0x3E,0x1C}
,
hum_bmp[] ={
  0x10,0x18,0x3C,0x7E,0x7E,0x3C,0x18,0x00};

//温湿度   
#include <AM2321.h>                             //调用库  
AM2321 am2321;   

int sensor_tem,sensor_hum;

int16_t ax, ay, az;
int16_t gx, gy, gz;

void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");

  Wire.begin();

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");


  matrix1.begin(0x71);
  matrix2.begin(0x70);

  matrix3.begin(0x73);
  matrix4.begin(0x72);
}

void loop() {

  am2321.read();   
  sensor_tem = am2321.temperature / 10;   
  sensor_hum = am2321.humidity / 10;   

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
   
   
   if(-10<ax/180&&ax/180<10 && -80>ay/180&&ay/180>=-90 && -10<az/180&&az/180<10 )
   lcdtemphun(sensor_tem,1);
   if(-10<ay/180&&ay/180<10 && -80>ax/180&&ax/180>=-90 && -10<az/180&&az/180<10 )
   lcdtemphun(sensor_hum,2);
   if(-10<ax/180&&ax/180<10 && 80<ay/180&&ay/180<=90 && -10<az/180&&az/180<10 )
   love();
   if(-10<ay/180&&ay/180<10 && 80<ax/180&&ax/180<=90 && -10<az/180&&az/180<10 )
   logo();
   */
  lcdtemphun(sensor_tem,2);
  delay(2000);
  lcdtemphun(sensor_hum,1);
  delay(2000);
  love();
  delay(2000);
  logo();
  delay(2000);
}

void lcdtemphun(int t,int num)
{
  matrix2.setTextSize(1);
  matrix2.setTextWrap(false);
  matrix2.setRotation(0);
  matrix2.clear();
  matrix2.setTextColor(LED_ON);
  matrix2.setCursor(0,0);
  matrix2.print(t/10);
  matrix2.setCursor(6,0);
  matrix2.print(t%10);
  matrix2.writeDisplay();
  delay(1);
  matrix1.setTextSize(1);
  matrix1.setTextWrap(false); 
  matrix1.setRotation(0);
  matrix1.clear();
  matrix1.setTextColor(LED_ON);
  matrix1.setCursor(-2,0);
  matrix1.print(t%10);
  switch(num)
  {
  case 1:
    {
      matrix1.drawRect(5,0, 2,2, LED_ON);
      matrix1.drawRect(6,6, 2,2, LED_ON);
      matrix1.drawLine(7,2, 5,5, LED_ON);
    }
    break;
  case 2:
    matrix1.drawRect(5,0, 3,3, LED_ON);
    break; 
  }
  matrix1.writeDisplay(); 
  delay(1);

  matrix4.clear();
  switch(num)
  {
  case 1:
    matrix4.drawBitmap(0, 0, hum_bmp, 8, 8, LED_ON);
    break;
  case 2:
    matrix4.drawBitmap(0, 0, temp_bmp, 8, 8, LED_ON);
    break; 
  }
  matrix4.writeDisplay();
  delay(1);
  matrix3.clear();
  matrix3.writeDisplay();
  delay(1);
}

void love()
{
  for(int i=0;i<5;i+=1)
  {
    matrix1.clear();
    if(i%2==0)
      matrix1.drawBitmap(0, 0, love_1_1_bmp, 8, 8, LED_ON);
    else
      matrix1.drawBitmap(0, 0, love_1_2_bmp, 8, 8, LED_ON);
    matrix1.writeDisplay();
    delay(1);
    matrix2.clear();
    if(i%2==0)
      matrix2.drawBitmap(0, 0, love_2_1_bmp, 8, 8, LED_ON);
    else
      matrix2.drawBitmap(0, 0, love_2_2_bmp, 8, 8, LED_ON);
    matrix2.writeDisplay();
    delay(1);
    matrix3.clear();
    if(i%2==0)
      matrix3.drawBitmap(0, 0, love_3_1_bmp, 8, 8, LED_ON);
    else
      matrix3.drawBitmap(0, 0, love_3_2_bmp, 8, 8, LED_ON);
    matrix3.writeDisplay();
    delay(1);
    matrix4.clear();
    if(i%2==0)
      matrix4.drawBitmap(0, 0, love_4_1_bmp, 8, 8, LED_ON);
    else
      matrix4.drawBitmap(0, 0, love_4_2_bmp, 8, 8, LED_ON);
    matrix4.writeDisplay();
    delay(1);
    delay(1000);
  }
}

void logo()
{
  for(int i=0;i<5;i+=1)
  {
    matrix1.clear();
    if(i%2==0)
      matrix1.drawBitmap(0, 0, logo_1_bmp, 8, 8, LED_ON);
    else
      matrix1.clear();
    matrix1.writeDisplay();
    delay(1);
    matrix2.clear();
    if(i%2==0)
      matrix2.drawBitmap(0, 0, logo_2_bmp, 8, 8, LED_ON);
    else
      matrix2.clear();
    matrix2.writeDisplay();
    delay(1);
    matrix3.clear();
    if(i%2==0)
      matrix3.drawBitmap(0, 0, logo_3_bmp, 8, 8, LED_ON);
    else
      matrix3.clear();
    matrix3.writeDisplay();
    delay(1);
    matrix4.clear();
    if(i%2==0)
      matrix4.drawBitmap(0, 0, logo_4_bmp, 8, 8, LED_ON);
    else
      matrix4.clear();
    matrix4.writeDisplay();
    delay(1);
    delay(1000);
  }
}
