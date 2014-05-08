import processing.serial.*;
import cc.arduino.*;

int x;
int y;
int w;
int h;

Arduino arduino;

LED[] leds = new LED[8]; // An array of 8 led objects!

void setup() {

  arduino = new Arduino(this, Arduino.list()[0], 57600);
  size(600, 200);
  smooth();
  for (int i = 0; i < leds.length; i ++ ) { // Initialize each Car using a for loop.
    leds[i] = new LED((i+1)*60, 75, i+1);
  }
}

void draw() {
  background(255);
  for (int i = 0; i < leds.length; i ++ ) { // Run each Car using a for loop.
    leds[i].display();
    if (leds[i].button) {
      arduino.digitalWrite(leds[i].ledPin, Arduino.HIGH);
    } 
    else {
      arduino.digitalWrite(leds[i].ledPin, Arduino.LOW);
    }
  }
}


// When the mouse is pressed, the state of the button is toggled.   
void mousePressed() {
  for (int i = 0; i < leds.length; i ++ ) {

    x=leds[i].xpos;
    y=leds[i].ypos;
    w=leds[i].w;
    h=leds[i].h;

    if (mouseX > x && mouseX < x+w && mouseY > y && mouseY < y+h) {
      leds[i].button = !leds[i].button;
    }
  }
}

