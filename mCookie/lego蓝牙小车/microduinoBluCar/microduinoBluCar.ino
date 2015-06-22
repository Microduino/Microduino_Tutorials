#include <SoftwareSerial.h>
#include <ccspi.h>
#include <SPI.h>
//#include "utility/debug.h"
//#include "utility/socket.h"
#include <Servo.h>

#include "def.h"
#include "robot.h"

//SoftwareSerial mySerial(4, 5); //RX,TX
SoftwareSerial mySerial(2, 3); //RX,TX

//#define my_Serial mySerial  //core
#define my_Serial Serial1  //Core+

String tmp="";

void setup() {
  Serial.begin(9600);

  my_Serial.begin(9600); 


  pinMode(motor_pinB, OUTPUT);
  pinMode(motor_pinA, OUTPUT);
  pinMode(servo_pinB, OUTPUT);
  pinMode(servo_pinA, OUTPUT);


  digitalWrite(servo_pinA, HIGH);
  digitalWrite(servo_pinB, LOW);

  // initialize serial communication at 9600 bits per second:
  myservo.attach(servoPWM); // attaches the servo on pin 3 to the servo object

  myservo.write(angle);
  delay(200); // a 2 seconds delay while we position the solar panel
}

void loop() {


  if (my_Serial.available() > 0) {
    currentInfo = my_Serial.readStringUntil('\n');
    //Serial.println(currentInfo);
    if(currentInfo=="ERROR"||currentInfo=="Connected") {
      return;
    }
    vorobot();
  }

}

