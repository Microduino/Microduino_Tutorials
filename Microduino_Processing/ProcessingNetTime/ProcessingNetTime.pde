import processing.serial.*;
Serial myPort;        // The serial port

//Declare PFont variable
PFont f;  
String timeShow="00:00:00";

void setup() {


  println(Serial.list());
  // is always my  Arduino, so I open Serial.list()[0].
  // Open whatever port is the one you're using.
  myPort = new Serial(this, Serial.list()[0], 9600);
  myPort.bufferUntil('\n'); // Trigger a SerialEvent on new line


  size(400, 200);

  //Load Font
  f = loadFont( "BDZYJT--GB1-0-48.vlw" );
} 

void draw() {
  background(255);
  textFont(f, 48); //Specify font to be used
  fill(0);        //Specify font color


  //read port data
  String val = myPort.readStringUntil('\n');
  if (val != null) {
    val = trim(val);
    println(val);

    if (val.startsWith("time:")) {
      //if some exception happend, initial time is 0 colock
      try {
        timeShow=val.substring(5,13);
      } 
      catch (NumberFormatException e) {
        println("error:"+val);
      }

      //Display Text
      text ( timeShow, 90, 120);
    }
  }
  
}

