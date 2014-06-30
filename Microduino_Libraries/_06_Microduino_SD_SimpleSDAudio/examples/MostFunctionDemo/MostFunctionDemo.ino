/*
 Simple SD Audio example, shows the usage of most functions 
 via terminal at serial communication port.
 
 This example shows how to use the SimpleSDAudio library for audio playback.
 You need: 
 - An Arduino with ATmega168/ATmega368 or better
 - An SD-Card connected to Arduinos SPI port (many shields will do)
   -> copy some converted audio files on freshly formated SD card into root folder
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
 
 created  29 Jun 2012 by Lutz Lisseck
 */
#include <SimpleSDAudio.h>

// Callback target, prints output to serial
void DirCallback(char *buf) { 
  Serial.println(buf);
}

char AudioFileName[16];

// Create static buffer 
#define BIGBUFSIZE (2*512)      // bigger than 2*512 is often only possible on Arduino megas!
uint8_t bigbuf[BIGBUFSIZE];

// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.print(F("Free Ram: "));
  Serial.println(freeRam());
  
  // Setting the buffer manually for more flexibility
  SdPlay.setWorkBuffer(bigbuf, BIGBUFSIZE); 
  
  Serial.print(F("\nInitializing SimpleSDAudio V" SSDA_VERSIONSTRING " ..."));  
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  SdPlay.setSDCSPin(7);
  
  // Select between SSDA_MODE_FULLRATE or SSDA_MODE_HALFRATE (62.5kHz or 31.25kHz)
  // and the output modes SSDA_MODE_MONO_BRIDGE, SSDA_MODE_MONO or SSDA_MODE_STEREO
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
  

}


void loop(void) {
  uint8_t count=0, c, flag;
  
  Serial.println(F("Files on card:"));
  SdPlay.dir(&DirCallback);

ReEnter: 
  count = 0;
  Serial.println(F("\r\nEnter filename (send newline after input):"));
  do {
    while(!Serial.available()) ;
    c = Serial.read();
    if(c > ' ') AudioFileName[count++] = c;
  } while((c != 0x0d) && (c != 0x0a) && (count < 14));
  AudioFileName[count++] = 0;
  
  Serial.print(F("Looking for file... "));
  if(!SdPlay.setFile(AudioFileName)) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    goto ReEnter;
  } else {
   Serial.println(F("found.")); 
  }    

  Serial.println(F("Press s for stop, p for play, h for pause, f to select new file, d for deinit, v to view status.")); 
  flag = 1;
  
  while(flag) {
    SdPlay.worker();
    if(Serial.available()) {
      c = Serial.read();
      switch(c) {
         case 's':
           SdPlay.stop();
           Serial.println(F("Stopped."));
           break; 
           
         case 'p':
           SdPlay.play();
           Serial.println(F("Play."));
           break;
           
         case 'h':
           SdPlay.pause();
           Serial.println(F("Pause."));
           break;   
   
         case 'd':
           SdPlay.deInit();
           Serial.println(F("SdPlay deinitialized. You can now safely remove card. System halted."));
           while(1) ;
           break;  
             
         case 'f':
           flag = 0;
           break; 
     
         case 'v':
           Serial.print(F("Status: isStopped="));
           Serial.print(SdPlay.isStopped());
           Serial.print(F(", isPlaying="));
           Serial.print(SdPlay.isPlaying());
           Serial.print(F(", isPaused="));
           Serial.print(SdPlay.isPaused());
           Serial.print(F(", isUnderrunOccured="));
           Serial.print(SdPlay.isUnderrunOccured());
           Serial.print(F(", getLastError="));
           Serial.println(SdPlay.getLastError());
           Serial.print(F("Free RAM: "));
           Serial.println(freeRam());
           break;      
      } 
    }
  }
  
  
}
