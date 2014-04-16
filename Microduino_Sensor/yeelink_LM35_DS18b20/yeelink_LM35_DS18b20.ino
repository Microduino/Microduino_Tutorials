#include <OneWire.h>
#include <EtherCard.h>
#include <DallasTemperature.h>
#define OUT
// Data wire is plugged into port A0 on the Arduino
#define ONE_WIRE_BUS A0
// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&oneWire);
 
#define REQUEST_RATE 60000 // milliseconds
// ethernet interface mac address
static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 
  10,21,0,193 };
// gateway ip address
static byte gwip[] = { 
  10,21,0,1 };
// remote dns ip address
static byte dnsip[] = {
  10,11,5,25 };
 
// remote website name
char website[] PROGMEM = "api.yeelink.net";
char urlBuf[] PROGMEM = "/v1.0/device/XXXXX/sensor/XXXXX/datapoints";//replace to your devic ID, sensor ID
char apiKey[] PROGMEM = "U-ApiKey: XXXXXXXXXXXXXXXXXXXXXXXXX";//replace to your APIKEY
byte Ethernet::buffer[400];   // a very small tcp/ip buffer is enough here
static long timer;
 
// called when the client request is complete
static void my_result_cb (byte status, word off, word len) {
  Serial.print("<<< reply ");
  Serial.print(millis() - timer);
  Serial.println(" ms");
  Serial.println((const char*) Ethernet::buffer + off);
}
 
void setup () {
  sensors.begin();
  Serial.begin(57600);
  Serial.println("\n[getStaticIP]");
 
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "Failed to access Ethernet controller");
 
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    
  ether.printIp("SRV: ", ether.hisip);

 
  timer = - REQUEST_RATE; // start timing out right away
}
 
void loop () {
  ether.packetLoop(ether.packetReceive());
 
  if (millis() > timer + REQUEST_RATE) {
    timer = millis();
    Serial.println("\n>>> REQ");
    static char buf[20];
    get_send_string(buf);
    if (!ether.dnsLookup(website))
      Serial.println("DNS failed");
    ether.printIp("Server: ", ether.hisip);
    ether.httpPost (urlBuf, website, apiKey, buf, my_result_cb);
  }
}
 
void get_send_string(OUT char *p){
  sensors.requestTemperatures();  
  uint16_t Tc_100 = sensors.getTempCByIndex(0)*10;
  uint8_t whole, fract;
  whole = Tc_100/10 ;  // separate off the whole and fractional portions
  fract = Tc_100 % 10;
  sprintf(p,"{\"value\":%d.%d}",whole,fract); 
}
