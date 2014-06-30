#define NUM 2

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = NUM; 
// Address of the other node
const uint16_t other_node = 0;

const unsigned long interval = 150; //ms
// When did we last send?
unsigned long last_sent;
// How many have we sent already
//unsigned long packets_sent;


// Structure of our payload
struct payload_r
{
  uint32_t sensorDataA;
};

struct payload_t
{
  uint32_t ms;
  uint32_t sensorDataB;
};

#include <OneWire.h>
//18B20
#define ONEWIRE_PIN A4
OneWire ds(ONEWIRE_PIN);

#define power_PIN 5
#define led_PIN 6

boolean power_SW;
int temp_VOL;

unsigned long temp_time=millis();


void setup(void)
{
  Serial.begin(115200);

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);

  pinMode(power_PIN,OUTPUT);
  pinMode(led_PIN,OUTPUT);
}

void loop(void)
{  

  if(temp_time>millis()) temp_time=millis();
  if(millis()-temp_time>1000)
  {
    temp_VOL=get_Current_Temp();
    Serial.print("temp_VOL:");
    Serial.println(temp_VOL);

    temp_time=millis();
  }
  // Pump the network regularly
  network.update();
  // Is there anything ready for us?
  while ( network.available() )
  {
    // If so, grab it and print it out
    RF24NetworkHeader header;
    payload_r payload;
    network.read(header,&payload,sizeof(payload));
    power_SW=payload.sensorDataA;

    {
      digitalWrite(led_PIN,HIGH);

      Serial.print("Sending...");
      payload_t payload = 
      { 
        millis(),temp_VOL                    
      };
      RF24NetworkHeader header(/*to node*/ other_node);
      bool ok = network.write(header,&payload,sizeof(payload));
      if (ok)
        Serial.println("ok.");
      else
        Serial.println("failed.");

      digitalWrite(led_PIN,LOW);
    }
  }


  digitalWrite(power_PIN,power_SW);
}

float get_Current_Temp(){ //0.1C
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];
  if ( !ds.search(addr)) {
    //no more sensors on chain, reset search
    ds.reset_search();
    return 0;
  }
  if ( OneWire::crc8( addr, 7) != addr[7]) {
    return 1000;
  }
  if ( addr[0] != 0x28) {
    return 1000;
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x44,1); // start conversion, with parasite power on at the end
  delay(100);
  ds.reset();
  ds.select(addr);  
  ds.write(0xBE); // Read Scratchpad

  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = ds.read();
  }
  ds.reset_search();
  float tempRead = (data[1] << 8) | data[0]; //using two's compliment
  return (float)tempRead/16.0;
}



