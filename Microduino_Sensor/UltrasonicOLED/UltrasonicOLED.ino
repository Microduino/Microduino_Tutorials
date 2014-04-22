#include "U8glib.h"


const int TrigPin = 2;
const int EchoPin = 3;
float cm;

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//show cm
void draw() {
  u8g.setFont(u8g_font_courB18);//font
  u8g.setPrintPos(0, 16);
  u8g.print("Distance:");
  u8g.setPrintPos(0, 50);
  u8g.print(cm);
  u8g.print(" cm");

}  

void setup()
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
void loop()
{
  digitalWrite(TrigPin, LOW); //Low-high-low-fat one short pulse to TrigPin
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);
  cm = pulseIn(EchoPin, HIGH) / 58.0; //Echo time will be converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Two decimal places


  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);

}


