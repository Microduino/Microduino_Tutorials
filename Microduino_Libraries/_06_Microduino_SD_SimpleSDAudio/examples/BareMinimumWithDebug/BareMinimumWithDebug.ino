/*
 Simple SD Audio bare minimum example, plays file EXAMPLE.AFM from root folder of SD card.
 
 This example shows how to use the SimpleSDAudio library for audio playback.
 You need: 
 - An Arduino with ATmega168/ATmega368 or better
 - An SD-Card connected to Arduinos SPI port (many shields will do)
   -> copy EXAMPLE.AFM on freshly formated SD card into root folder
 - A passive loudspeaker and resistor or better: active speakers (then stereo output will be possible)
 
 Audio signal output is at the following pin:
 - Arduino with ATmega168/328   (many non-mega Arduinos): Pin 9
 - Arduino with ATmega1280/2560 (many mega Arduinos)    : Pin 44
 
 Using passive speaker:	
    Audio-Pin --- -[100 Ohm resistor]- ---- Speaker ---- GND
    
 Using amplifier / active speaker / line-in etc.:
    Audio-Pin --||--------------[10k resistor]----+----[1k resistor]---- GND
              100nF capacitor                   to amp
 
 See SimpleSDAudio.h or our website for more information:
 http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio
 
 created  23 Jun 2012 by Lutz Lisseck
 */
#include <SimpleSDAudio.h>

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  // Using F("...") to avoid wasting RAM  
  Serial.print(F("\nInitializing SD card..."));  
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
   SdPlay.setSDCSPin(7);
  
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card is inserted?"));
    Serial.println(F("* Is your wiring correct?"));
    Serial.println(F("* maybe you need to change the chipSelect pin to match your shield or module?"));
    Serial.print(F("Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("Wiring is correct and a card is present.")); 
  }

  Serial.print(F("Looking for EXAMPLE.AFM... "));
  if(!SdPlay.setFile("EXAMPLE.AFM")) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("found.")); 
  }    

  Serial.print(F("Playing... ")); 
  SdPlay.play();
  while(!SdPlay.isStopped()) {
    SdPlay.worker();
  }
  Serial.println(F("done."));
  SdPlay.deInit();
}


void loop(void) {

}
