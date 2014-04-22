#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

String onOff="OFF";

//show ON/OFF
void draw() {
  u8g.setFont(u8g_font_courB18);//font
  u8g.setPrintPos(0, 16);
  u8g.print("Switch");
  u8g.setPrintPos(0, 50);
  u8g.print(onOff);

} 



void setup() {       
  Serial.begin(9600);   // initialize serial communication at 9600 bits per second:  
}

void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);

  if(sensorValue>210) {
    Serial.println("ON");
    onOff="ON";
  }
  else {
    Serial.println("OFF");
    onOff="OFF";
  }

  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() ); 
  delay(2);

}



