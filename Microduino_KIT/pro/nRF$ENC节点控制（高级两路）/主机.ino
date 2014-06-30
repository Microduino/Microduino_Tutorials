
//=========================
#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);
// Network uses that radio
RF24Network network(radio);
// Address of our node
const uint16_t this_node = 0;
// Address of the other node 

const uint16_t other_node[2]={
  1,2};
// How often to send 'hello world to the other unit
const unsigned long interval = 150; //ms
// When did we last send?
unsigned long last_sent;
// How many have we sent already
//unsigned long packets_sent;

// Structure of our payload
struct payload_t
{
  uint32_t sensorDataA;
};

struct payload_r
{
  uint32_t ms;
  uint32_t sensorDataB;
};

const int temp_RUN[2] = {
  5,5};
const int temp_STOP[2] ={
  2,2};
boolean power_SW[2];
int temp_VOL[2],temp_SET[2],temp_SET_CACHE[2];
const int temp_VOL_ADD[2] = {
  0,10};
boolean temp_STA_H[2]={
  false,false};
boolean temp_STA_L[2]={
  false,false};
unsigned long led_TIME[2];

//============
#include <EtherCard.h>

static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 
  192,168,169,111 };
// gateway ip address
static byte gwip[] = { 
  192,168,169,1 };

byte Ethernet::buffer[1400];

char* sw[2] = {
  "OFF","ON"};
char* color[2] = {
  "#0099FF","#666666"};
char* statusLabel[3]={
  "OFF","OFF","OFF"};
int buttonLabel[3]={
  1,1,1};

int key[3];

int i_staA,i_staB;

//=============
#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  int EEPROM_TEMP[2];
};

unsigned long eeprom_TIME;


void setup(void)
{
  Serial.begin(115200);
  Serial.println("RF24Network/examples/helloworld_tx/");

  SPI.begin();
  radio.begin();
  radio.setDataRate( RF24_250KBPS ) ;
  network.begin(/*channel*/ 50, /*node address*/ this_node);
  randomSeed(analogRead(0));
  pinMode(7,INPUT_PULLUP); 
  //============
  if(!ether.begin(sizeof Ethernet::buffer, mymac))
    Serial.println( "Failed to access Ethernet controller");
  else
    Serial.println("Ethernet controller initialized");

  if(ether.staticSetup(myip, gwip)){
    Serial.println("Set static IP ok!");
  }
  else{
    Serial.println("Set static IP Failed!");  
  }
  //============
  config_type config_readback;
  EEPROM_read(0, config_readback);
  temp_SET[0] = config_readback.EEPROM_TEMP[0];
  temp_SET[1] = config_readback.EEPROM_TEMP[1];

  Serial.println(temp_SET[1]);  

}

void loop(void)
{
  math(1);

  enc_network();

  if(last_sent>millis()) last_sent=millis();
  if (millis() - last_sent >= interval  )
  {
    nrf_network();

    last_sent = millis();
  }


  if(eeprom_TIME>millis()) eeprom_TIME=millis();
  if(millis()-eeprom_TIME>1000)
  {
    if(temp_SET_CACHE[1]!=temp_SET[1])
      eeprom_WRITE();

    temp_SET_CACHE[1]=temp_SET[1];

    eeprom_TIME=millis();
  }

}

void nrf_network()
{
  Serial.print("Sending...");
  for(int a=0;a<2;a++)
  {
    payload_t payload = { 
      power_SW[a]                                                                                                                                                                                                                                                                                    };
    RF24NetworkHeader header(other_node[a]);
    bool ok = network.write(header,&payload,sizeof(payload));

    if (ok)
    {
      delay(200);
      Serial.print(a);
      Serial.print(" is ok.");

      network.update();
      // If it's time to send a message, send it!
      while ( network.available() )
      {
        // If so, grab it and print it out
        RF24NetworkHeader header;
        payload_r payload;
        network.read(header,&payload,sizeof(payload));
        temp_VOL[a]=payload.sensorDataB;
        led_TIME[a]=payload.ms;
        temp_VOL[a]+=temp_VOL_ADD[a];

        Serial.print(" ,temp_VOL: ");
        Serial.print(temp_VOL[a]);
        Serial.print(", led_TIME:");
        Serial.print(led_TIME[a]/1000);

      }
      Serial.println("");
    }
    else
    {
      Serial.print(a);
      Serial.println(" is failed.");
    }
  }
}

void enc_network()
{
  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if(pos) {
    Serial.print("freeRam: ");
    Serial.println(freeRam ());

    if(strstr((char *)Ethernet::buffer + pos, "GET /?status") != 0)
    {
      i_staA=0,i_staB=0;
      key[1]=0,key[2]=0;

      sscanf((char *)strstr((char *)Ethernet::buffer + pos, "status"), "status%d=%d", &i_staA,&i_staB);


      statusLabel[i_staA]=sw[i_staB];
      buttonLabel[i_staA]=!i_staB;
      key[i_staA]=i_staB;

      Serial.print(key[0]);
      Serial.print(key[1]);
      Serial.print(key[2]);

      if(i_staA==1 && temp_SET[1]<99)
        temp_SET[1]+=5;

      if(i_staA==2 && temp_SET[1]>10)
        temp_SET[1]-=5;
    }

    power_SW[0]=key[0];

    if(power_SW[0])
      color[0]="#0099FF";
    else
      color[0]="#999999";

    if(power_SW[1])
      color[1]="#0099FF";
    else
      color[1]="#999999";


    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\"><title>WebLed</title>"
      "<style type=\"text/css\">"
      ".STYLE2 {background-color:#0099FF; width:60; height:49; border:0;}"
      ".STYLE7 {font-size: xx-large;font-family: Arial; color:#FFFFFF;}"
      ".STYLE6 {font-size: X-large;font-family: Arial; color:#FFFFFF;}"
      "</style>"
      "</head><body style=\"zoom:300%\">"
      "<table width=\"AUTO\" border=\"0\" align=\"center\">"
      "<td><a href=\"/?status0=$D\"><table bgcolor=\"$S\" width=\"164\" height=\"102\" border=\"0\">"
      "<td><div align=\"center\"><span class=\"STYLE7\">LED1 $S</span></div></td>"
      "</table></a></td>"
      "<td><table bgcolor=\"$S\" width=\"64\" height=\"102\" border=\"0\">"
      "<tr><td><div align=\"center\"><span class=\"STYLE6\">$D`C</span></div></td></tr>"
      "<tr><td><div align=\"center\"><span class=\"STYLE6\">$D`C</span></div></td></tr>"
      "</table></td>"
      "<td><table width=\"AUTO\" height=\"100\" border=\"0\">"
      "<tr><td><a href=\"/?status1=$D\"><table class=\"STYLE2\">"
      "<td><div align=\"center\"><span class=\"STYLE6\">∧</span></div></td>"
      "</table></a></td></tr>"
      "<tr><td><a href=\"/?status2=$D\"><table class=\"STYLE2\">"
      "<td><div align=\"center\"><span class=\"STYLE6\">∨</span></div></td>"
      "</table></a></td></tr>"
      "</table></td>"
      "</table>"
      "</body></html>"
      ),  buttonLabel[0],color[0],statusLabel[0],color[1],temp_SET[1], temp_VOL[1],buttonLabel[1],buttonLabel[2]);
    ether.httpServerReply(bfill.position());

    Serial.print("freeRam: ");
    Serial.println(freeRam ());
  }
}


void math(int math_NUM)
{
  if(temp_SET[math_NUM] - temp_VOL[math_NUM] > temp_RUN[math_NUM]) //Tem is low
  {
    power_SW[math_NUM] = true;
    temp_STA_L[math_NUM] = true;
  }
  if(temp_STA_L[math_NUM] == true && temp_SET[math_NUM] - temp_VOL[math_NUM] < temp_STOP[math_NUM])
  {
    power_SW[math_NUM] = false;
    temp_STA_L[math_NUM] = false;
  }

  if(temp_SET[math_NUM] - temp_VOL[math_NUM] < -temp_RUN[math_NUM]) //Tem is low
  {
    power_SW[math_NUM] = false;
    temp_STA_H[math_NUM] = true;
  }
  if(temp_STA_H[math_NUM] == true && temp_SET[math_NUM] - temp_VOL[math_NUM] > -temp_STOP[math_NUM])
  {
    power_SW[math_NUM] = true;
    temp_STA_H[math_NUM] = false;  
  }
}

void eeprom_WRITE()
{
  config_type config;  		// 定义结构变量config，并定义config的内容
  config.EEPROM_TEMP[0] = temp_SET[0];
  config.EEPROM_TEMP[1] = temp_SET[1];
  EEPROM_write(0, config); 	// 变量config存储到EEPROM，地址0写入
}

unsigned int freeRam () {
  extern unsigned int __heap_start, *__brkval; 
  unsigned int v; 
  return (unsigned int) &v - (__brkval == 0 ? (unsigned int) &__heap_start : (unsigned int) __brkval); 
}

