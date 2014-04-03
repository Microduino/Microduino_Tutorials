#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>

// nRF24L01(+) radio attached using Getting Started board 
RF24 radio(9,10);

// Network uses that radio
RF24Network network(radio);

// Address of our node
const uint16_t this_node = 0;
// How often to send 'hello world to the other unit
#define interval  200
// When did we last send?
unsigned long last_sent;

// Structure of our payload
struct payload_t
{
  unsigned long nRF_ms;
  boolean nRF_sta;
};


#include <EtherCard.h>

static byte mymac[] = { 
  0x74,0x69,0x69,0x2D,0x30,0x31 };
// ethernet interface ip address
static byte myip[] = { 
  192,168,10,1 };
// gateway ip address
static byte gwip[] = { 
  192,168,10,100 };

byte Ethernet::buffer[920];

#define ledPin  7
boolean ledStatus=false;

char* on = "ON";
char* off = "OFF";
char* statusLabel[4]={
  "OFF","OFF","OFF","OFF"};
int buttonLabel[4]={
  1,1,1,1};

int sta[4];

int i_staA,i_staB;

void setup () {

  Serial.begin(115200);
  Serial.println("WebLed Demo");

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


  Serial.println();

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  SPI.begin();
  radio.begin();
  network.begin(/*channel*/ 90, /*node address*/ this_node);
}

void loop() {

  word len = ether.packetReceive();
  word pos = ether.packetLoop(len);

  if(pos) {
    Serial.print("freeRam: ");
    Serial.println(freeRam ());

    if(strstr((char *)Ethernet::buffer + pos, "GET /?status") != 0)
      sscanf((char *)strstr((char *)Ethernet::buffer + pos, "status"), "status%d=%d", &i_staA,&i_staB);

    if(i_staB)
    {
      statusLabel[i_staA]=on;
      buttonLabel[i_staA]=0;
      sta[i_staA]=1;
    }
    else
    {
      statusLabel[i_staA]=off;
      buttonLabel[i_staA]=1;
      sta[i_staA]=0;
    }

    Serial.print(sta[0]);
    Serial.print(sta[1]);
    Serial.print(sta[2]);
    Serial.println(sta[3]);


    BufferFiller bfill = ether.tcpOffset();
    bfill.emit_p(PSTR("HTTP/1.0 200 OK\r\n"
      "Content-Type: text/html\r\nPragma: no-cache\r\n\r\n"
      "<html><head><title>WebLed</title>"
      "<style type=\"text/css\">.STYLE0 {background-color:#666666; width:160; height:60; border:0;}.STYLE1 {background-color:#0000FF; width:160; height:60; border:0;}.STYLE7 {font-size: xx-large}</style>"
      "</head><body>"
      "<table width=\"AUTO\"border=\"0\"align=\"center\"><tr>"
      "<td><span class=\"STYLE7\">LED1 $S:</span></td>"
      "<td><span class=\"STYLE7\">LED2 $S:</span></td>"
      "<td><span class=\"STYLE7\">LED3 $S:</span></td>"
      "<td><span class=\"STYLE7\">LED4 $S:</span></td>"
      "</tr><tr><td><a href=\"/?status0=$D\"><table class=\"STYLE$D\">"
      "<tr></tr></table></a></td><td><a href=\"/?status1=$D\"><table class=\"STYLE$D\">"
      "<tr></tr></table></a></td><td><a href=\"/?status2=$D\"><table class=\"STYLE$D\">"
      "<tr></tr></table></a></td><td><a href=\"/?status3=$D\"><table class=\"STYLE$D\">"
      "<tr></tr></table></a></td></tr></table>"
      "</body></html>"
      ), statusLabel[0],statusLabel[1],statusLabel[2],statusLabel[3],buttonLabel[0], sta[0],buttonLabel[1], sta[1],buttonLabel[2], sta[2],buttonLabel[3], sta[3]);
    ether.httpServerReply(bfill.position());

    Serial.print("freeRam: ");
    Serial.println(freeRam ());
  }

  network.update();
  // If it's time to send a message, send it!
  if ( millis() - last_sent >= interval  )
  {
    Serial.println("Sending...");
    for(int a=0;a<4;a++)
    {
      payload_t payload = {
        millis(), sta[a]            };
      RF24NetworkHeader header(a+1);    //1,2,3,4
      bool ok = network.write(header,&payload,sizeof(payload));
      Serial.print(a+1);
      if (ok)
        Serial.println(" ..ok.");
      else
        Serial.println(" ..failed.");
    }

    last_sent = millis();
  }
}

unsigned int freeRam () {
  extern unsigned int __heap_start, *__brkval; 
  unsigned int v; 
  return (unsigned int) &v - (__brkval == 0 ? (unsigned int) &__heap_start : (unsigned int) __brkval); 
}

