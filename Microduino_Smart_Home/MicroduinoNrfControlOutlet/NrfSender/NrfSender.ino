
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#define buttonPin 2
// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
int lastState=0;

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 1;

// Address of the other node
const uint16_t other_node = 0;

// How often to send 'hello world to the other unit
const unsigned long interval = 2000; //ms

// When did we last send?
unsigned long last_sent;

// How many have we sent already
unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  //unsigned long ms;
  unsigned long counter;
};

void setup(void)
{
  Serial.begin(57600);
  Serial.println("begin");

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);

  pinMode(buttonPin, OUTPUT);
  digitalWrite(buttonPin,LOW);
}

void loop(void)
{
  // Pump the network regularly
  network.update();

  // If it's time to send a message, send it!
  //unsigned long now = millis();

  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {     
    packets_sent=1;

    if(lastState!=packets_sent) {
      sendMessage();
    }
    lastState=1;
  } 
  else {
    packets_sent=0;
    if(lastState!=packets_sent) {
      sendMessage();
    }
    lastState=0;
  }
}



void sendMessage() {
  Serial.print(packets_sent);
  Serial.print("--");
  Serial.print(lastState);
  //payload_t payload = { millis(), packets_sent };
  payload_t payload = { packets_sent };
    RF24NetworkHeader header(/*to node*/ other_node);
  bool ok = network.write(header,&payload,sizeof(payload));
  if (ok)
    Serial.println("ok.");
  else
    Serial.println("failed.");
}
