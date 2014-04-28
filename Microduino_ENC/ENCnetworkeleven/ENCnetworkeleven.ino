#include <EtherCard.h>


// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };

static byte ntpServer[] = {193,204,114,232};
static byte srcPort = 0;


byte Ethernet::buffer[700];
BufferFiller bfill;


void setup () {

  Serial.begin(9600);
  Serial.println("wr703N Demo");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
    Serial.println( "Failed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("Failed to get configuration from DHCP");
  else
    Serial.println("DHCP configuration done");
 
  ether.printIp("IP Address:\t", ether.myip);
  ether.printIp("Netmask:\t", ether.netmask);
  ether.printIp("Gateway:\t", ether.gwip);

  Serial.println();

}


void loop() {
 
  ether.packetLoop(ether.packetReceive());
  }
