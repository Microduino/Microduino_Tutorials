import processing.serial.*;

Serial myPort;        // The serial port

float[] vals;

float[] vals2;

float temp;
float humidity;

//Declare PFont variable
PFont f;  

void setup() {

  //println(Serial.list());

  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line


  size(400, 200);
  f = loadFont( "Batang-20.vlw" ); 
  smooth();
  // two array of random values
  vals = new float[width];
  vals2 = new float[width];
  
  for (int i = 0; i < vals.length; i++) {
    vals[i] = 0;
    vals2[i] = 0;
  }

  textFont(f, 16); //Specify font to be used
  fill(0);        //Specify font color
}


void draw() {

  background(255);
  // Draw lines connecting all points
  for (int i = 0; i < vals.length-1; i++) {
    
    strokeWeight(1);
    
    stroke(255,0,0);
    line(i, vals[i], i+1, vals[i+1]);
    stroke(0,0,255);
    line(i, vals2[i], i+1, vals2[i+1]);
    
    //stroke(0);
  }

  // Slide everything down in the array
  for (int i = 0; i < vals.length-1; i++) {
    vals[i] = vals[i+1];
    
    vals2[i] = vals2[i+1];
  }


  String val = myPort.readStringUntil('\n');
  if (val != null) {
    val = trim(val);
    println(val);
    
    
    String tempAndHumidty[]=val.split(","); 
    
    
    temp=Float.parseFloat(tempAndHumidty[0]);
    humidity=Float.parseFloat(tempAndHumidty[1]);
    
    //lux=Float.parseFloat(val);
  }

  vals[vals.length-1] = 200-temp;
  vals2[vals.length-1] = 200-humidity;

  //Display scale
  fill(0, 0, 0);
  text ( "200-", 370, 10); 
  text ( "--", 370, 50); 
  text ( "100-", 370, 100);
  text ( "--", 370, 150); 
  text ( "0-", 370, 200); 

  //show current num
  fill(255, 0, 0);
  text ( "temp:"+temp+"'C", 0, 15);
  fill(0, 0, 255);
  text ( "humidity:"+humidity+"%", 0, 28);
}

