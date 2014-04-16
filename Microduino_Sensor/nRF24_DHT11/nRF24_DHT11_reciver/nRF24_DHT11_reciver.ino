#include "U8glib.h"

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);// definition OLED connection

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 0; 
// Address of the other node
const uint16_t other_node = 1;


float temperature=0.0;
float humidity=0.0;

// Structure of our payload
struct payload_t
{
  uint32_t ms;
  uint32_t temperature;
  uint32_t humidity;
};

//show speed
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

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);
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

    temperature=payload.temperature;
    humidity=payload.humidity;

    u8g.firstPage();
    do {
      draw();
    }
    while( u8g.nextPage() );
  }
}

