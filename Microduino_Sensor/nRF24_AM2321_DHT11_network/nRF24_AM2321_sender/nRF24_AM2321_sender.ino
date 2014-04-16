#include "U8glib.h"

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

#include <Wire.h>
#include <AM2321.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);// definition OLED connection

AM2321 am2321;

float temperature=0.0;
float humidity=0.0;


// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 2;
// Address of the other node
const uint16_t server_node = 0;
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
  uint32_t nodeNum;
  uint32_t temperature;
  uint32_t humidity;
};



//show Info
void draw() {
  u8g.setFont(u8g_font_courB14);//font
  u8g.setPrintPos(0, 20);
  u8g.print("Temp:");
  u8g.print(temperature);
  u8g.print("`c");
  u8g.setPrintPos(0, 54);
  u8g.print("AH:");
  u8g.print(humidity);
  u8g.print("%");
} 

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

  am2321.read();
  
  temperature=(float)am2321.temperature/10.0;
  humidity=(float)am2321.humidity/10.0;


  // Pump the network regularly
  network.update();
  // If it's time to send a message, send it!
  unsigned long now = millis();
  if ( now - last_sent >= interval  )
  {
    last_sent = now;

    //Serial.print("Sending...");
    payload_t payload = { millis(),this_node,temperature, humidity};
    RF24NetworkHeader header(/*to node*/ server_node);
    bool ok = network.write(header,&payload,sizeof(payload));

    if (ok)
      Serial.println("ok.");
    else
      Serial.println("failed.");
  }


    u8g.firstPage();
    do {
      draw();
    }
    while( u8g.nextPage() );
}


