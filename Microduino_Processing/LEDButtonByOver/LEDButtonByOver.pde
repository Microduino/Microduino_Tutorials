//Button as switch

import processing.serial.*;
import cc.arduino.*;

// Declaring a variable of type PImage, a class available to us from the Processing core library.
PImage LEDON, LEDOFF; 

Arduino arduino;
int ledPin = 13;

boolean button = false;

int x = 50;
int y = 50;
int w = 75;
int h = 100;

void setup() {

  println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  arduino.pinMode(ledPin, Arduino.OUTPUT);

  size(200, 200);
  LEDON = loadImage("LEDON.png");
  LEDOFF = loadImage("LEDOFF.png");
  background(0);//black background
}

void draw() {
  if (button) {
    arduino.digitalWrite(ledPin, Arduino.HIGH);
    //background(255);
    image(LEDON,x,y);
    //stroke(0);
  } 
  else {
    arduino.digitalWrite(ledPin, Arduino.LOW);
    //background(0);
    image(LEDOFF,x,y);
    //stroke(255);
  }

  //fill(175);
  //rect(x, y, w, h);
}

// When the mouse is moved, the state of the button is toggled. 
void mouseMoved() {
  if (mouseX > x && mouseX < x+w && mouseY > y && mouseY < y+h) {
    button = true;
  }  
  else {
    button = false;
  }
}

