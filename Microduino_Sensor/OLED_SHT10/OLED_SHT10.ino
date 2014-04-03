#include "U8glib.h"
#include <SHT1x.h>


// Specify data and clock connections and instantiate SHT1x object
#define dataPin  5
#define clockPin 2
SHT1x sht1x(dataPin, clockPin);

float temperature=0.0;
float humidity=0.0;

#define DHT11_PIN 2//Sensor data interface to the digital pin2

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//show speed
void draw() {
  u8g.setFont(u8g_font_courB14);//font
  u8g.setPrintPos(0, 20);
  u8g.print("Temp:");
  u8g.print(temperature);
  u8g.print("`c");
  u8g.setPrintPos(0, 54);
  u8g.print("AH:");
  u8g.print(humidity);
  u8g.print("%");
}  


void setup()
{
}
 
 
void loop()
{
  
  temperature=(float)sht1x.readTemperatureC();
  humidity=(float)sht1x.readHumidity();
  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);
}

