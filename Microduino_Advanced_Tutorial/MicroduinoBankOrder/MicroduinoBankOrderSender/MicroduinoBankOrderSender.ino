#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>
#include <Servo.h>

int pos = 0; // variable to store the servo position
uint8_t key[] = {
  0x04,0x93,0xA5,0x92,0xA9,0x34,0x80};//replace your own key UID
#define IRQ (2)
#define RESET (3) // Not connected by default on the NFC Shield
  Adafruit_NFCShield_I2C nfc(IRQ, RESET);


// nRF24L01(+) radio attached using Getting Started board
  RF24 radio(9,10);
// Network uses that radio
  RF24Network network(radio);
// Address of our node
  const uint16_t this_node = 1;
// Address of the other node
const uint16_t other_node = 0;
// How often to send 'hello world to the other unit
  #define interval 200
// When did we last send?
  unsigned long last_sent;
  int x;
  int num;

  boolean ledStatus;
// Structure of our payload
  struct payload_t {
    boolean nRF_sta;
    int nRF_x;
  };


  void setup(void) {
    Serial.begin(115200);
    Serial.println("Hello!");

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
  // configure board to read RFID tags
  nfc.SAMConfig();
  Serial.println("Waiting for an ISO14443A Card ...");
  nrf();
}



void loop(void) {
  uint8_t success;
  uint8_t uid[] = { 
    0, 0, 0, 0, 0, 0, 0   }; // Buffer to store the returned UID
  uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  // Wait for an ISO14443A type cards (Mifare, etc.). When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  if (success) {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print(" UID Length: ");
    Serial.print(uidLength, DEC);
    Serial.println(" bytes");
    Serial.print(" UID Value: ");
    nfc.PrintHex(uid, uidLength);
    //nfc.PrintHexChar(uid, uidLength);
    if (uidLength == 7) {
      int x=memcmp(uid,key,7);
      if(x==0) {
        num=1;
        ledStatus=true;
        nrf();
        delay(5000);
      } 
      else {
        Serial.println("Error: Your key is not right!!!");
      }
    } 
    else {
      Serial.println("Error: UID Length is not 7!!!");
    }
  } 
  else {
    Serial.println("read Passive Target ID Error!!!");
  }
}


void nrf() {
  network.update();
// If it's time to send a message, send it!
  Serial.println("Sending...");
  payload_t payload = {
    ledStatus,num };
  RF24NetworkHeader header(other_node); //1,2,3,4
  bool ok = network.write(header,&payload,sizeof(payload));
  Serial.println(num);
  if (ok)
    Serial.println(" ..ok.");
  else
    Serial.println(" ..failed.");
}