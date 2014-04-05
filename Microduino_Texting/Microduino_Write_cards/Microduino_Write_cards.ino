#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#define IRQ                     (2)
#define RESET                   (3)     // Not connected by default on the NFC Shield

#define NR_SHORTSECTOR          (32)    // Number of short sectors on Mifare 1K/4K
#define NR_LONGSECTOR           (8)     // Number of long sectors on Mifare 4K
#define NR_BLOCK_OF_SHORTSECTOR (4)     // Number of blocks in a short sector
#define NR_BLOCK_OF_LONGSECTOR  (16)    // Number of blocks in a long sector

// Determine the sector trailer block based on sector number
#define BLOCK_NUMBER_OF_SECTOR_TRAILER(sector) (((sector)<NR_SHORTSECTOR)? \
  ((sector)*NR_BLOCK_OF_SHORTSECTOR + NR_BLOCK_OF_SHORTSECTOR-1):\
  (NR_SHORTSECTOR*NR_BLOCK_OF_SHORTSECTOR + (sector-NR_SHORTSECTOR)*NR_BLOCK_OF_LONGSECTOR + NR_BLOCK_OF_LONGSECTOR-1))

// Determine the sector's first block based on the sector number
#define BLOCK_NUMBER_OF_SECTOR_1ST_BLOCK(sector) (((sector)<NR_SHORTSECTOR)? \
  ((sector)*NR_BLOCK_OF_SHORTSECTOR):\
  (NR_SHORTSECTOR*NR_BLOCK_OF_SHORTSECTOR + (sector-NR_SHORTSECTOR)*NR_BLOCK_OF_LONGSECTOR))
  
  // For a http://www. url:
const char * url = "microduino.cc";
uint8_t ndefprefix = NDEF_URIPREFIX_HTTP_WWWDOT;

// for an email address
//const char * url = "mail@example.com";
//uint8_t ndefprefix = NDEF_URIPREFIX_MAILTO;

// for a phone number
//const char * url = "51508987";
//uint8_t ndefprefix = NDEF_URIPREFIX_TEL;

  uint8_t success_f,success_r;                          // Flag to check if there was an error with the PN532
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  bool authenticated = false;               // Flag to indicate if the sector is authenticated
  uint8_t blockBuffer[16];                  // Buffer to store block contents
  uint8_t blankAccessBits[3] = { 0xff, 0x07, 0x80 };
  uint8_t idx = 0;
  uint8_t numOfSector = 16;                 // Assume Mifare Classic 1K for now (16 4-block sectors)
  
char data;

// Create an instance of the NFCShield_I2C class
Adafruit_NFCShield_I2C nfc(IRQ, RESET);

void setup(void) {
  Serial.begin(115200);
  Serial.println("Looking for PN532...");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
}

void loop(void) {
   
  Serial.println("Place your NDEF formatted Mifare Classic 1K card on the reader");
  Serial.println("Please press 'f' to format.....");
  Serial.println("Please press 'r' to Rewrite data......");
  // Wait for user input before proceeding
  while (!Serial.available());
  while (Serial.available()) 
  {
  data=Serial.read();
  }
if(data=='f')
format();
else if(data=='r')
writ();
}
void format()
{
  
// The default Mifare Classic key
static const uint8_t KEY_DEFAULT_KEYAB[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

  success_f = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success_f) 
  {
    // We seem to have a tag ...
    // Display some basic information about it
    Serial.println("Found an ISO14443A card/tag");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");
    
    // Make sure this is a Mifare Classic card
    if (uidLength !=4)
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!"); 
      return;
    }    
    
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");
    Serial.println("");
    Serial.println("Reformatting card for Mifare Classic (please don't touch it!) ... ");

    // Now run through the card sector by sector
    for (idx = 0; idx < numOfSector; idx++)
    {
      // Step 1: Authenticate the current sector using key B 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      success_f = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, BLOCK_NUMBER_OF_SECTOR_TRAILER(idx), 1, (uint8_t *)KEY_DEFAULT_KEYAB);
      if (!success_f)
      {
        Serial.print("Authentication failed for sector "); Serial.println(numOfSector);
        return;
      }
      
      // Step 2: Write to the other blocks
      if (idx == 16)
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      if ((idx == 0) || (idx == 16))
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      else
      {
        memset(blockBuffer, 0, sizeof(blockBuffer));
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 3, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
        if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 2, blockBuffer)))
        {
          Serial.print("Unable to write to sector "); Serial.println(numOfSector);
          return;
        }
      }
      memset(blockBuffer, 0, sizeof(blockBuffer));
      if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)) - 1, blockBuffer)))
      {
        Serial.print("Unable to write to sector "); Serial.println(numOfSector);
        return;
      }
      
      // Step 3: Reset both keys to 0xFF 0xFF 0xFF 0xFF 0xFF 0xFF
      memcpy(blockBuffer, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));
      memcpy(blockBuffer + 6, blankAccessBits, sizeof(blankAccessBits));
      blockBuffer[9] = 0x69;
      memcpy(blockBuffer + 10, KEY_DEFAULT_KEYAB, sizeof(KEY_DEFAULT_KEYAB));

      // Step 4: Write the trailer block
      if (!(nfc.mifareclassic_WriteDataBlock((BLOCK_NUMBER_OF_SECTOR_TRAILER(idx)), blockBuffer)))
      {
        Serial.print("Unable to write trailer block of sector "); Serial.println(numOfSector);
        return;
      }
    }
  }
  
  // Wait a bit before trying again
  Serial.println("\n\nDone!");
  delay(1000);
  Serial.flush();
  while(Serial.available()) Serial.read();
}
void writ()
{

  // Use the default key
  uint8_t keya[6] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };

  Serial.println("");
  Serial.println("PLEASE NOTE: Formatting your card for NDEF records will change the");
  Serial.println("authentication keys.  To reformat your NDEF tag as a clean Mifare");
  Serial.println("Classic tag, use the mifareclassic_ndeftoclassic example!");
  Serial.println("");
  Serial.println("Place your Mifare Classic card on the reader to format with NDEF");
 // Serial.println("and press any key to continue ...");
  // Wait for user input before proceeding

  // Wait for an ISO14443A type card (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success_r = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success_r)
  {
    // Display some basic information about the card
    Serial.println("Found an ISO14443A card");
    Serial.print("  UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("  UID Value: ");
    nfc.PrintHex(uid, uidLength);
    Serial.println("");

    // Make sure this is a Mifare Classic card
    if (uidLength != 4)
    {
      Serial.println("Ooops ... this doesn't seem to be a Mifare Classic card!");
      return;
    }

    // We probably have a Mifare Classic card ...
    Serial.println("Seems to be a Mifare Classic card (4 byte UID)");

    // Try to format the card for NDEF data
    success_r = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 0, 0, keya);
    if (!success_r)
    {
      Serial.println("Unable to authenticate block 0 to enable card formatting!");
      return;
    }
    success_r = nfc.mifareclassic_FormatNDEF();
    if (!success_r)
    {
      Serial.println("Unable to format the card for NDEF");
      return;
    }

    Serial.println("Card has been formatted for NDEF data using MAD1");

    // Try to authenticate block 4 (first block of sector 1) using our key
    success_r = nfc.mifareclassic_AuthenticateBlock (uid, uidLength, 4, 0, keya);

    // Make sure the authentification process didn't fail
    if (!success_r)
    {
      Serial.println("Authentication failed.");
      return;
    }

    // Try to write a URL
    Serial.println("Writing URI to sector 1 as an NDEF Message");

    // Authenticated seems to have worked
    // Try to write an NDEF record to sector 1
    // Use 0x01 for the URI Identifier Code to prepend "http://www."
    // to the url (and save some space).  For information on URI ID Codes
    // see http://www.ladyada.net/wiki/private/articlestaging/nfc/ndef
    if (strlen(url) > 38)
    {
      // The length is also checked in the WriteNDEFURI function, but lets
      // warn users here just in case they change the value and it's bigger
      // than it should be
      Serial.println("URI is too long ... must be less than 38 characters long");
      return;
    }

    // URI is within size limits ... write it to the card and report success/failure
   success_r = nfc.mifareclassic_WriteNDEFURI(1, ndefprefix, url);
    if (success_r)
    {
      Serial.println("NDEF URI Record written to sector 1");
    }
    else
    {
      Serial.println("NDEF Record creation failed! :(");
    }
  }

  // Wait a bit before trying again
  Serial.println("\n\nDone!");
  delay(1000);
  Serial.flush();
  while(Serial.available()) Serial.read();
}
