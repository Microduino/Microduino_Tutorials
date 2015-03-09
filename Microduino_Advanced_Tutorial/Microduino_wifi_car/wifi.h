#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   2  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIV2); // you can change this clock speed but DI

#define WLAN_SECURITY   WLAN_SEC_WPA2

/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    //Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {

    char ipSegment[5];

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 24));
    IPAdressShow=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 16));
    IPAdressShow+=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 8));
    IPAdressShow+=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress));
    IPAdressShow+=ipSegment;

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

void setup_wifi()
{
  Serial.println(F("Hello, CC3000!\n")); 

  Serial.print("Free RAM: "); 
  Serial.println(getFreeRam(), DEC);

  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  else
    osd_setup(1,"begin...");

  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  else
    osd_setup(2,"Initializing...");

  Serial.println(F("Connected!"));

  Serial.println(F("Request DHCP"));
  delay(1000);
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
  /* Display the IP address DNS, Gateway, etc. */
  while (! displayConnectionDetails()) {
    delay(1000);
  }
  osd_setup(4,"Init is ok");
  delay(1000);
  //start control your car
  Serial.println("Start Control Your Car");

  // Start listening for connections
  carServer.begin();

  Serial.println(F("Listening for connections..."));

}

void wifi_data()
{
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = carServer.available();
  if (client) {
    boolean currentLineIsBlank = true;
    // Check if there is data available to read.
    while (client.available()) {

      char c = client.read();
      result = result + c;
      //Serial.write(c);

      // Delete HTTP headers
      if(result.endsWith("end"))
      {
        currentInfo=result;
        result="";
      }

      if (c == '\n' && currentLineIsBlank) {
        // send a standard http response header
        // client.println("HTTP/1.1 200 OK");
        // client.println("Content-Type: text/html");
        // client.println("Connection: close");
        // client.println();
      }
      if (c == '\n') {
        // you're starting a new line
        currentLineIsBlank = true;
      } 
      else if (c != '\r') {
        // you've gotten a character on the current line
        currentLineIsBlank = false;
      }
    }
    //client.close();
    //Serial.println("client disconnected");
    // Reset result variable
    result = "";
  }

  //print currentInfo
  if(currentInfo!=NULL) {

    for (int i = 0; i < currentInfo.length(); i++) {
      if (currentInfo.substring(i, i+1) == ":") {
        angle = currentInfo.substring(0, i).toInt();
        speed = currentInfo.substring(i+1).toInt();
        break;
      }
    }
    /*
    Serial.print("Angle:");
     Serial.print(angle);
     Serial.print("  Speed:");
     Serial.println(speed);
     */
  }
}

