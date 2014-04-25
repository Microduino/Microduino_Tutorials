#include <SoftwareSerial.h>

SoftwareSerial mySerial(9, 10); //RX,TX

String inString = "";    // string to hold input
int currentColor = 0;
int red, green, blue = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  mySerial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // send an intro:
  Serial.println("\n\nString toInt() RGB:");
  Serial.println();
  // set LED cathode pins as outputs:
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(6, OUTPUT);

}

void loop() {
  int inChar;

  // Read serial input:
  if (mySerial.available() > 0) {
    inChar = mySerial.read();
  }

  if (isDigit(inChar)) {
    // convert the incoming byte to a char 
    // and add it to the string:
    inString += (char)inChar; 
  }

  // if you get a comma, convert to a number,
  // set the appropriate color, and increment
  // the color counter:
  if (inChar == ',') {
    // do something different for each value of currentColor:
    switch (currentColor) {
    case 0:    // 0 = red
      red = inString.toInt();
      // clear the string for new input:
      inString = ""; 
      break;
    case 1:    // 1 = green:
      green = inString.toInt();
      // clear the string for new input:
      inString = ""; 
      break;
    }
    currentColor++;
  }
  // if you get a newline, you know you've got
  // the last color, i.e. blue:
  if (inChar == '>') {
    blue = inString.toInt();


    analogWrite(6,  255 - red);
    analogWrite(4, 255 - green);
    analogWrite(5, 255 - blue);

    // print the colors:
    Serial.print("Red: ");
    Serial.print(red);
    Serial.print(", Green: ");
    Serial.print(green);
    Serial.print(", Blue: ");
    Serial.println(blue);

    // clear the string for new input:
    inString = ""; 
    // reset the color counter:
    currentColor = 0;
  }

}
