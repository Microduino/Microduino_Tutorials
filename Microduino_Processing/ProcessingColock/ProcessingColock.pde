import processing.serial.*;
Serial myPort;        // The serial port

int cx, cy;
float secR;
float minR;
float hourR;
float clockDiameter;
int hour, minute, second;

void setup() {

  println(Serial.list());
  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line

  size(640, 480);
  stroke(255);

  int radius = min(width, height)/2;
  secR = radius * 0.72;
  minR = radius * 0.60;
  hourR = radius * 0.50;
  clockDiameter = radius * 1.8;
  cx = width /2 ;
  cy = height /2 ;
}


void draw() {
  background(0);
  fill(255, 0, 25, 100);
  noStroke();
  ellipse(cx, cy, clockDiameter, clockDiameter);

  //read port data
  String val = myPort.readStringUntil('\n');
  if (val != null) {
    val = trim(val);
    println(val);

    //split data by ":"
    String time[]=val.split(":"); 

    //if some exception happend, initial time is 0 colock
    try {
      hour=Integer.parseInt(time[0]);
      minute=Integer.parseInt(time[1]);
      second=Integer.parseInt(time[2]);
    } 
    catch (NumberFormatException e) {
      hour=0;
      minute=0;
      second=0;
    }

    float s = map(second, 0, 60, 0, TWO_PI) - HALF_PI;
    float m = map(minute+norm(second, 0, 60), 0, 60, 0, TWO_PI)- HALF_PI;
    float h = map(hour+norm(minute, 0, 60), 0, 24, 0, TWO_PI * 2 ) - HALF_PI;



    //draw colock
    stroke(255, 250, 0);
    strokeWeight(1);
    line(cx, cy, cx + cos(s) * secR, cy + sin(s) * secR);
    strokeWeight(2);
    line(cx, cy, cx+ minR * cos(m), cy + minR * sin(m));
    strokeWeight(4);
    line(cx, cy, cx + hourR * cos(h), cy + hourR * sin(h));

    strokeWeight(2);
    stroke(255, 0, 0);

    for (int a = 0 ;a<360;a+=6) {
      float angle =  radians(a);
      float cx1 =  (secR+20) * cos(angle);
      float cy1 =  (secR+20) * sin(angle);
      point(cx + cx1, cy + cy1);

      if (a%30==0) {
        line(cx+cx1, cy+cy1, cx+cx1*0.98, cy+cy1*0.98);
        fill(255);
        int mark;
        if (a/30>9) {
          mark = a/30 -9;
        }
        else
        { 
          mark = a/30 +3;
        }
        text(mark, cx+cx1*1.05-5, cy+cy1*1.05+5);
      }

      fill(90, 155, 11);
      text(hour+":"+minute+":"+second, cx-25, cy+100);
      text("Microduino", cx-25, cy-100);
    }
  }
}

