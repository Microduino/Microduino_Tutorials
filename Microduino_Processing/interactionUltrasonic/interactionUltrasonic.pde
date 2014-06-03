import processing.serial.*;

Serial myPort;        // The serial port

float[] vals;

float cm;

//Declare PFont variable
PFont f;  

void setup() {

  println(Serial.list());

  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line


  size(400, 200);
  f = loadFont( "Batang-20.vlw" ); 
  smooth();
  // An array of random values
  vals = new float[width];
  for (int i = 0; i < vals.length; i++) {
    //vals[i] = mouseY;
    vals[i] = 0;
  }

  textFont(f, 16); //Specify font to be used
  fill(0);        //Specify font color
}


void draw() {

  background(255);
  // Draw lines connecting all points
  for (int i = 0; i < vals.length-1; i++) {
    stroke(0);
    strokeWeight(1);
    line(i, vals[i], i+1, vals[i+1]);
  }

  // Slide everything down in the array
  for (int i = 0; i < vals.length-1; i++) {
    vals[i] = vals[i+1];
  }
  // Add a new random value
  //vals[vals.length-1] = random(height);

  String val = myPort.readStringUntil('\n');

  if (val != null) {
    val = trim(val);
    println(val);
    cm=Float.parseFloat(val);
  }


  vals[vals.length-1] = 200-cm;


  //Display scale
  text ( "200-", 370, 10); 
  text ( "--", 370, 50); 
  text ( "100-", 370, 100);
  text ( "--", 370, 150); 
  text ( "0-", 370, 200); 

  //show current num
  text ( cm, 0, 10);
}

