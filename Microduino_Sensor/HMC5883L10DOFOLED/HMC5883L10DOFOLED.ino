#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

/*-------fangxiang------*/
#include "Wire.h"
#include "I2Cdev.h"

#include "HMC5883L.h"

HMC5883L mag;

int16_t mx, my, mz;

#define xXIAO 90

float xHeading,yHeading,zHeading;
int xDegrees,yDegrees,zDegrees;

void setup ()
{ 

  u8g.setRot90();

  Wire.begin();

  // initialize serial communication
  // it's really up to you depending on your project)
  Serial.begin(115200);

  // initialize device
  Serial.println("Initializing I2C devices...");
  mag.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(mag.testConnection() ? "HMC5883L connection successful" : "HMC5883L connection failed");

}

void loop()
{   
  vofangxiang();

    u8g.firstPage();  
    do {
      draw();
    } 
    while( u8g.nextPage() );

}

void draw() {  


  //==========FANGXIANG
  float hudu=3.14*(xDegrees/180.0);
  int x,y;
  x=25*sin(hudu);
  y=25*cos(hudu);

#define xqishi 36
#define yqishi 60

  u8g.drawCircle(xqishi,yqishi,25);

  u8g.drawLine(xqishi, yqishi,   xqishi+x, yqishi-y);
  u8g.drawLine(xqishi, yqishi-1,   xqishi+x, yqishi-y);
  u8g.drawLine(xqishi, yqishi+1,   xqishi+x, yqishi-y);
  u8g.drawLine(xqishi-1, yqishi,   xqishi+x, yqishi-y);
  u8g.drawLine(xqishi+1, yqishi,   xqishi+x, yqishi-y);

}


void vofangxiang()
{
  mag.getHeading(&mx, &my, &mz);
  xHeading = atan2(my, mx)/3.141593*180.0; 

  xHeading += xXIAO;

  if(xHeading < 0) xHeading += 360; 
  if(xHeading > 360) xHeading -= 360; 

  xDegrees = xHeading+0.5; 
}








