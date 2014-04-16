#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//-------set font Large、Middle、Small
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)



/*-------fangxiang------*/
#include "Wire.h"
#include "I2Cdev.h"

#include "BMP085.h"

/*-------gaodu------*/
BMP085 barometer;

float temperature;
float pressure;
float altitude;
int32_t lastMicros;

//==================
unsigned long time1=millis();

void setup ()
{ 

  //u8g.setRot90();

  Wire.begin();

  // initialize serial communication
  // it's really up to you depending on your project)
  Serial.begin(11520);

  // initialize device
  Serial.println("Initializing I2C devices...");
  barometer.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");
}

void loop()
{   
  vobmp085();

    u8g.firstPage();  
    do {
      draw();
    } 
    while( u8g.nextPage() );
}

void draw() {  

  setFont_L;
  u8g.setPrintPos(2, 10); 
  u8g.print(temperature,1); 
  u8g.print("`C");
  u8g.setPrintPos(2, 25); 
  u8g.print(pressure/1000,3); 
  u8g.print("Kpa");
  u8g.setPrintPos(2, 40); 
  u8g.print(altitude,1);
  u8g.print("M");

}



void vobmp085()
{
  barometer.setControl(BMP085_MODE_TEMPERATURE);

  // wait appropriate time for conversion (4.5ms delay)
  lastMicros = micros();
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated temperature value in degrees Celsius
  temperature = barometer.getTemperatureC();

  // request pressure (3x oversampling mode, high detail, 23.5ms delay)
  barometer.setControl(BMP085_MODE_PRESSURE_3);
  while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());

  // read calibrated pressure value in Pascals (Pa)
  pressure = barometer.getPressure();

  // calculate absolute altitude in meters based on known pressure
  // (may pass a second "sea level pressure" parameter here,
  // otherwise uses the standard value of 101325 Pa)
  altitude = barometer.getAltitude(pressure);

}





