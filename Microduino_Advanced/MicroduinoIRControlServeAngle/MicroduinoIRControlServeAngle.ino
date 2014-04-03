#include <IRremote.h>
#include <Servo.h>

int action;
int deg=0;
int t;

int RECV_PIN = 11;//Defined infrared receiver pin 11
IRrecv irrecv(RECV_PIN);
Servo servo;
decode_results results;

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

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Initialization infrared receiver
  servo.attach(9);//D9pin
  servo.write(0);
  t=0;
}

void loop() {

  deg=servo.read();
  if (irrecv.decode(&results))
  {
    action=deckey(results.value);
    Serial.println(results.value, HEX);//Wrap output in hexadecimal code receiver
    Serial.println(action);
    Serial.println();

    switch(action)
    {
      case 1:
      servo.write(0);
      break;
      case 2:
      servo.write(20);
      break;
      case 3:
      servo.write(40);
      break;
      case 4:
      servo.write(60);
      break;
      case 5:
      servo.write(80);
      break;
      case 6:
      servo.write(100);
      break;
      case 7:
      servo.write(120);
      break;
      case 8:
      servo.write(140);
      break;
      case 9:
      servo.write(160);
      break;
    }
    delay(10);
    irrecv.resume();
  }

}
