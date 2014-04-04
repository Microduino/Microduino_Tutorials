#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)
/*
font:  
 u8g.setFont(u8g_font_7x13)
 u8g.setFont(u8g_font_fixed_v0r);
 u8g.setFont(u8g_font_chikitar);
 u8g.setFont(u8g_font_osb21);
 */

#include "EEPROM.h"
#include "cc1101.h"

// The LED is wired to the Arduino Output 4 (physical panStamp pin 19)
#define LEDOUTPUT 4
// The syncWord of sender and receiver must be the same
byte syncWord[] = {
  19, 9};
byte receiverAddress = 5;

// The connection to the hardware chip CC1101 the RF Chip
CC1101 cc1101;

CCPACKET packet;

// a flag that a wireless packet has been received 
boolean packetAvailable = false;         

/**
 * Let the LED Output blink one time.
 * 
 * With small pause after the blink to see two consecutive blinks. 
 */
void blinker(){
  digitalWrite(LEDOUTPUT, HIGH);
  delay(100);
  digitalWrite(LEDOUTPUT, LOW);
  delay(100);
}

/**
 * Handle interrupt from CC1101 (INT0)
 */
void cc1101signalsInterrupt(void){
  // set the flag that a package is available
  packetAvailable = true;
}

/**
 * The setup method gets called on start of the system.
 */
void setup()
{
  Serial.begin(115200);
  Serial.println("start");

  // setup the blinker output
  pinMode(LEDOUTPUT, OUTPUT);
  digitalWrite(LEDOUTPUT, LOW);

  // blink once to signal the setup
  blinker();

  // initialize the RF Chip
  cc1101.init();
  cc1101.setSyncWord(syncWord, false);

  // this device address need to match the target address in the sender
  cc1101.setDevAddress(receiverAddress, false);
  cc1101.enableAddressCheck(); // you can skip this line, because the default is to have the address check enabled
  cc1101.setRxState();
  // Enable wireless reception interrupt
  attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  Serial.println("device initialized");

  Serial.println("setup done");
}

/**
 * The loop method gets called on and on after the start of the system.
 */
void loop()
{
  if(packetAvailable){
    // clear the flag
    packetAvailable = false;


    // Disable wireless reception interrupt
    detachInterrupt(0);

    if(cc1101.receiveData(&packet) > 0){
      if (packet.crc_ok && packet.length > 1){
        Serial.print("packet: len");
        Serial.print(packet.length);
        Serial.print(" data1: ");
        Serial.println(packet.data[1]);
        blinker();
      }
    }

    // Enable wireless reception interrupt
    attachInterrupt(0, cc1101signalsInterrupt, FALLING);
  }

  u8g.firstPage();
  do
  {
    draw();
  }
  while( u8g.nextPage() );

}

void draw(void)
{
  //---------------------------------
  setFont_L;
  u8g.setPrintPos(10, 15); 
  u8g.print("Rec. pack #");
  u8g.print(packet.data[1]);
}



