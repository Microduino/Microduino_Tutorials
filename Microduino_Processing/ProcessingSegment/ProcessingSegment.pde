import processing.serial.*;
import cc.arduino.*;

Serial myPort; // The serial port

int x;
int y;
int w;
int h;

Arduino arduino;

LED[] leds = new LED[8]; // An array of 8 led objects!

int microduinoPins[] = {//Correspondence microduino pin for 7 segment 
  11,2,4,5,6,7,9,10};

String segmentLables[]={"1-e","2-d","3-c","4-dp","5-b","6-a","7-f","8-g"};
  
//Declare PFont variable
PFont f; 

void setup() {

  arduino = new Arduino(this, Arduino.list()[0], 57600);
  size(600, 200);
  smooth();
  
  f = loadFont( "TraditionalArabic-20.vlw" ); 
  textFont(f, 20); //Specify font to be used
  fill(0);        //Specify font color
  
  for (int i = 0; i < leds.length; i ++ ) { // Initialize each led and output pin using a for loop.
    leds[i] = new LED((i+1)*60, 75, 50,50,i+1);
    arduino.pinMode(microduinoPins[i], Arduino.OUTPUT); 
  }
}

void draw() {
  background(255);
  for (int i = 0; i < leds.length; i ++ ) { // Run each Car using a for loop.
    leds[i].display();
    
    //draw segemnt lables
    text (segmentLables[i], (i+1)*60+8, 60); 
    
    if (leds[i].button) {//switch led on/off
      arduino.digitalWrite(microduinoPins[i], Arduino.HIGH);
    }
    else {
      arduino.digitalWrite(microduinoPins[i], Arduino.LOW);
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


