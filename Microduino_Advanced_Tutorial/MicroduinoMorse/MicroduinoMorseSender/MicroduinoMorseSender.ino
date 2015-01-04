#include <SoftwareSerial.h>

// Author Erik Linder
// Released 2011 under GNU GPLv3
//
// Usage: morse( , , <1 0="PTT" beep=""> )
//        sendmsg( "" )
//

#include <Morse.h>

// Uncomment to beep a speaker at pin 9
//Morse morse(9, 12, 1);

// Use pin 13 (built-in LED of Arduino 2009)
Morse morse(13, 12, 0);

void setup()
{
  Serial.begin(9600);
  // Nothing here for the Morse lib
}

void loop()
{
  morse.sendmsg("ABCD");
  delay (5000);
}
