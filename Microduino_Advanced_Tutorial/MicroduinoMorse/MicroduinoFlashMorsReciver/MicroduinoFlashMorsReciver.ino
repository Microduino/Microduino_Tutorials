#include <Servo.h>
Servo myservo; // create servo object to control a servo
// a maximum of eight servo objects can be created
int pos = 0; // variable to store the servo position 


int photocellPin = 0;     // LDR connected to pin 0
int photocellReading;     // the analog reading from LDR
long onmillis = 0;        // reset onmillis
long timeon = 0;          // reset timeon
int offset=30;
int dotTime=150;
int dashTime=350;
boolean isPrinted=false;

int servoPin = 10;

String morse;

void setup(void) {
  Serial.begin(9600);

  pinMode(servoPin, OUTPUT); // sets the digital pin as output
  myservo.attach(servoPin); // attaches the servo on pin 10 to the servo object 


}

void loop(void) {
  photocellReading = analogRead(photocellPin);  
  //Serial.println(photocellReading);
  //delay(1000);

  if(photocellReading<offset) {
    if(!isPrinted) {
      timeon = millis() - onmillis;

      if(timeon<=10) {

      } else if (timeon>=dotTime && timeon <= 250) {
        //Serial.print(timeon);
        //Serial.print(":  ");
        Serial.print(". ");
        morse+=".";
      } else if (timeon > dashTime && timeon <= 450) {
        //Serial.print(timeon);
        //Serial.print(":  ");
        Serial.print("_ ");
        morse+="_";
      } else { // larger than 300: space detected, which means the end of this letter
        Serial.print(timeon);
        Serial.print(":  ");
        Serial.println();
      }
      onmillis = 0;
      isPrinted=true;

      if(morse==".__..._._._..") {
        Serial.println(morse);
        morse="";
        myservo.write(90); // tell servo to go to position in variable 'pos'
        delay(1000); // waits 5s for the servo to reach the position
        myservo.write(0); // tell servo to go back 0 pos
      }

    }
  } else {
    // on (receiving signal)
    if (onmillis == 0) {
      // we just started receiving signal
      onmillis = millis();
      isPrinted=false;
    }
  }
}
