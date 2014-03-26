#include <AccelStepper.h>

int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 5; //digital pin 2
int motorStepPin = 6; //digital pin 3

AccelStepper stepper(1, motorStepPin, motorDirPin); 

void setup(){
 stepper.setMaxSpeed(motorSpeed);
 stepper.setSpeed(motorSpeed);
 stepper.setAcceleration(motorAccel);
 
 stepper.moveTo(3200); //move 32000 steps (should be 10 rev)
}

void loop(){

 if (stepper.distanceToGo() == 0){

  stepper.moveTo(-stepper.currentPosition()); 
 }

 stepper.run();
}
