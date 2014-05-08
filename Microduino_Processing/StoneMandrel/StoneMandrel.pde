import processing.serial.*;
import cc.arduino.*;

Serial serial;  

Arduino arduino;

PImage stoneMandrel1, stoneMandrel2; 

int hallPin=13;

int statePin=Arduino.LOW;

void setup() {
  println(Arduino.list());
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  arduino.pinMode(hallPin, Arduino.INPUT);
  size(453, 475);
  stoneMandrel1 = loadImage("StoneMandrel1.png");
  stoneMandrel2 = loadImage("StoneMandrel2.png");

}

void draw() {
  statePin=arduino.digitalRead(hallPin);
  if ( statePin==Arduino.HIGH) {
    image(stoneMandrel1, 0, 0);
  } else if (statePin==Arduino.LOW) {
    image(stoneMandrel2, 0, 0);
  }
}

