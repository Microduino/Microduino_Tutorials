// Processing code for this example

// Write data to the serial port according to the status of a button controlled
// by the mouse

import processing.serial.*;

Serial port;                // Create serial port object
PFont f;  

// Declaring a variable of type PImage, a class available to us from the Processing core library.
PImage img; 

String turning = "turning";

void setup() {
  size(500, 311);

  // List all the available serial ports in the output pane.
  // You will need to choose the port that the Wiring board is
  // connected to from this list. The first port in the list is
  // port #0 and the third port in the list is port #2.
  println(Serial.list());

  // Open the port that the Wiring board is connected to (in this case #0)
  // Make sure to open the port at the same speed Wiring is using (9600bps)
  port = new Serial(this, Serial.list()[0], 9600);


  f = loadFont( "ArialMT-16.vlw" );
    // Make a new instance of a PImage by loading an image file
  img = loadImage("turnLeftRight.jpg");
}

void draw() 
{
  background(0);
  image(img,0,0);

  
  noFill();
  if("left".equals(turning)) {
    rect(30,80,150,210);
    fill(0);        //Specify font color
    text ( "turn left" ,250,20); 
    port.write("l");
  }else if("right".equals(turning)) {
    rect(320,90,150,200);
    fill(0);        //Specify font color
    text ( "turn right" ,250,20); 
    port.write("r");
  }
  
  
  
  
}


// When the mouse is moved, the state of the turning is toggled.
void mouseMoved() {
  if (mouseX > 30 && mouseX < 180 && mouseY > 80 && mouseY < 290) {
    turning = "left";
    cursor(HAND);
  }
  else if (mouseX > 320 && mouseX < 470 && mouseY > 90 && mouseY < 290) {
    turning = "right";
    cursor(HAND);
  }
  else {
    turning = "turning";
    cursor(ARROW);
  }
}

