#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;
// How often to send 'hello world to the other unit
#define interval  200
// When did we last send?
unsigned long last_sent;

int x;

// Structure of our payload
struct payload_t
{
  boolean nRF_sta;
  int nRF_x;
};

boolean ledStatus;

long dataCard_one[4]={
  147,140,239,170};
long dataCard_two[4]={
  243,95,237,170};

boolean success;
uint8_t uid[] = {
  0, 0, 0, 0, 0, 0, 0           };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)


void setup () {

  Serial.begin(115200);
  Serial.println("WebLed Demo");

  Serial.println();

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
  TIMSK0 &= !(1 << TOIE0); 
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }

  // Got ok data, print it out!
  Serial.print("Found chip PN5"); 
  Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); 
  Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); 
  Serial.println((versiondata>>8) & 0xFF, DEC);

  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
  nrf();
}

void loop() {

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);

  if (success) {
    //  Serial.println("Found a card!");
    //  Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(" 0x");
      Serial.print(uid[i]); 
    }
    if(uid[0]==dataCard_one[0]&&uid[1]==dataCard_one[1]&&uid[2]==dataCard_one[2]&&uid[3]==dataCard_one[3])
    {
      x=2;
      ledStatus=true;
      nrf();
    }
    else if(uid[0]==dataCard_two[0]&&uid[1]==dataCard_two[1]&&uid[2]==dataCard_two[2]&&uid[3]==dataCard_two[3])
    {
      x=1;
      ledStatus=true;
      nrf();
    }
    // Wait 1 second before continuing
    // delay(10);
  }
  else
  {
    Serial.println("Timed out waiting for a card");
  }

  Serial.println("");
  ledStatus=false;
  nrf();
}

void nrf()
{
  network.update();
  // If it's time to send a message, send it!

  Serial.println("Sending...");
  payload_t payload = {
    ledStatus,x                                                                         };
  RF24NetworkHeader header(x);    //1,2,3,4
  bool ok = network.write(header,&payload,sizeof(payload));
  Serial.println(x);
  if (ok)
    Serial.println(" ..ok.");
  else
    Serial.println(" ..failed.");
}

