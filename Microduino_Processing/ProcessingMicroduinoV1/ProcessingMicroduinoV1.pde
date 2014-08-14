import processing.serial.*;


Serial myPort;  
int buttonValue;

int xPos, yPos;


PImage face; 

void setup() {
  //println(Serial.list());

  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line

  size(800, 400);
  face = loadImage("face.png");

  xPos=400;
  yPos=200;
  image(face, xPos, yPos);
}

void draw() {

  background(255);


  String val = myPort.readStringUntil('\n');

  if (val != null) {
    val = trim(val);
    println(val);
    buttonValue=Integer.parseInt(val);

    switch(buttonValue) {
    case 1:
      yPos++;
      break;
    case 2:
      xPos--;
      break;
    case 3:
      xPos=400;
      yPos=200;
      break;
    case 4:
      xPos++;
      break;
    case 5:
      yPos--;  
      break;
    default :
    }
    image(face, xPos, yPos);
  }

}

