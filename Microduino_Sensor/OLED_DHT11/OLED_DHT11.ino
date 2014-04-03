#include "U8glib.h"
//call Dht Series library function 
#include <dht.h>  
//Definition DHT
dht DHT;

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
  int chk = DHT.read11(DHT11_PIN); 
  
  temperature=(float)DHT.temperature;
  humidity=(float)DHT.humidity;
  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);
}

