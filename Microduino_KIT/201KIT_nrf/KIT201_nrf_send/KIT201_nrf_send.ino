#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

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
// Structure of our payload
struct payload_t
{
  uint32_t sensorData;
};

#define button_up 6
#define button_down 7

int led_VOL;
int up,down;

void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);
  randomSeed(analogRead(0));
  pinMode(button_up,INPUT_PULLUP); 
  pinMode(button_down,INPUT_PULLUP); 
}

void loop(void)
{
  up =digitalRead(button_up);        
  down = digitalRead(button_down);
  if (up==LOW)      //外部上拉，初始化为高，按下为低                       
  { 
    led_VOL+=10;                         
    if (led_VOL>=255) 
    {
      led_VOL=255;
    }   
    delay (300);    
  }
  if (down==LOW)       
  {
    led_VOL-=10;
    if (led_VOL<=0) {
      led_VOL=0;
    }
    delay (300);
  }
  // Pump the network regularly
  network.update();
  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    Serial.print("led_VOL:");
    Serial.println(led_VOL);

    Serial.print("Sending...");
    payload_t payload = { 
      led_VOL                            };
    RF24NetworkHeader header(/*to node*/ other_node);
    bool ok = network.write(header,&payload,sizeof(payload));
    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }
}

