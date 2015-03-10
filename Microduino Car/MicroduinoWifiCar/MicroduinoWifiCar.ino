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
#include "robot.h"

//#include <Wire.h> //调用库








unsigned long lcd_time = millis(); //OLED刷新时间计时器 





void setup() {

  Serial.begin(115200);

  pinMode(motor_pinB, OUTPUT);
  pinMode(motor_pinA, OUTPUT);
  pinMode(servo_pinB, OUTPUT);
  pinMode(servo_pinA, OUTPUT);



  digitalWrite(servo_pinA, HIGH);
  digitalWrite(servo_pinB, LOW);

	// initialize serial communication at 9600 bits per second:
	myservo.attach(3); // attaches the servo on pin 11 to the servo object

	myservo.write(angle);
	delay(200); // a 2 seconds delay while we position the solar panel

  setup_wifi();

}
void loop() {
  wifi_data();
  

  vorobot();



  
  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD) {
    volcd(); //调用显示库
    lcd_time = millis();
  }
}






