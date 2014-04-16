#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "UsbKeyboard.h"

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 2 ;
// Address of the other node
const uint16_t other_node = 0;

// Structure of our payload
struct payload_t
{
  boolean nRF_sta;
  int nRF_x;
};

int num;

#define ledPin  6
boolean ledStatus=false;

void setup(void)
{
  Serial.begin(115200);

  TIMSK0 &= !(1 << TOIE0);

  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(ledPin, OUTPUT);
}

void loop(void)
{
  UsbKeyboard.update();
  network.update();

  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    ledStatus=payload.nRF_sta;
    num=payload.nRF_x;
  }
  digitalWrite(ledPin, ledStatus);
  // Pump the network regularly
  if(ledStatus==HIGH&&num==2)
  {
    delay(1000);
    if(ledStatus==HIGH)
      key_ylb();
  }
  else if(ledStatus==HIGH&&num==1)
  {
    delay(1000);
    if(ledStatus==HIGH)
      key_one();
  }

}

void key_ylb()
{
  {
    UsbKeyboard.sendKeyStroke(KEY_M);
    UsbKeyboard.sendKeyStroke(KEY_I);
    UsbKeyboard.sendKeyStroke(KEY_C);
    UsbKeyboard.sendKeyStroke(KEY_R);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_D);
    UsbKeyboard.sendKeyStroke(KEY_U);
    UsbKeyboard.sendKeyStroke(KEY_I);
    UsbKeyboard.sendKeyStroke(KEY_N);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_ENTER);
  }
}

void key_one()
{
  {
    UsbKeyboard.sendKeyStroke(KEY_clock);
    UsbKeyboard.sendKeyStroke(KEY_M);
    UsbKeyboard.sendKeyStroke(KEY_I);
    UsbKeyboard.sendKeyStroke(KEY_C);
    UsbKeyboard.sendKeyStroke(KEY_R);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_D);
    UsbKeyboard.sendKeyStroke(KEY_U);
    UsbKeyboard.sendKeyStroke(KEY_I);
    UsbKeyboard.sendKeyStroke(KEY_N);
    UsbKeyboard.sendKeyStroke(KEY_O);
    UsbKeyboard.sendKeyStroke(KEY_ENTER);
  }
}



