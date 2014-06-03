import processing.serial.*;

Serial myPort;        // The serial port


PFont myFont;

double x=0,y=0;
int r;
float hudu;

void setup() {
  
    println(Serial.list());

  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line

  
  
  size(640, 480);
  myFont = createFont("arial", 30);
  r=(height/2)-50;
  textFont (myFont);
}

void draw() {
  background(255);

  stroke(color(0, 200, 200));
  fill(255);
  ellipse(width/2, height/2, 2*r, 2*r);


  //arrow(width/2, height/2, (width/2)+(int)x, (height/2)-(int)y, color(200,0,200));
  arrow(width/2, height/2, (width/2)+mouseX, (height/2)-mouseY, color(200,0,200));
  //arrow(width/2, height/2, width/2, 50, color(0, 200, 200));


  textAlign(CENTER);
  fill(0, 102, 153);
  //text("North", width/2, 25); 
  PVector v1 = new PVector(0, width/2-50);
  
  
  String val = myPort.readStringUntil('\n');
  if (val != null) {
    val = trim(val);
    String xAndy[]=val.split(","); 
    
    println(xAndy);
    
    if(isNumeric(xAndy[0])&&isNumeric(xAndy[1])) {
      x=Float.parseFloat(xAndy[0]);
      y=Float.parseFloat(xAndy[1]);
    }

  }
  
  
  //PVector v2 = new PVector((width/2)-mouseX, (height/2)-mouseY); 
  PVector v2 = new PVector((width/2)-(int)x, (height/2)-(int)y); 
  float a = PVector.angleBetween(v1, v2);
  if (x < width/2) {
  //if (mouseX < width/2) {
    text(-degrees(a)+360 + " degrees", width/2, height/4*3); 
    hudu=3.14*((-degrees(a)+360)/180.0);
  }
  else {
    text(degrees(a) + " degrees", width/2, height/4*3);  // Prints "10.304827");
    hudu=3.14*(degrees(a)/180.0);
  }
  //text("(Clockwise from North)", width/2, height*.85);
  fill(200, 0, 200);
  text("South pole", width/2+(int)x, height-(height/2+(int)y));
  //text("South pole", mouseX-50, mouseY+50);
  
  x=r*Math.sin(hudu);
  y=r*Math.cos(hudu);
}

void arrow(int x1, int y1, int x2, int y2, color c) {
  smooth();
  stroke (c);
  strokeWeight(3);
  line(x1, y1, x2, y2);
  pushMatrix();
  translate(x2, y2);
  float a = atan2(x1-x2, y2-y1);
  rotate(a);
  line(0, 0, -10, -10);
  line(0, 0, 10, -10);
  popMatrix();
} 

public static boolean isNumeric(String str){
  for (int i = 0; i < str.length(); i++){
   //System.out.println(str.charAt(i));
   if (!Character.isDigit(str.charAt(i))){
    return false;
   }
  }
  return true;
 }
