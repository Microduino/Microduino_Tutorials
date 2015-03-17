#include <SoftwareSerial.h>

#define outletPin 6

SoftwareSerial mySerial(4, 5); //RX,TX

#define my_Serial mySerial 
//#define my_Serial Serial1  //Core+

String tmp="";

void setup() {
  Serial.begin(9600);
  Serial.println("LEDOnOff Starting...");
  my_Serial.begin(9600); 

  pinMode(outletPin, OUTPUT);
  digitalWrite(outletPin,LOW);
}

void loop() {
  // if there's any serial available, read it:
  while (my_Serial.available() > 0) {

    tmp += char(my_Serial.read());
    delay(2);
  }

  if (tmp.length() > 0) {
    if(tmp=="on") {
      Serial.println("power on");
      digitalWrite(outletPin,HIGH);
    } 
    else if(tmp=="off") {
      digitalWrite(outletPin,LOW);
      Serial.println("power off");
    }
    tmp = "";
  }
}


