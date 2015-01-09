#include <U8glib.h>
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
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



//-------字体设置，大、中、小
//#define setFont_L u8g.setFont(u8g_font_fur25)
#define setFont_L u8g.setFont(u8g_font_fur20)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar) 

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

// nRF24L01(+) radio attached using Getting Started board
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
boolean buttonEnable=true;
#define ARRAY_SIZE 100
int lineNubers[ARRAY_SIZE];

int currentNumber=0;

// Address of our node
const uint16_t this_node = 0 ;
// Address of the other node
const uint16_t other_node = 1;
// Structure of our payload
struct payload_t {
  boolean nRF_sta;
  int nRF_x;
};

int num;
#define nextPin 2
boolean ledStatus=false;

int buttonState = 0;         // variable for reading the pushbutton status

void setup(void) {
  Serial.begin(115200);






  // Using F("...") to avoid wasting RAM
  Serial.print(F("\nInitializing SD card..."));
  // If your SD card CS-Pin is not at Pin 7, enable and adapt the following line:
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
  if(!SdPlay.setFile("1.AFM")) {
  Serial.println(F(" not found on card! Error code: "));
  Serial.println(SdPlay.getLastError());
  while(1);
  } else {
  Serial.println(F("found."));
  } 

  TIMSK0 &= !(1 << TOIE0);
  Serial.println("RF24Network/examples/helloworld_rx/");
  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  pinMode(nextPin, INPUT);

}

void loop(void) {


  //if next button push down
    buttonState=digitalRead(nextPin);
    if((buttonState == HIGH) && buttonEnable) {
      for(int i=0; i<ARRAY_SIZE-1; i++) {
        if(lineNubers[i]!=0) {
          lineNubers[i]=lineNubers[i+1];
        }
      }
      currentNumber=lineNubers[0];
      buttonEnable=false;

      playSound();


  }else {
    buttonEnable=true;
  }


  network.update();
  // Is there anything ready for us?
  while ( network.available() ) {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    ledStatus=payload.nRF_sta;
    num=payload.nRF_x;

  //if someone book order
  if(ledStatus==HIGH) {
    for(int i=0; i<ARRAY_SIZE; i++) {
      if(i==0) {//if this person is first
        lineNubers[i]=1;
      }else {//other person except first person
        if(lineNubers[i]==0) {
          lineNubers[i]=lineNubers[i-1]+1;//pervious person number + 1
          break;
        }
      }
    }
    currentNumber=lineNubers[0];
  }
}

    //OLED display
      u8g.firstPage();
      do {
        draw();
      } while( u8g.nextPage() );

/*
for(int i=0; i<ARRAY_SIZE; i++) {
  if(lineNubers[i]!=0) {
    Serial.print(lineNubers[i]);
    Serial.print(", ");
  }
}
*/

//Serial.println();
delay(5000);
}

void draw(void) {
  setFont_L; //设置字体为大

  u8g.setPrintPos(2, 30); //设置文字开始坐标
  u8g.print("NUM:");
  u8g.setPrintPos(80, 30); //设置文字开始坐标
  u8g.print(currentNumber , 1); //床号

  setFont_M;
  u8g.setPrintPos(40, 60); //设置文字开始坐标
  u8g.print("come here"); 
}


void playSound(void) {
Serial.print(F("Playing... "));
SdPlay.play();
while(!SdPlay.isStopped()) {
SdPlay.worker();
}
Serial.println(F("done."));
SdPlay.deInit();
}