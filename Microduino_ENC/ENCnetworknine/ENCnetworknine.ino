#include <EtherCard.h>


#define LED_PIN 2

char* on = "ON";
char* off = "OFF";


boolean ledStatus;
char* ledLabel;
char* linkLabel;


// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[] = { 
  192,168,1,203 };

byte Ethernet::buffer[700];
BufferFiller bfill;


void setup () {

  Serial.begin(9600);
  Serial.println("BigerImage Demo");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.staticSetup(myip))
    Serial.println("Failed to set IP address");

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  ledStatus = false;
  ledLabel = off;
  linkLabel = on;

}


void loop () {

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);
  
  if(pos) {
        
    if(strstr((char *)Ethernet::buffer + pos, "GET /?ON") != 0) {
      ledStatus = true;
      ledLabel = on;
      linkLabel = off;
    } else if(strstr((char *)Ethernet::buffer + pos, "GET /?OFF") != 0) {
      ledStatus = false;
      ledLabel = off;
      linkLabel = on;
    }
    digitalWrite(LED_PIN, ledStatus);

    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html><head><meta name='viewport' content='width=200px'/></head><body>"
      "<div style='position:absolute;width:200px;height:200px;top:50%;left:50%;margin:-100px 0 0 -100px'>"
      "<div style='font:bold 14px verdana;text-align:center'>Relay is $S</div>"
      "<br><div style='text-align:center'>"
      "<a href='/?$S'><img src='http://www.lucadentella.it/files/bt_$S.png'></a>"
      "</div></div></body></html>"
      ), ledLabel, linkLabel, linkLabel);

      ether.httpServerReply(bfill.position());
    }
}
