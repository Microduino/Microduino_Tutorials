#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <dht.h>
//Definition DHT
dht DHT;

float temperature=0.0;
float humidity=0.0;

#define DHT11_PIN 2//Sensor data interface to the digital pin2

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 1;
// Address of the other node
const uint16_t other_node = 0;
// How often to send 'hello world to the other unit
const unsigned long interval = 150; //ms
// When did we last send?
unsigned long last_sent;
// How many have we sent already
//unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  uint32_t ms;
  uint32_t temperature;
  uint32_t humidity;
};


void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);
}

void loop(void)
{
  int chk = DHT.read11(DHT11_PIN);
  
  temperature=(float)DHT.temperature;
  humidity=(float)DHT.humidity;

  // Pump the network regularly
  network.update();
  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    //Serial.print("Sending...");
    payload_t payload = { millis(),temperature, humidity};
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));

    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}


