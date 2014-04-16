#include <EtherCard.h>
#include <dht.h> 
  
#define TEMP_TYPE    1
#define HUMI_TYPE    2
  
// pin 2 is connected to DH21
//Definition DHT
dht DHT;


float temperature=0.0;
float humidity=0.0;
//DH21 dh21(2);

#define DHT11_PIN 2//Sensor data interface to the digital pin2

char temp[10];
  
/*
 * ATmega1280/2560: MOSI --> 51; MISO --> 52; SCK --> 53, SS --> 8
 * Others: MOSI --> 11; MISO --> 12; SCK --> 13, SS --> 8
*/
  
// ethernet interface mac address
static byte mymac[] = {0x04,0x69,0x69,0x2b,0x30,0x31}; 

   
// remote website name
char website[] PROGMEM = "api.yeelink.net";
  
char temp_urlBuf[] PROGMEM = "/v1.0/device/XXXX/sensor/XXXX/datapoints";//replace to your devic ID, sensor ID
char humi_urlBuf[] PROGMEM = "/v1.0/device/XXXX/sensor/XXXX/datapoints";//replace to your devic ID, sensor ID
char apiKey[] PROGMEM = "U-ApiKey: XXXXXXXXXXXXXXXXXXXXXXXXXXXXX";//replace to your APIKEY
  
byte Ethernet::buffer[400];   // a very small tcp/ip buffer is enough here
  
  
// Data transmission interval, yeelink two data upload interval is greater than the 10s
#define REQUEST_RATE 13000    // 13000ms
static long timer;
  
uint8_t flag = TEMP_TYPE;
static char buf[20];
   
// The callback function, print out the server returns a string
static void cb_debug (byte status, word off, word len)
{
  Serial.print("\n--> server response ");
  Serial.print(millis() - timer);
  Serial.println(" ms");
  Serial.println((const char*) Ethernet::buffer + off);
}
   
void setup ()
{
  Serial.begin(9600);
  Serial.println("\nSet gateway ip, interface ip\n");
   
  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0) 
    Serial.println( "\nFailed to access Ethernet controller");
   
  if (!ether.dhcpSetup())
    Serial.println("DHCP failed");

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);  
  ether.printIp("DNS: ", ether.dnsip);  

  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
    
  ether.printIp("SRV: ", ether.hisip);
  
  // testing the gateway internection
  while (ether.clientWaitingGw())
    ether.packetLoop(ether.packetReceive());
  
  Serial.println("Gateway found");
    
  // Initialize the variable timing
  timer = - REQUEST_RATE;
}
  
void loop ()
{  
  ether.packetLoop(ether.packetReceive());
   
  //sending data to the server when interval is greater than the 13s
  if (millis() > timer + REQUEST_RATE) {
  
    // Update timing variables
    timer = millis();
  
    Serial.println("\n--> Sending Request");
      
    // Get sensor values ​​string
    memset(buf, 0, 20);
    get_send_string(buf, flag);
  
    // Temperature and humidity data alternately sent
    switch (flag){
      case TEMP_TYPE:
        ether.httpPost (temp_urlBuf, website, apiKey, buf, cb_debug);
        Serial.println("post temp");
        flag = HUMI_TYPE;
        break;
  
      case HUMI_TYPE:
        ether.httpPost (humi_urlBuf, website, apiKey, buf, cb_debug);
        Serial.println("post humi");
        flag = TEMP_TYPE;
        break;
    }
  }
}
  
char *ftoa(char *a, double f, int precision)
{
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};
    
  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}
   
void get_send_string(char *p, uint8_t type)
{



  int chk = DHT.read11(DHT11_PIN);


  
  
  temperature=(float)DHT.temperature;
  humidity=(float)DHT.humidity;

  Serial.println(temperature);
  Serial.println(humidity);

    
  switch (type) {
    case TEMP_TYPE:
  
     memset(temp, 0, 10);
     sprintf(p,"{\"value\":%s}", ftoa(temp, DHT.temperature, 1));
     break;
  
    case HUMI_TYPE:
     memset(temp, 0, 10);
     sprintf(p,"{\"value\":%s}", ftoa(temp, DHT.humidity, 1));
     break;
  }
  
  Serial.println(p);
}