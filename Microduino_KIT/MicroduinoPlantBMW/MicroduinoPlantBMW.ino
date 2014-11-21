#include <SimpleSDAudio.h>

// Callback target, prints output to serial
void DirCallback(char *buf) {
  Serial.println(buf);
}
char AudioFileName[16];
// Create static buffer
#define BIGBUFSIZE (2*512) // bigger than 2*512 is often only possible on Arduino megas!
uint8_t bigbuf[BIGBUFSIZE];
// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval;
  int v;
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
}


int ledPin = 9;
int capval;
void setup()
{
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);


  //pinMode(2, INPUT_PULLUP);
  //attachInterrupt(0, blink, FALLING);
  while (!Serial) {
  ; // wait for serial port to connect. Needed for Leonardo only
}
Serial.println("Touch senser");
Serial.print(F("Free Ram: "));
Serial.println(freeRam());
  // Setting the buffer manually for more flexibility
SdPlay.setWorkBuffer(bigbuf, BIGBUFSIZE);
Serial.print(F("\nInitializing SimpleSDAudio V" SSDA_VERSIONSTRING " ..."));
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  //SdPlay.setSDCSPin(10);
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
}
else {
  Serial.println(F("Wiring is correct and a card is present."));
}




  Serial.println(F("Files on card:"));
  SdPlay.dir(&DirCallback);
  ReEnter:
  Serial.println(F("\r\nEnter filename (send newline after input):"));
  Serial.print(F("Looking for file... "));
  if(!SdPlay.setFile("2.AFM")) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    goto ReEnter;
  }
  else {
    Serial.println(F("found."));
  }
  Serial.println(F("Press s for stop, p for play, h for pause, f to select new file, d for deinit, v to view status."));






}

void loop ()
{



  digitalWrite(ledPin,LOW);
  capval = readCapacitivePin(8);
  Serial.println(capval, DEC);
  if (capval > 2) {
    // turn LED on:
    digitalWrite(ledPin, HIGH);
    SdPlay.worker();

    SdPlay.play();
    Serial.println(F("Play."));


    delay(5000);
/*
    SdPlay.pause();
    Serial.println(F("Pause."));
*/
  }
}
uint8_t readCapacitivePin(int pinToMeasure) {
  // Variables used to translate from Arduino to AVR pin naming
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  // Arduino pin number to the AVR PORT, PIN, DDR,
  // and which bit of those registers we care about.
  byte bitmask;
  port = portOutputRegister(digitalPinToPort(pinToMeasure));
  ddr = portModeRegister(digitalPinToPort(pinToMeasure));
  bitmask = digitalPinToBitMask(pinToMeasure);
  pin = portInputRegister(digitalPinToPort(pinToMeasure));
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr |= bitmask;
  delay(1);
  // Make the pin an input with the internal pull-up on
  *ddr &= ~(bitmask);
  *port |= bitmask;
  // Now see how long the pin to get pulled up. This manual unrolling of the loop
  // decreases the number of hardware cycles between each read of the pin,
  // thus increasing sensitivity.
  uint8_t cycles = 17;
  if (*pin & bitmask) { 
    cycles = 0;
  }
  else if (*pin & bitmask) { 
    cycles = 1;
  }
  else if (*pin & bitmask) { 
    cycles = 2;
  }
  else if (*pin & bitmask) { 
    cycles = 3;
  }
  else if (*pin & bitmask) { 
    cycles = 4;
  }
  else if (*pin & bitmask) { 
    cycles = 5;
  }
  else if (*pin & bitmask) { 
    cycles = 6;
  }
  else if (*pin & bitmask) { 
    cycles = 7;
  }
  else if (*pin & bitmask) { 
    cycles = 8;
  }
  else if (*pin & bitmask) { 
    cycles = 9;
  }
  else if (*pin & bitmask) { 
    cycles = 10;
  }
  else if (*pin & bitmask) { 
    cycles = 11;
  }
  else if (*pin & bitmask) { 
    cycles = 12;
  }
  else if (*pin & bitmask) { 
    cycles = 13;
  }
  else if (*pin & bitmask) { 
    cycles = 14;
  }
  else if (*pin & bitmask) { 
    cycles = 15;
  }
  else if (*pin & bitmask) { 
    cycles = 16;
  }
  // Discharge the pin again by setting it low and output
  // It's important to leave the pins low if you want to
  // be able to touch more than 1 sensor at a time - if
  // the sensor is left pulled high, when you touch
  // two sensors, your body will transfer the charge between
  // sensors.
  *port &= ~(bitmask);
  *ddr |= bitmask;
  return cycles;
}

