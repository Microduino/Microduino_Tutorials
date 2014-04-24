#include <EtherCard.h>
static byte mymac[] = {
  0x74,0x69,0x69,0x2D,0x30,0x31};
byte Ethernet::buffer[700];

void setup () {

  Serial.begin(9600);
  Serial.println("DHCP Demo");

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "\nFailed to access Ethernet controller");

  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP Address:\t", ether.myip);
  ether.printIp("Netmask:\t", ether.netmask);
  ether.printIp("Gateway:\t", ether.gwip);

}

void loop() {
  ether.packetLoop(ether.packetReceive());
}

