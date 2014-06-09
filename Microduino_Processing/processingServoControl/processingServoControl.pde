import processing.serial.*;

import cc.arduino.*;

Arduino arduino;

int pos=0; //servo position in degrees (0..180)
PFont f;  


void setup()
{
  size(360, 200);
  arduino = new Arduino(this, Arduino.list()[0], 57600); //your offset may vary
  arduino.pinMode(9, 5);
  f = loadFont( "ArialMT-16.vlw" );
}

void draw()
{
  // read mouseX coordinate
  int newPos = constrain(mouseX/2, 0, 180); // update bg & servo position if mouseX changed
  if (newPos != pos)
  {
    background(0);
    stroke(255);  
    line(mouseX, 0, mouseX, 160);   
    text (newPos, mouseX, 180); 
    arduino.analogWrite(9, newPos);
    println (" newPos = " + newPos );
    pos=newPos; //update servo position storage variable
  }
}  

