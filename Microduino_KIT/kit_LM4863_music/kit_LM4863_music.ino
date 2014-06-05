
#include <SimpleSDAudio.h>

int num_one,num_two,ok;

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
  Serial.begin(115200);
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(0, blink, FALLING);

  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

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


}


void loop(void) {
  uint8_t count=0, c, flag;

  Serial.println(F("Files on card:"));
  SdPlay.dir(&DirCallback);

ReEnter: 
  count = 0;
  Serial.println(F("\r\nEnter filename (send newline after input):"));

  Serial.print(F("Looking for file... "));
  if(!SdPlay.setFile("1.AFM")) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    goto ReEnter;
  } 

  else {
    Serial.println(F("found.")); 
  }    

  Serial.println(F("Press s for stop, p for play, h for pause, f to select new file, d for deinit, v to view status.")); 

  while(1) {
    SdPlay.worker();
    if(num_two!=num_one)
    {
      num_two=num_one ;
      switch(ok) {
      case 0:
        SdPlay.play();
        Serial.println(F("Play."));
        break; 
      case 1:
        SdPlay.pause();
        Serial.println(F("Pause."));
        break;            
      }
      Serial.println(ok);
      Serial.print(num_one);
      Serial.println(num_two);
    }
  }
}

void blink()
{
  ok=!ok;
  num_one++;
  if(num_one>10)
    num_one=0;
}

