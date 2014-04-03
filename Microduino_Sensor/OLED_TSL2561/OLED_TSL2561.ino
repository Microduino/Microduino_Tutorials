#include "U8glib.h"

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>

Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);

float sensor_lux=0.00;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//show speed
void draw() {
  u8g.setFont(u8g_font_courB18);//font
  u8g.setPrintPos(0, 16);
  u8g.print("Light intensity");
  u8g.setPrintPos(0, 50);
  u8g.print(sensor_lux);
  u8g.print("LUX");
  
}  


void setup()
{
  Serial.begin(9600);
  Serial.println(tsl.begin() ? "TSL2561 connection successful" : "TSL2561 connection failed");
}
 
 
void loop()
{

    sensors_event_t event;
  tsl.getEvent(&event);
  
  
    /* Display the results (light is measured in lux) */
  if (event.light)
    sensor_lux=event.light;
  
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);
}
