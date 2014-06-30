#include "EEPROM.h"
#include "cc1101.h"

#define LEDOUTPUT 4  // Arduino's digital pin 4 = panStamp's pin 19

// The syncWord of sender and receiver must be the same
byte syncWord[] = {19, 9};
byte senderAddress = 4;
byte receiverAddress = 5;

// The connection to the hardware chip CC1101 the RF Chip
CC1101 cc1101;

// counter to get increment in each loop
byte counter;

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
* The setup method gets called on start of the system.
*/
void setup()
{
  Serial.begin(38400);
  Serial.println("start");

  // setup the blinker output
  pinMode(LEDOUTPUT, OUTPUT);
  digitalWrite(LEDOUTPUT, LOW);

  // blink once to signal the setup
  blinker();
  
  // reset the counter
  counter=0;
  
  // initialize the RF Chip
  cc1101.init();
  // Set default carrier frequency = 433 MHz£¬you can choose CFREQ_433¡¢CFREQ_868¡¢CFREQ_915
  cc1101.setCarrierFreq(CFREQ_433);
  cc1101.setSyncWord(syncWord, false);
  cc1101.setDevAddress(senderAddress, false);
  Serial.println("device initialized");

  Serial.print("set sender address to ");
  Serial.println(senderAddress);
  Serial.println("done");
}

/**
* The loop method gets called on and on after the start of the system.
*/
void loop()
{
  Serial.println("loop");
  
  CCPACKET data;
  data.length=2;
  byte blinkCount=counter++;
  // the first byte in the data is the destination address
  // it must match the device address of the receiver
  Serial.println("set address to ");
  data.data[0]=receiverAddress;
  data.data[1]=blinkCount;
  if(cc1101.sendData(data)){
    Serial.print("ok ");
    Serial.println(blinkCount);
    for(int j=0; j<blinkCount; j++){
      blinker();
    }
  }else{
    Serial.print("failed ");
    Serial.println(blinkCount);    
    blinker();
    blinker();
  }  
  Serial.println("loop done");    
  delay(2000);
}