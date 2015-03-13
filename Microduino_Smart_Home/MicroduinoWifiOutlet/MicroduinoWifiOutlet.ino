#include "U8glib.h"
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"
#include <Servo.h>

#include "def.h"
#include "oled.h"
#include "wifi.h"
#include "outlet.h"

unsigned long lcd_time = millis(); //OLED刷新时间计时器 

void setup() {

  Serial.begin(115200);

  pinMode(outletPin, OUTPUT);

  digitalWrite(outletPin,LOW);

	delay(200); // a 2 seconds delay while we position the solar panel

  setup_wifi();

}
void loop() {
  wifi_data();

  doOutlet();

  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD) {
    volcd(); //调用显示库
    lcd_time = millis();
  }
}






