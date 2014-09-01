////////////////////
//  all  the  xxxxx  need  you  get  the  ID  from yeelink
////////////////////
#include <EtherCard.h>
////传感器个数及对应传感器ID
#define N 1
const uint16_t PostingInterval = 3000;
// 发送到服务器时间间隔,N大了可以适当缩短,
//每个传感器发送时间间隔为PostingInterval*N
const uint16_t SensorID[N] = {/*xxxxx*/}; // replace your sensor ID
char sensorData[N][30];
uint8_t dataLength[N];
int i;
float myVoltage;// the  analog voltage  map 0-5V
float windSpeed;// the  windSpeed  range  0-30m/s
static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };
static byte myip[]={
  192,168,1,254};
static byte mygate[]={
  192,168,1,1};
static byte mydns[]={
  192,168,1,1};
// remote website name
char website[] PROGMEM = "api.yeelink.net";
// for yeelink api
char APIKey[] PROGMEM = "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"; // replace your yeelink api key here
char DeviceID[] PROGMEM = "xxxx"; // replace your device ID
uint8_t Ethernet::buffer[700];
uint32_t lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
Stash stash; 

void setup() 
{
  Serial.begin(9600);
  Serial.println("\n[yeelink client]");
  if (ether.begin(sizeof Ethernet::buffer, mymac,10) == 0) 
    Serial.println( "Failed to access Ethernet controller");
  if (!ether.staticSetup(myip,mygate,mydns))
    Serial.println("DHCP failed");
  ether.printIp("My IP: ", ether.myip);
  ether.printIp("GW IP: ", ether.gwip);
  ether.printIp("DNS IP: ", ether.dnsip);
  if (!ether.dnsLookup(website))
    Serial.println("DNS failed");
  ether.printIp("Server: ", ether.hisip); 
}

void loop() 
{
  ether.packetLoop(ether.packetReceive());
  if( /*millis() < lastConnectionTime ||*/ millis() - lastConnectionTime > PostingInterval)
  {
    get_Send_String(i);
    send_Data(i);
    i++;
    if (i == N) i=0;  
  }
}

void send_Data(uint8_t j) {
  // Create a Post for yeelink server,
  // and send request saving sessionID
  Stash::prepare(PSTR("POST /v1.0/device/$F/sensor/$D/datapoints HTTP/1.1" "\r\n" 
    "Host: api.yeelink.net" "\r\n" 
    "U-ApiKey: $F" "\r\n" 
    "Content-Length: $D" "\r\n"
    "Content-Type: application/x-www-form-urlencoded" "\r\n" "\r\n"                       
    "$S" "\r\n"),
  DeviceID,SensorID[i],APIKey,dataLength[i],sensorData[i]);
  ether.tcpSend();
  lastConnectionTime = millis();  
}
////get the send string
void get_Send_String(uint8_t k)
{
  switch(k)
  {
  case 0:
    {
      myWindSpeed();
      uint8_t whole, fract;
      whole = (int)(windSpeed)  ;  // separate off the whole and fractional portions
      fract =(int)(windSpeed*10);
      fract=fract%10;
      dataLength[k] = sprintf(sensorData[k],"{\"value\":%d.%d}",whole,fract);
      break;
    }
    //here  you  can  add  case 1:  if you  have  the other  sensor  data

  }
}
/////////////wind speed  function  
void  myWindSpeed()
{
   myVoltage=analogRead(A0)/1023.0*5.0;
  delay(10);
   windSpeed=myVoltage*30.0/5.0;//max detect windspeed 30m/s
  windSpeed=windSpeed*1.0;//change this value (1.0) to calibrate
  if(windSpeed<0.15) //debounce  
  {
    windSpeed=0;
    Serial.print("Wind  speed   ");
    Serial.println("0m/s");
    delay(1000);
  }
  else
  {
    Serial.print("Wind  speed   ");
    Serial.print(windSpeed,1 );// output ("0.0") fraction
    Serial.println("m/s");
    delay(1000);
  }
}



