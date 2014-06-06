import processing.serial.*;

Serial myPort;
PFont b;

int lf = 10;    // Linefeed in ASCII
String myString = null;
float angle;

void setup(){
  size(600,400);
  b = loadFont("Arial-BoldMT-48.vlw");
  myPort = new Serial(this, Serial.list()[0], 9600);
}

void draw(){

background(255);

while (myPort.available() > 0) {
    myString = myPort.readStringUntil(lf);
    if (myString != null) {
  //print(myString);  // Prints String
    angle=float(myString);  // Converts and prints float
    println(angle);
    }
  }
  translate(160, 50);

  // draw the compass background
  ellipseMode(CENTER);
  fill(50);
  stroke(10);
  strokeWeight(2);
  ellipse(150,150,300,300);

  // draw the lines and dots
  translate(150,150);  // translate the lines and dots to the middle of the compass
  float CompassX = -angle;
  rotate(radians(CompassX));
  noStroke();
  fill(51, 255, 51);

  int radius = 120;

  for( int degC = 5; degC < 360; degC += 10) //Compass dots
  {
    float angleC = radians(degC);
    float xC = 0 + (cos(angleC)* radius);
    float yC = 0 + (sin(angleC)* radius);
    ellipse(xC,yC, 3, 3);
  }

  for( int degL = 10; degL < 370; degL += 10) //Compass lines
  {
    float angleL = radians(degL);
    float x = 0 + (cos(angleL)* 145);
    float y = 0 + (sin(angleL)* 145);
 
    if( degL==90 || degL==180 || degL==270 || degL==360) {
     stroke(51, 255, 51);
     strokeWeight(4);
    }
    else {
      stroke(234,144,7);
      strokeWeight(2);
    }
    line(0,0, x,y);
  }

  fill(102, 102, 102);
  noStroke();
  ellipseMode(CENTER);
  ellipse(0,0, 228,228); //draw a filled circle to hide the lines in the middle

  b = loadFont("Arial-BoldMT-48.vlw");
  textAlign(CENTER);

  // Draw the letters
  fill(250);
  textFont(b, 32);
  text("N", 1, -90);
  rotate(radians(90));
  text("E", 0, -90);
  rotate(radians(90));
  text("S", 0, -90);
  rotate(radians(90));
  text("W", 0, -90);
  rotate(radians(90));
 
  textFont(b,40);
  textAlign(CENTER);
  //text((angle), 20, 20);
  println(angle);

  //draw the needle

  rotate(radians(-CompassX)); //make it stationary
  stroke(234,144,7);
  strokeWeight(3);

  triangle(-10, 0, 10, 0, 0, -85);
  fill(234,144,7);
  triangle(-10, 0, 10, 0, 0, 60);

}
