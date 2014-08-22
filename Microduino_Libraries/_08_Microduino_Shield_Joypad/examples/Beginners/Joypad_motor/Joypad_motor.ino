#include <Joypad.h>

void setup() {
  // There's nothing to set up for this sketch
}

void loop() {
  Joypad.motor(0); 
  delay(1000);
  Joypad.motor(255/5); 
  delay(1000);
  Joypad.motor(255/4); 
  delay(1000);
  Joypad.motor(255/3); 
  delay(1000);
  Joypad.motor(255/2); 
  delay(1000);
  Joypad.motor(255); 
  delay(1000);
}