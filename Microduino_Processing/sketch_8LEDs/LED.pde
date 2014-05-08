
// LED object
class LED { 

  int xpos;
  int ypos;
  int w = 50;
  int h = 50;
  
  PImage LEDState=loadImage("LEDOFF.png");
  int ledPin;
  boolean button = false;


  LED(int xpos_, int ypos_, int ledPin_) {
    xpos = xpos_;
    ypos = ypos_;
    ledPin=ledPin_;
  }

  void display() {
    if(button) {
      LEDState=loadImage("LEDON.png");
    } else {
      LEDState=loadImage("LEDOFF.png");
    }
    image(LEDState,xpos,ypos);
  }

}

