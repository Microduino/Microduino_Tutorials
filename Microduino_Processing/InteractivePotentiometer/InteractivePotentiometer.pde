import processing.serial.*;
import cc.arduino.*;

Serial serial;  
int sensorValue;
int potPin = 0; //Potentiometer input pin

Arduino arduino;

PImage face; 

void setup() {
  //println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  size(800, 200);
  face = loadImage("face.png");
  
  image(face,80,80);
}

void draw() {
  sensorValue = arduino.analogRead(potPin); //Reading the value of the potentiometer
  background(255);
  image(face,sensorValue,80);
}

