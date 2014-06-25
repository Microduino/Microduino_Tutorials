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
  size(400, 700);
  smooth();
  
  f = loadFont( "TraditionalArabic-20.vlw" ); 
  textFont(f, 20); //Specify font to be used
  fill(0);        //Specify font color
  
  for (int i = 0; i < leds.length; i ++ ) { // Initialize each led and output pin using a for loop.
    arduino.pinMode(microduinoPins[i], Arduino.OUTPUT); 
  }
  
  leds[0] = new LED(50, 350, 50,200,1);
  leds[1] = new LED(100, 550, 200,50,2);
  leds[2] = new LED(300, 350, 50,200,3);
  leds[3] = new LED(350, 550, 50,50,4);
  leds[4] = new LED(300, 100, 50,200,5);
  leds[5] = new LED(100, 50, 200,50,6);
  leds[6] = new LED(50, 100, 50,200,7);
  leds[7] = new LED(100, 300, 200,50,8);

}

void draw() {
  background(255);
  for (int i = 0; i < leds.length; i ++ ) { // Run each Car using a for loop.
    leds[i].display();
    
    //draw segemnt lables
    text (segmentLables[i], leds[i].xpos, leds[i].ypos); 
    
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



