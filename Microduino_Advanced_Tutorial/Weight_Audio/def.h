#include "arduino.h"
#include <SoftwareSerial.h>

SoftwareSerial mySerial(4, 5); //RX,TX

//#define Audio_serial Serial 
#define Audio_serial Serial1
//#define Audio_serial mySerial

#define key_pin 6
#define shock_pin 7

#define alarm_vol 28

#define time 5000

#define Angle 14

unsigned long timer;

boolean sta=false;

float Weight,Weight_cg=0;



