//#include "audio.h"
#include "key.h"

const int buttonPin = 6;     // the number of the pushbutton pin
const int buzzerPin =  5;      // the number of the buzzer pin

boolean on_off;
boolean statusChange;


void setup(void) {
  //初始化串口波特率  
  Serial.begin(9600);    
  // initialize the LED pin as an output:
  pinMode(buzzerPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);  

  key_init();

}   

void loop(void) {

  updateButton();

}   


void updateButton() {

  if(key_get(buttonPin,0))
  {
    delay(200);
    on_off=!on_off;
  }
  if(statusChange!=on_off)
  {
    statusChange=on_off;
    if(on_off) {
      Serial.println("ON");
      //digitalWrite(buzzerPin, HIGH); 
      tone(buzzerPin,255);
    } 
    else {
      Serial.println("OFF");
      //digitalWrite(buzzerPin, LOW); 
      tone(buzzerPin,0);
    }
  }



}

