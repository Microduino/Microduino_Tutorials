#include <IRremote.h>
#include <SevSeg.h>
SevSeg sevseg;
int RECV_PIN = 2;//IR receiver pin is defined as pin2
IRrecv irrecv(RECV_PIN);
decode_results results;
int t;
int action;

int deckey(unsigned long t)
{
  switch(t){
    case 0xFD08F7://button 1's code
    return 1;
    break;
    case 0xFD8877://button 2's code
    return 2;
    break;
    case 0xFD48B7://button 3's code
    return 3;
    break;
    case 0xFD28D7://button 4's code
    return 4;
    break;
    case 0xFDA857://button 5's code
    return 5;
    break;
    case 0xFD6897://button 6's code
    return 6;
    break;
    case 0xFD18E7://button 7's code
    return 7;
    break;
    case 0xFD9867://button 8's code
    return 8;
    break;
    case 0xFD58A7://button 9's code
    return 9;
    break;
    default:
    return 0;
    break;
  }
} 

void setup() {
//defing use pins
  sevseg.Begin(1,2,3,4,5,6,7,8,9,10,11,12,13);
  irrecv.enableIRIn(); // Initialization infrared receiver
  //sevseg.PrintOutput();
  //sevseg.NewNum(0, 4);
}

void loop() {
  //enable printout
  sevseg.PrintOutput();
  
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);//Wrap output in hexadecimal code receiver
    action=deckey(results.value);
    Serial.println(action);
    Serial.println();//In order to facilitate viewing a blank line to increase output
    
    sevseg.NewNum(action, 4);     //Digital output function number, where number is the location of the decimal point, I placed fourth grid so the effect is not displayed decimal
    
    irrecv.resume(); // receive next value
  }


}
