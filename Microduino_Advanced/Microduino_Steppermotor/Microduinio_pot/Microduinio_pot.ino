#include <AccelStepper.h>

int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 5; //digital pin 2
int motorStepPin = 6; //digital pin 3

const int ENPin =  4;      // the number of the LED pin

#define ANALOG_IN A0

AccelStepper stepper(1, motorStepPin, motorDirPin); 

void setup(){
  Serial.begin(9600);
  pinMode(ENPin, OUTPUT);  
  digitalWrite(ENPin, LOW);
  stepper.setMaxSpeed(motorSpeed);
  stepper.setSpeed(motorSpeed);
  stepper.setAcceleration(motorAccel);
  
}

void loop(){

  int analog_in = analogRead(ANALOG_IN);
  analog_in = map(analog_in, 0, 1023, 0, 3200);
  stepper.moveTo(analog_in);

  stepper.run();
}


