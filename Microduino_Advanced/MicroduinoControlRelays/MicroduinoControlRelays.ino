/*
 * relay control DC motor
 */
 
const int buttonPin = 2;                 // pushbutton
const int relayPin = 13;                 // Relay
int relayState = 0;                      // relay state
 
void setup()
{
  Serial.begin(9600);
   
  pinMode(buttonPin, INPUT);             // set buttonPin to INPUT
  pinMode(relayPin, OUTPUT);             // set relayPin to OUTPUT    
}
 
void switchRelay()
{
  if (relayState == 0)                        
    relayState = 1;                      // change relay state to ON
  else
    relayState = 0;                      // change relay state to OFF     
       
  digitalWrite(relayPin, relayState);    // switch
 
  Serial.print("Relay status: ");        // serial print relay state
  Serial.println(relayState);
}
 
void loop()
{
  int buttonState;
   
  // read button state
  buttonState = digitalRead(buttonPin);
 
  // check button if pressed
  // buttonState is HIGH
  if (buttonState == HIGH) {      
     switchRelay();                      // switch relay on/off
     delay(500);                         // delay 0.5s
  }
}
