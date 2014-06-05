#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 0; 
// Address of the other node
const uint16_t other_node = 1;

// Structure of our payload
struct payload_t
{
  boolean sensorData;
};

#define led_PIN 6

int led_VOL;

void setup(void)
{
  Serial.begin(115200);

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);

  pinMode(led_PIN,OUTPUT);
}

void loop(void)
{  
  // Pump the network regularly
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    led_VOL=payload.sensorData;
  }
  //analogWrite(led_PIN,led_VOL);
  digitalWrite(led_PIN,led_VOL);
  //Serial.println(led_VOL);
}
