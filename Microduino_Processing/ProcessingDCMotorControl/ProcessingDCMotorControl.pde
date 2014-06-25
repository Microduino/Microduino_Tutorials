// Processing code for this example

// Write data to the serial port according to the status of a button controlled
// by the mouse

import processing.serial.*;
import cc.arduino.*;
import controlP5.*;

ControlP5 cp5;

float turn=radians(-90);
float speed=0;

//Declare PFont variable
PFont f;  

Arduino arduino;

int motorPin = 9;//Motor pin

int vel = 0;//Value recorded motor

void setup() {
  size(600, 600);
  strokeWeight(30);

  arduino = new Arduino(this, Arduino.list()[0], 57600); //your offset may vary
  arduino.pinMode(motorPin, Arduino.OUTPUT);  

  f = loadFont( "ArialMT-16.vlw" );

  cp5 = new ControlP5(this);

  // add a vertical slider
  cp5.addSlider("slider")
    .setPosition(0, 580)
      .setSize(600, 20)
        .setRange(0, 600)
          .setValue(25)
            ;
}

void draw() 
{

  background(0);
  noFill();
  stroke(255);
  smooth();
  arc(width/2, height/2, 300, 300, turn, radians(map(speed, 0, 600, 0, 360))+turn);
  //Specify font to be used
  textFont(f, 48); 
  //Display Text
  text ((int)speed, width/2-30, height/2+20);


  vel=(int)map(speed, 0, 600, 0, 255);//map
  if (speed>0)//run motor by speed
  {
    arduino.analogWrite(motorPin, vel);
    delay(1);
  }
  else//stop motor
  {
    arduino.analogWrite(motorPin, Arduino.LOW);
  }
}

void slider(float speedValue) {
  speed=speedValue;
  println("Motor Speed:"+speed);
}


