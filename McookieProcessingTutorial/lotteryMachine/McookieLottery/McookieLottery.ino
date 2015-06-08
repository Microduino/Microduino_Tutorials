#include "keyButton.h"
const int buttonPin = 6;      // the number of the pushbutton pin
// const int ledPin =  7;
bool buttonState = false;         // variable for reading the pushbutton status

bool statusChange=false;

// char c;
// int cc;

void setup()
{
  Serial.begin(9600); 
  pinMode(buttonPin,INPUT); 
  //pinMode(ledPin, OUTPUT);
  keyButton_init();   // initialize the pushbutton pin as an input
}
void loop()
{
    updateButton();
    //Serial.flush();
}


void updateButton() {
  if(keyButton_get(buttonPin,0))
  {
    delay(200);
    buttonState=!buttonState;
  }
  if(statusChange!=buttonState)
  {
    statusChange=buttonState;
    if(buttonState) {
      Serial.print('1');
      //digitalWrite(ledPin, HIGH); 
    } else {
      Serial.print('0');
      //digitalWrite(ledPin, LOW); 
    }

  }

}