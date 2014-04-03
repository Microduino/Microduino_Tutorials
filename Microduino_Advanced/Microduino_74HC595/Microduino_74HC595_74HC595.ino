/*                               
Using a 74HC595 shift register code
     Count from 0 to 255
*/

//Pin 8 connect to ST_CP of 74HC595
int latchPin = 8;
//Pin 12 connect to SH_CP of 74HC595
int clockPin = 12;
////Pin 11 connect to DS of 74HC595
int dataPin = 11;



void setup() {
  //Set pins to output so that you can control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
}

void loop() {
  // 2 hex digits in the LED display on, from 0-255
  for (int numberToDisplay = 0; numberToDisplay < 256; numberToDisplay++) {
    // Set latchPin LOW
    digitalWrite(latchPin, LOW);
    // Enter the decimal number
    shiftOut(dataPin, clockPin, MSBFIRST, numberToDisplay);  

    //Set latch pin is high, LED will display the corresponding binary numbers
    digitalWrite(latchPin, HIGH);
    // pause before next value:
    delay(500);
  }
}
