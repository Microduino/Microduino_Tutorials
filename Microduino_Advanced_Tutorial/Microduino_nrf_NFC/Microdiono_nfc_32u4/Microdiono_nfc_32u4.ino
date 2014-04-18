#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ   (2)
#define RESET (3)  // Not connected by default on the NFC Shield

Adafruit_NFCShield_I2C nfc(IRQ, RESET);

int x;

boolean success;
uint8_t uid[] = { 
  0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)

long dataCard_one[4]={
  147,140,239,170};
long dataCard_two[4]={
  243,95,237,170};

void setup(void) {
  Serial.begin(115200);
  Serial.println("Hello!");

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

  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for an ISO14443A card");
  Keyboard.begin();
  Keyboard.write(KEY_LEFT_SHIFT);//microduino
}

void loop(void) {

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
      x=1;
      key_one();
    }
    else if(uid[0]==dataCard_two[0]&&uid[1]==dataCard_two[1]&&uid[2]==dataCard_two[2]&&uid[3]==dataCard_two[3])
    {
      x=2;
      key_two();
    }
    delay(500);
  }

  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
  }
}

void key_two()
{
  Keyboard.write('m');
  Keyboard.write('i');
  Keyboard.write('c');
  Keyboard.write('r');
  Keyboard.write('o');
  Keyboard.write('d');
  Keyboard.write('u');
  Keyboard.write('i');
  Keyboard.write('n');
  Keyboard.write('o');
  Keyboard.write(KEY_RETURN);
}
void key_one()
{
  Keyboard.press(KEY_LEFT_SHIFT);
  Keyboard.write('m');
  Keyboard.write('i');
  Keyboard.write('c');
  Keyboard.write('r');
  Keyboard.write('o');
  Keyboard.write('d');
  Keyboard.write('u');
  Keyboard.write('i');
  Keyboard.write('n');
  Keyboard.write('o');
  Keyboard.releaseAll();
  Keyboard.write(KEY_RETURN);
}





