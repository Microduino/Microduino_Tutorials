// Processing code for this example

// Write data to the serial port according to the status of a button controlled
// by the mouse

import processing.serial.*;

Serial port;                // Create serial port object
PFont f;  

void setup() {
  size(640, 200);

  // List all the available serial ports in the output pane.
  // You will need to choose the port that the Wiring board is
  // connected to from this list. The first port in the list is
  // port #0 and the third port in the list is port #2.
  println(Serial.list());

  // Open the port that the Wiring board is connected to (in this case #2)
  // Make sure to open the port at the same speed Wiring is using (9600bps)
  port = new Serial(this, Serial.list()[0], 9600);


  f = loadFont( "ArialMT-16.vlw" );
}

void draw() 
{
  background(0);
  update(mouseX/10); 
  println(mouseX);
}

void update(int x) 
{
  port.write(x);

  stroke(255);  
  line(mouseX, 0, mouseX, 160);   
  text (mouseX, mouseX, 180);
}

