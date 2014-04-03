#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <Wire.h>
#include "RTClib.h"
#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	

//定义microduino图标大小
#define u8g_logo_width 128
#define u8g_logo_height 18

const unsigned char u8g_logo_bits[] U8G_PROGMEM = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0x01,0xE0,
  0x03,0x00,0x00,0x00,0x00,0x7E,0x00,0xF0,0x01,0x00,0x00,0x00,
  0x00,0xFE,0xF9,0xF7,0x07,0x00,0x00,0x00,0x00,0x3C,0x00,0xF8,
  0x03,0x00,0x00,0x00,0x00,0xFC,0xF9,0xE1,0x03,0x00,0x00,0x00,
  0x00,0x38,0x00,0xF0,0x01,0x00,0x00,0x00,0x00,0xFC,0xFF,0x01,
  0x00,0x00,0x00,0x00,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0xFC,0xEF,0xF9,0x8F,0xD7,0x73,0xF1,0xC1,0x3B,0x9F,0xFF,
  0xFF,0x1E,0x3E,0x00,0x00,0xBC,0xEF,0xC1,0xE1,0x9F,0xFF,0xDD,
  0xE3,0x3F,0xCC,0xE1,0xF0,0xBF,0x7B,0x00,0x00,0x3C,0xF7,0xE1,
  0xE1,0x9F,0xFF,0xC6,0xF7,0x3E,0x8E,0xF3,0xF0,0xFF,0xF8,0x00,
  0x00,0x3C,0xF3,0xE1,0xF1,0x93,0xFF,0xE6,0xF7,0x3C,0x8F,0xF7,
  0xF0,0xFF,0xFC,0x00,0x00,0x7C,0xF2,0xE1,0xF1,0x83,0x87,0xFE,
  0xF7,0x39,0xFF,0xF7,0xF0,0xFF,0xFF,0x00,0x00,0x7C,0xF0,0xE3,
  0xF3,0xA3,0x03,0xFE,0xF7,0x3F,0xFF,0xF7,0x71,0xFC,0xFF,0x00,
  0x00,0x7C,0xF8,0xE3,0xF3,0xBF,0x03,0xFE,0xE3,0x3F,0xFF,0xF3,
  0x71,0xDC,0x7F,0x00,0x00,0x7E,0xFC,0xE7,0xE3,0xBF,0x03,0xFC,
  0xE3,0x3F,0xFE,0xF3,0x71,0x9C,0x7F,0x00,0x00,0xC1,0x03,0xF8,
  0xCF,0xE7,0x0F,0xF0,0x00,0x7F,0xFC,0xFC,0xFF,0x3E,0x1E,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
};//microduino图标

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   2  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIV2); // you can change this clock speed but DI

#define WLAN_SSID       "HiWiFi_ylb"        // cannot be longer than 32 characters!
#define WLAN_PASS       "13667928123"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

Adafruit_CC3000_Client client;

const unsigned long
connectTimeout  = 15L * 1000L, // Max time to wait for server connection
responseTimeout = 15L * 1000L; // Max time to wait for data from server
int
countdown       = 0;  // loop() iterations until next time server query
unsigned long
lastPolledTime  = 0L, // Last value retrieved from time server
sketchTime      = 0L; // CPU milliseconds since last server query

long time_year,time_month,time_day,time_hour,time_minute,time_second;
unsigned long currentTime;

void showDate(const DateTime& dt) {
  time_year=dt.year();
  time_month=dt.month();
  time_day=dt.day();
  time_hour=dt.hour();
  time_minute=dt.minute();
  time_second=dt.second();

  Serial.print(dt.year(), DEC);
  Serial.print('/');
  Serial.print(dt.month(), DEC);
  Serial.print('/');
  Serial.print(dt.day(), DEC);
  Serial.print(' ');
  Serial.print(dt.hour(), DEC);
  Serial.print(':');
  Serial.print(dt.minute(), DEC);
  Serial.print(':');
  Serial.print(dt.second(), DEC);   
  Serial.println();

}

void setup(void)
{
  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n")); 
  volcdsetup("Hello CC3000!",20,40);//开机显示
  delay(1000);
  displayDriverMode();

  Serial.println(F("\nInitialising the CC3000 ..."));
  if (!cc3000.begin()) {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    for(;;);
  }

  uint16_t firmware = checkFirmwareVersion();
  if ((firmware != 0x113) && (firmware != 0x118)) {
    Serial.println(F("Wrong firmware version!"));
    for(;;);
  }
  volcdsetup(" MAC_Address ",20,40);//开机显示
  delay(1000);
  displayMACAddress();

  Serial.println(F("\nDeleting old connection profiles"));
  if (!cc3000.deleteProfiles()) {
    Serial.println(F("Failed!"));
    while(1);
  }

  /* Attempt to connect to an access point */
  char *ssid = WLAN_SSID;             /* Max 32 chars */
  Serial.print(F("\nAttempting to connect to ")); 
  Serial.println(ssid);
  volcdsetup(ssid,20,40);//开机显示
  delay(1000); 

  /* NOTE: Secure connections are not available in 'Tiny' mode! */
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }

  Serial.println(F("Connected!"));
  volcdsetup("Request DHCP ",20,40);//开机显示
  delay(1000);
  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP()) {
    delay(100); // ToDo: Insert a DHCP timeout!
  }

  /* Display the IP address DNS, Gateway, etc. */
  while (!displayConnectionDetails()) {
    delay(1000);
  }
  volcdsetup("  Welcome! ",20,40);//开机显示
  delay(1000);
}


// To reduce load on NTP servers, time is polled once per roughly 24 hour period.
// Otherwise use millis() to estimate time since last query.  Plenty accurate.
void loop(void) {
  if(countdown == 0) {            // Time's up?
    unsigned long t  = getTime(); // Query time server
    if(t) {                       // Success?
      lastPolledTime = t;         // Save time
      sketchTime     = millis();  // Save sketch time of last valid time query
      countdown      = 24*60*4-1; // Reset counter: 24 hours * 15-second intervals
    }
  } 
  else {
    countdown--;                  // Don't poll; use math to figure current time
  }
  currentTime = lastPolledTime + (millis() - sketchTime) / 1000;

  Serial.print(F("UNIX time: "));
  Serial.print(currentTime);
  Serial.println(F("s"));
  Serial.print(F("Data time: "));
  showDate(currentTime+28800);
  //delay(1000L); 
  u8g.firstPage();  
  do {
    draw();
  } 
  while( u8g.nextPage() );
}


/**************************************************************************/
/*!
 @brief  Displays the driver mode (tiny of normal), and the buffer
 size if tiny mode is not being used
 
 @note   The buffer size and driver mode are defined in cc3000_common.h
 */
/**************************************************************************/
void displayDriverMode(void)
{
#ifdef CC3000_TINY_DRIVER
  Serial.println(F("CC3000 is configure in 'Tiny' mode"));
#else
  Serial.print(F("RX Buffer : "));
  Serial.print(CC3000_RX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
  Serial.print(F("TX Buffer : "));
  Serial.print(CC3000_TX_BUFFER_SIZE);
  Serial.println(F(" bytes"));
#endif
}

/**************************************************************************/
/*!
 @brief  Tries to read the CC3000's internal firmware patch ID
 */
/**************************************************************************/
uint16_t checkFirmwareVersion(void)
{
  uint8_t major, minor;
  uint16_t version;

#ifndef CC3000_TINY_DRIVER  
  if(!cc3000.getFirmwareVersion(&major, &minor))
  {
    Serial.println(F("Unable to retrieve the firmware version!\r\n"));
    version = 0;
  }
  else
  {
    Serial.print(F("Firmware V. : "));
    Serial.print(major); 
    Serial.print(F(".")); 
    Serial.println(minor);
    version = major; 
    version <<= 8; 
    version |= minor;
  }
#endif
  return version;
}

/**************************************************************************/
/*!
 @brief  Tries to read the 6-byte MAC address of the CC3000 module
 */
/**************************************************************************/
void displayMACAddress(void)
{
  uint8_t macAddress[6];

  if(!cc3000.getMacAddress(macAddress))
  {
    Serial.println(F("Unable to retrieve MAC Address!\r\n"));
  }
  else
  {
    Serial.print(F("MAC Address : "));
    cc3000.printHex((byte*)&macAddress, 6);
  }
}


/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

// Minimalist time server query; adapted from Adafruit Gutenbird sketch,
// which in turn has roots in Arduino UdpNTPClient tutorial.
unsigned long getTime(void) {

  uint8_t       buf[48];
  unsigned long ip, startTime, t = 0L;

  Serial.print(F("Locating time server..."));

  // Hostname to IP lookup; use NTP pool (rotates through servers)
  if(cc3000.getHostByName("pool.ntp.org", &ip)) {
    static const char PROGMEM
      timeReqA[] = { 
      227,  0,  6, 236                     }
    ,
    timeReqB[] = {  
      49, 78, 49,  52                     };

    Serial.println(F("\r\nAttempting connection..."));
    startTime = millis();
    do {
      client = cc3000.connectUDP(ip, 123);
    } 
    while((!client.connected()) &&
      ((millis() - startTime) < connectTimeout));

    if(client.connected()) {
      Serial.print(F("connected!\r\nIssuing request..."));

      // Assemble and issue request packet
      memset(buf, 0, sizeof(buf));
      memcpy_P( buf    , timeReqA, sizeof(timeReqA));
      memcpy_P(&buf[12], timeReqB, sizeof(timeReqB));
      client.write(buf, sizeof(buf));

      Serial.print(F("\r\nAwaiting response..."));
      memset(buf, 0, sizeof(buf));
      startTime = millis();
      while((!client.available()) &&
        ((millis() - startTime) < responseTimeout));
      if(client.available()) {
        client.read(buf, sizeof(buf));
        t = (((unsigned long)buf[40] << 24) |
          ((unsigned long)buf[41] << 16) |
          ((unsigned long)buf[42] <<  8) |
          (unsigned long)buf[43]) - 2208988800UL;
        Serial.print(F("OK\r\n"));
      }
      client.close();
    }
  }
  if(!t) Serial.println(F("error"));
  return t;
}
void draw(void) {  
  u8g.drawXBMP( 0, 0, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
  u8g.drawLine(0, 20,128,20);
  u8g.setFont(u8g_font_fixed_v0r);
  u8g.setPrintPos(0, 32); 
  u8g.print("UNIX:");
  u8g.print(currentTime);
  u8g.print("s");
  u8g.drawLine(0, 37,128,37);
  u8g.setPrintPos(0, 49); 
  u8g.print(time_year);
  u8g.print('/');
  u8g.print(time_month);
  u8g.print('/');
  u8g.print(time_day);
  u8g.print("    ");
  u8g.print(time_hour);
  u8g.print(':');
  u8g.print(time_minute);
  u8g.print(':');
  u8g.print(time_second); 

  u8g.setFont(u8g_font_chikitar);
  u8g.drawFrame(0, 54,128,10);
  u8g.setPrintPos(3, 62); 
  u8g.print("Microduino-CC3000-netTime");
}
//=======================开机状态显示函数===========================//
void volcdsetup(char* zi,int x,int y)
{
  u8g.firstPage();  
  do {
    u8g.setFont(u8g_font_7x14Br);
    u8g.setPrintPos(x, y); 
    u8g.print(zi);
  } 
  while( u8g.nextPage() );
}





