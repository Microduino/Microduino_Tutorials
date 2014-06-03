#include <AccelStepper.h>
#include <Wire.h>

int motorSpeed = 7000; //maximum steps per second (about 3rps / at 16 microsteps)

int motorLDirPin = 5; //digital pin 5
int motorLStepPin = 6; //digital pin 6
char command='t';

const int ENPin =  4;      // the number of the LED pin


//set up the accelStepper intances
//the "1" tells it we are using a driver
AccelStepper stepper(1, motorLStepPin, motorLDirPin);

//stop
void stop() {
}

void setup() { 
  pinMode(ENPin, OUTPUT);  
  digitalWrite(ENPin, LOW);
  Serial.begin(9600);
  Wire.begin();
  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
}

void loop() {
  if(Serial.available())
  {
    command=Serial.read();
    Serial.println(command);
    if(command=='l') {
      stepper.setSpeed(motorSpeed);
      stepper.runSpeed();
    } 
    else if(command=='r') {
      stepper.setSpeed(-motorSpeed);
      stepper.runSpeed();
    }
    else {
      stepper.stop();
    }
  }
}












