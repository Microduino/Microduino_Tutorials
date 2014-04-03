#include <IRremote.h>                  // call IRRemote lib

IRsend irsend; 
// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  3;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  

  digitalWrite(ledPin, HIGH); 
  delay(3000);
  digitalWrite(ledPin, LOW); 
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {    
    Serial.print("SendIR: ");
    irsend.sendNEC(0x234E817, 32); //9
  }
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW);
  }
}
