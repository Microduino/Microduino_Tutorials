#include <U8glib.h> 
#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_fur25)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar) 

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192,168,199,177);

unsigned int localPort = 8888;      // local port to listen for UDP packets

//IPAddress timeServer(132, 163, 4, 101); // time-a.timefreq.bldrdoc.gov NTP server
 IPAddress timeServer(132, 163, 4, 102); // time-b.timefreq.bldrdoc.gov NTP server
// IPAddress timeServer(132, 163, 4, 103); // time-c.timefreq.bldrdoc.gov NTP server

const int NTP_PACKET_SIZE= 48; // NTP time stamp is in the first 48 bytes of the message

int HH, MM, Ss;
String currentTime;

byte packetBuffer[ NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets 

// A UDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() 
{
 // Open serial communications and wait for port to open:
  Serial.begin(115200);
  // while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}





  SPI.begin();
  // start Ethernet and UDP
  // if (Ethernet.begin(mac) == 0) {
  //   Serial.println("Failed to configure Ethernet using DHCP");
  //   // no point in carrying on, so do nothing forevermore:
  //   for(;;)
  //     ;
  // }



  Ethernet.begin(mac, ip);
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());


  Udp.begin(localPort);
}

void loop()
{
  sendNTPpacket(timeServer); // send an NTP packet to a time server

    // wait to see if a reply is available
  delay(1000);  
  if ( Udp.parsePacket() ) {  
    // We've received a packet, read the data from it
    Udp.read(packetBuffer,NTP_PACKET_SIZE);  // read the packet into the buffer

    //the timestamp starts at byte 40 of the received packet and is four bytes,
    // or two words, long. First, esxtract the two words:

    unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
    unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);  
    // combine the four bytes (two words) into a long integer
    // this is NTP time (seconds since Jan 1 1900):
    unsigned long secsSince1900 = highWord << 16 | lowWord;  
    Serial.print("Seconds since Jan 1 1900 = " );
    Serial.println(secsSince1900);               

    // now convert NTP time into everyday time:
    Serial.print("Unix time = ");
    // Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
    const unsigned long seventyYears = 2208988800UL;     
    // subtract seventy years:
    unsigned long epoch = secsSince1900 - seventyYears;  
    // print Unix time:
    Serial.println(epoch);                               


    // print the hour, minute and second:
    Serial.print("The UTC time is ");       // UTC is the time at Greenwich Meridian (GMT)
    Serial.print((epoch  % 86400L) / 3600); // print the hour (86400 equals secs per day)
    
    HH=((epoch  % 86400L) / 3600)+8;


    Serial.print(':');  
    if ( ((epoch % 3600) / 60) < 10 ) {
      // In the first 10 minutes of each hour, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.print((epoch  % 3600) / 60); // print the minute (3600 equals secs per minute)
    
    MM=(epoch  % 3600) / 60;

    Serial.print(':'); 
    if ( (epoch % 60) < 10 ) {
      // In the first 10 seconds of each minute, we'll want a leading '0'
      Serial.print('0');
    }
    Serial.println(epoch %60); // print the second
    Ss=epoch %60;
  }
  // wait ten seconds before asking for the time again
  delay(10000); 


 u8g.firstPage();
  do {
  draw();
  } while( u8g.nextPage() );

}

// send an NTP request to the time server at the given address 
unsigned long sendNTPpacket(IPAddress& address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE); 
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12]  = 49; 
  packetBuffer[13]  = 0x4E;
  packetBuffer[14]  = 49;
  packetBuffer[15]  = 52;

  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp: 		   
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer,NTP_PACKET_SIZE);
  Udp.endPacket(); 
}



void draw(void) {

char temp[25]; 
itoa(HH,temp,10); 
currentTime=temp;
currentTime+=":";
itoa(MM,temp,10); 
currentTime+=temp;
currentTime+=":";
itoa(Ss,temp,10); 
currentTime+=temp;


setFont_L; //设置字体为大
u8g.setPrintPos(5, 50); //设置文字开始坐标
u8g.print(currentTime); 

}






