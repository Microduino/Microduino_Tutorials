//This is an example of how you would control 1 stepper

#include <AccelStepper.h>

int motorSpeed = 9600; //maximum steps per second (about 3rps / at 16 microsteps)
int motorAccel = 80000; //steps/second/second to accelerate

int motorDirPin = 20; //digital pin 2
int motorStepPin = 21; //digital pin 3

//set up the accelStepper intance
//the "1" tells it we are using a driver
AccelStepper stepper(1, motorStepPin, motorDirPin); 
//A4982,left to right(也就是OUT2B、OUT2A、OUT1A、OUT1B依次排开)，:red,green,yellow,blue，


void setup(){
 stepper.setMaxSpeed(motorSpeed);
 stepper.setSpeed(motorSpeed);
 stepper.setAcceleration(motorAccel);
 
 stepper.moveTo(32000); //move 32000 steps (should be 10 rev)
}

void loop(){
 
 //if stepper is at desired location
 if (stepper.distanceToGo() == 0){
  //go the other way the same amount of steps
  //so if current position is 400 steps out, go position -400
  stepper.moveTo(-stepper.currentPosition()); 
 }
 

 
 //these must be called as often as possible to ensure smooth operation
 //any delay will cause jerky motion
 stepper.run();
}