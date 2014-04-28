// This is a demo of the RBBB running as webserver with the Ether Card
// 2010-05-28 <jc@wippler.nl> http://opensource.org/licenses/mit-license.php

#include <EtherCard.h>

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 192,168,1,203 };

byte Ethernet::buffer[500];
BufferFiller bfill;



const int ledPin = 2;
boolean ledStatus;

char* on = "ON";
char* off = "OFF";
char* statusLabel;
char* buttonLabel;





void setup () {


  Serial.begin(9600);
  Serial.println("WebLed Demo");



  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");


  if (!ether.staticSetup(myip))
    Serial.println("Failed to set IP address");


  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  ledStatus = false;

}



void loop () {

 
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if(pos) {
    
    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=ON") != 0) {
      Serial.println("Received ON command");
      ledStatus = true;
    }

    if(strstr((char *)Ethernet::buffer + pos, "GET /?status=OFF") != 0) {
      Serial.println("Received OFF command");
      ledStatus = false;
    }
    
    if(ledStatus) {
      digitalWrite(ledPin, HIGH);
      statusLabel = on;
      buttonLabel = off;
    } else {
      digitalWrite(ledPin, LOW);
      statusLabel = off;
      buttonLabel = on;
    }
      
    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html><head><title>WebLed</title></head>"
      "<body>LED Status: $S "
      "<a href=\"/?status=$S\"><input type=\"button\" value=\"$S\"></a>"
      "</body></html>"
      ), statusLabel, buttonLabel, buttonLabel);
    ether.httpServerReply(bfill.position());
  }






}
