// LED object
class LED {

  int xpos;
  int ypos;
  int w;
  int h;
  
  PImage LEDState=loadImage("LEDOFF.png");
  int ledPin;
  boolean button = false;


  LED(int xpos_, int ypos_, int w_, int h_,int ledPin_) {
    xpos = xpos_;
    ypos = ypos_;
    ledPin=ledPin_;
    w=w_;
    h=h_;
  }

  void display() {
    if(button) {
      LEDState=loadImage("LEDON.png");
    } else {
      LEDState=loadImage("LEDOFF.png");
    }
    image(LEDState,xpos,ypos,w,h);
  }

}
