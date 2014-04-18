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

  Serial.println("RF24Network/examples/helloworld_rx/");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(ledPin, OUTPUT);
  delay(1000);
  Keyboard.begin();
  Keyboard.write(KEY_LEFT_SHIFT);//microduino
}

void loop(void)
{
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
    delay(100);
    if(ledStatus==HIGH)
    key_ylb();
  }
  else if(ledStatus==HIGH&&num==1)
  {
    delay(100);
    if(ledStatus==HIGH)
    key_one();
  }

}

void key_ylb()
{
 Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write('m');
  Keyboard.write('i');
  Keyboard.write('c');
  Keyboard.write('r');
  Keyboard.write('o');
  Keyboard.write('d');
  Keyboard.write('u');
  Keyboard.write('i');
  Keyboard.write('n');
  Keyboard.write('o');
  Keyboard.releaseAll();
  Keyboard.write(KEY_RETURN);
}

void key_one()
{
  Keyboard.write('m');
  Keyboard.write('i');
  Keyboard.write('c');
  Keyboard.write('r');
  Keyboard.write('o');
  Keyboard.write('d');
  Keyboard.write('u');
  Keyboard.write('i');
  Keyboard.write('n');
  Keyboard.write('o');
  Keyboard.write(KEY_RETURN);
}


