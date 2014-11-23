/* 
*  Simple robot control with Arduino & the CC3000 WiFi chip
*/

// Include required libraries
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"
#include <stdlib.h>

String result;
int ledCommand[4];

// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   2
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10

// LED pins
int led1 = 4;
int led2 = 5;
 
const unsigned long
  dhcpTimeout     = 60L * 1000L, // Max time to wait for address from DHCP
  connectTimeout  = 15L * 1000L, // Max time to wait for server connection
  responseTimeout = 15L * 1000L; // Max time to wait for data from server
uint32_t t;

int resultLength;

// WiFi network (change with your settings !)
#define WLAN_SSID       "Your route"        // cannot be longer than 32 characters!
#define WLAN_PASS       "Password"
#define WLAN_SECURITY   WLAN_SEC_WPA2 // This can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2

// What TCP port to listen on for connections.
#define LISTEN_PORT           8888    

// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2);                                
                                         
// Create server
Adafruit_CC3000_Server robotServer(LISTEN_PORT);

void setup() {
   
  Serial.begin(115200);
  
  result = "";
  
  for(int i=4;i<=7;i++)
  {  
    pinMode(i, OUTPUT);  //set pin 4,5,6,7 to output mode
  }
  
  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }
 
  // Start listening for connections
  robotServer.begin();
  
  Serial.println(F("Listening for connections..."));
 
}

void loop() {
  
  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = robotServer.available();
  if (client) {
     //boolean currentLineIsBlank = true;
     // Check if there is data available to read.
     while (client.available()) {
     
       char c = client.read();
       result = result + c;
     }

     //Serial.println(result);

     // give the web browser time to receive the data
    delay(5);

    send_led_command(result);

    // close the connection:
    client.close();
    Serial.println("client disconnected");
    
    // Reset result variable
    result = "";
  }
           
}

void send_led_command(String result)
{

  result.trim();


  format_result(ledCommand,result);

  for(int i=0; i<4; i++) {
    Serial.println(ledCommand[i]);
  }


if(ledCommand[0]==0&&ledCommand[1]==0) {
  digitalWrite(4, LOW);    
  digitalWrite(5, LOW);
} else {
  if(ledCommand[2]==1&&ledCommand[3]==1) {
    digitalWrite(4, HIGH); 
    digitalWrite(5, HIGH);
} else {
  if(ledCommand[2]==0&&ledCommand[3]==1) {
    digitalWrite(4, LOW); 
    digitalWrite(5, HIGH);
  } else {
    digitalWrite(4, HIGH);
    digitalWrite(5, LOW);
  }
}
}





}



void format_result(int* array, String result) {
 
 result.trim();
 resultLength = result.length();
 //Serial.println(result);
 
 int commaPosition;
 int i = 0;
 do
  {
      commaPosition = result.indexOf(',');
      if(commaPosition != -1)
      {
          //Serial.println( result.substring(0,commaPosition));
          array[i] = result.substring(0,commaPosition).toInt();
          i = i+1;
          result = result.substring(commaPosition+1, result.length());
      }
      else
      {
         if(result.length() > 0) {
           //Serial.println(result);
          }
      }
      
   }
   while(commaPosition >=0);  
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
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}
