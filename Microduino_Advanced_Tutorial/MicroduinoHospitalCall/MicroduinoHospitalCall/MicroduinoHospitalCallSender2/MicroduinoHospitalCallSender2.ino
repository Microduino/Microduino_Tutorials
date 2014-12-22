#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 2;
// Address of the other node
const uint16_t other_node = 0;
// How often to send 'hello world to the other unit
const unsigned long interval = 150; //ms
// When did we last send?
unsigned long last_sent;

#define CALLPIN 2 //number of call pin

// Structure of our payload
struct payload_t {
  uint32_t ms;
  uint32_t sensorDataA;
  uint32_t sensorDataB;
};

boolean power_SW;
int bedNum;

void setup(void) {
  //Serial.begin(115200);
  //Serial.println("sender2 init");
  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);
  //randomSeed(analogRead(0));
  pinMode(CALLPIN,INPUT_PULLUP); 
}

void loop(void) {
  power_SW=digitalRead(CALLPIN);
  bedNum=25;
  // Pump the network regularly
  network.update();
  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  ) {
    last_sent = now;
    //Serial.print("power_SW:");
    //Serial.println(power_SW);
    //Serial.print("bedNum:");
    //Serial.println(bedNum);
    if(power_SW) {
        //Serial.print("Sending...");
      payload_t payload = { 
        millis(),power_SW,bedNum    };
        RF24NetworkHeader header(/*to node*/ other_node);
        bool ok = network.write(header,&payload,sizeof(payload));
        /*
        if (ok)
          Serial.println("ok.");
        else
          Serial.println("failed.");
        */
      }
    }
  }


