import processing.serial.*;

import cc.arduino.*;

import controlP5.*;

ControlP5 cp5;

float turn=radians(-90);

Arduino arduino;

int pos=0; //servo position in degrees (0..180)
PFont f;  


void setup()
{
    size(600, 600);
  strokeWeight(30);
  arduino = new Arduino(this, Arduino.list()[0], 57600); //your offset may vary
  arduino.pinMode(9, 5);
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

    float angle=map(pos, 0, 600, 0, 360);


    arc(width/2, height/2, 300, 300, turn, radians(angle)+turn);
    //Specify font to be used
    textFont(f, 48); 
    //Display Text
    text ((int)angle, width/2-30, height/2+20);

    arduino.analogWrite(9, (int)constrain(angle, 0, 180));
}  

void slider(float posValue) {
  pos=(int)posValue;
  println("Servo pos:"+pos);
}
