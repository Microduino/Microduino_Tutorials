import processing.serial.*;
Serial myPort;

int i, j;
PFont font;

int lf = 10;    // Linefeed in ASCII
String myString = null;

void setup()
{
  
  myPort = new Serial(this, Serial.list()[0], 9600);
  size(600, 600, P3D);
  font = loadFont("Tahoma-48.vlw");
}

void buildpoint()
{
  smooth();
  fill(#ff0000);
  ellipse(0, 0, 7, 7);
  noStroke();
  beginShape(QUADS);
  
  fill(#ffffff);//behind
  vertex(-2, 20, 0);
  vertex(2, 20, 0);
  vertex(1, -60, 0);
  vertex(-1, -60, 0);
  endShape();
}

void builddashboard()
{
  stroke(255);
  smooth();
  strokeWeight(8);
  stroke(100);
  noFill();
  ellipse(0, 0, 150, 150);
  stroke(255);
  strokeWeight(2);
  ellipse(0, 0, 158, 158);
  
  
  textFont(font, 10);
  for (i=0;i<36;i++)
  {
    line(46*cos((PI/18)*i), 46*sin((PI/18)*i), 50*cos((PI/18)*i), 50*sin((PI/18)*i));
  }
  for (j=-3;j<9;j++)
  {
    line(40*cos((PI/18)*j*3), 40*sin((PI/18)*j*3), 50*cos((PI/18)*j*3), 50*sin((PI/18)*j*3));
    if (j==-3)
    {
      text(j*30+90, 65*cos((PI/18)*j*3)-3, 60*sin((PI/18)*j*3)+3);
    }
    else
    {
      text(j*30+90, 65*cos((PI/18)*j*3)-7, 60*sin((PI/18)*j*3)+3);
    }
  }
}

void drawCube()
{
  pushMatrix();
  translate(300, 300, 0);
  scale(2, 2, 0);
  
  while (myPort.available() > 0) {
    myString = myPort.readStringUntil(lf);
    if (myString != null) {
  //print(myString);  // Prints String
    angle=float(myString);  // Converts and prints float
    println(angle);
    }
  }

  //rotateZ(mouseX*PI/100-300);
  rotateZ(angle);
  
  buildpoint();
  popMatrix();
  
}

void drawCube1()
{
  pushMatrix();
  translate(300, 300, 0);
  scale(2, 2, 0);
  builddashboard();
  popMatrix();
}

void draw()
{
  background(#000000);
  fill(#ffffff);
  
  drawCube();
  drawCube1();
  
  
  stroke(0, 255, 0);
  ellipse(width/2, height/2, 300, 300);
}

