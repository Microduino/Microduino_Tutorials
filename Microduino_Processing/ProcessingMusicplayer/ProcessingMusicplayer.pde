import processing.serial.*;

// Declaring a variable of type PImage, a class available to us from the Processing core library.
PImage play, pause;
PImage bg; 

Serial port; // Create serial port object

boolean button = false;

int x = 120;
int y = 115;
int w = 97;
int h = 97;

void setup() {

  println(Serial.list());
  // is always my Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  port = new Serial(this, Serial.list()[0], 9600);
  size(338, 328);
  bg=loadImage("musicPlayer.png");
  play = loadImage("play.png");
  pause = loadImage("pause.png");
  background(0);//black background
}

void draw() {

  image(bg, 0, 0);

  if (button) {
    image(play, x, y);
    port.write("p");
  } 
  else {
    image(pause, x, y);
    port.write("t");
  }
}

// When the mouse is pressed, the state of the button is toggled.
void mousePressed() {
  if (mouseX > x && mouseX < x+w && mouseY > y && mouseY < y+h) {
    button = !button;
  }
}

// When the mouse is moved, the state of the turning is toggled.
void mouseMoved() {
  if (mouseX > x && mouseX < x+w && mouseY > y && mouseY < y+h) {
    cursor(HAND);
  }
  else {
    cursor(ARROW);
  }
}


