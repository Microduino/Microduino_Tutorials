
 
#include <IRremote.h>
 
int RECV_PIN = 11;//Defined infrared receiver pin 11
IRrecv irrecv(RECV_PIN);
decode_results results;

void setup()
{
  Serial.begin(9600);
  irrecv.enableIRIn(); // Initialization infrared receiver
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);//Wrap output in hexadecimal code receiver
    Serial.println();
    irrecv.resume(); // receive next value
  }
}
