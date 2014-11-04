#include <Wire.h>
#include <Adafruit_NFCShield_I2C.h>

#include <Servo.h> 
Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 
int pos = 0;    // variable to store the servo position 

uint8_t key[] = {0x04,0x93,0xA5,0x92,0xA9,0x34,0x80};//replace your own key UID


#define IRQ (2)
#define RESET (3) // Not connected by default on the NFC Shield
Adafruit_NFCShield_I2C nfc(IRQ, RESET);

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
	Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX);
	Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC);
	Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
	// configure board to read RFID tags
	nfc.SAMConfig();
	Serial.println("Waiting for an ISO14443A Card ...");
	myservo.attach(9);  // attaches the servo on pin 9 to the servo object 
}

void loop(void) {
	uint8_t success;
	uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 }; // Buffer to store the returned UID
	uint8_t uidLength; // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
	// Wait for an ISO14443A type cards (Mifare, etc.). When one is found
	// 'uid' will be populated with the UID, and uidLength will indicate
	// if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
	success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
	if (success) {
		// Display some basic information about the card
		Serial.println("Found an ISO14443A card");
		Serial.print(" UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
		Serial.print(" UID Value: ");
		nfc.PrintHex(uid, uidLength);
		//nfc.PrintHexChar(uid, uidLength);
		if (uidLength == 7) {
                        int x=memcmp(uid,key,7);
  			if(x==0) {
				//control Servo
				myservo.write(90);              // tell servo to go to position in variable 'pos' 
				delay(5000);                       // waits 5s for the servo to reach the position 
				myservo.write(0);              // tell servo to go back 0 pos
			} else {
				Serial.println("Error: Your key is not right!!!");
			}
		} else {
			Serial.println("Error: UID Length is not 7!!!");
		}
	} else {
		Serial.println("read Passive Target ID Error!!!");
	}
}
