#include <EtherCard.h>

// ethernet interface mac address
static byte mymac[] = {
  0x04,0x69,0x69,0x2b,0x30,0x31}; 

// remote website name
char website[] PROGMEM = "www.lucadentella.it";

byte Ethernet::buffer[700];   // a very small tcp/ip buffer is enough here

static long timer;

static void response_callback (byte status, word off, word len) {
  Serial.println((const char*) Ethernet::buffer + off + 207);
  //Serial.print((const char*) Ethernet::buffer + off);
}

void setup ()
{
  Serial.begin(9600);
  Serial.println("Client Demo");
  Serial.println();

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "\nFailed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("Netmask:  ", ether.netmask);
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");

  ether.printIp("SRV: ", ether.hisip);

  // testing the gateway internection
  while (ether.clientWaitingGw())
    ether.packetLoop(ether.packetReceive());

  Serial.println("Gateway found");

}

void loop ()
{  
  ether.packetLoop(ether.packetReceive());
  
  if (millis() > timer) {
    timer = millis() + 5000;
    ether.browseUrl(PSTR("/demo/"), "aphorisms.php", website, response_callback);
  }
}
