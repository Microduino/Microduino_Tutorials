#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);        // HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)
/*
font:  
 u8g.setFont(u8g_font_7x13)
 u8g.setFont(u8g_font_fixed_v0r);
 u8g.setFont(u8g_font_chikitar);
 u8g.setFont(u8g_font_osb21);
 */

//================================

#include <iterator>
#include <string>
#include <pnew.cpp>

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include "utility/debug.h"

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   2  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
SPI_CLOCK_DIV2); // you can change this clock speed


// What page to grab!
#define WEBSITE      "wxdata.weather.com"
#define WEBPAGE      "/wxdata/weather/local/chxx0008?cc=*&unit=m"

uint32_t ip;

Adafruit_CC3000_Client www;

/**************************************************************************/
/*!
 @brief  Sets up the HW and the CC3000 module (called automatically
 on startup)
 */
/**************************************************************************/

#define startbuf 228
#define NUM 41

String s_data[NUM];


boolean b_cache;
boolean NET_WEBSITE_sta=false;

int i_data_num=0,i_cache0_num=0,i_cache1_num=0;

//===============================================



int updata[5];

int tem,hum;


int wind_level,wind_angle;

//=============================
#define INTERVAL_LCD             100
#define INTERVAL_LCD_SWITCH      2000
#define INTERVAL_UPDATA          3600000

//*60*2

unsigned long updatatime=millis();
unsigned long lcdtime=millis();
unsigned long lcd_switch_time=millis();

int lcd_switch_num=0;

//=============================

void voDATA_begin()
{
  i_data_num=0;

  for(int a=0;a<NUM;a++)    s_data[a]="";

  s_data[17]+="<lsup>";
}


bool voCC3000_updata()
{
  //----------------------------------------------------------
  cc3000.reboot();
  //----------------------------------------------------------
  delay(2000);

  if(!voCC3000_get_dhcp())
  {
    Serial.println("\n 2/4 dhcp ERROR");
  }
  else
  {
    Serial.println("\n 2/4 dhcp OK");
    {
      if(!voCC3000_get_server_ip())
      {
        Serial.println("\n 3/4 server ERROR");
      }
      else
      {
        Serial.println("\n 3/4 server OK");
        {
          if(voCC3000_ping())
          {
            NET_WEBSITE_sta = true;
            Serial.println("\n 4/4 pos/rec OK");

            voDATA_begin();
            voCC3000_rec();
            voDATA_get();
          }
          else
          {
            NET_WEBSITE_sta = false;
            Serial.println("\n 4/4 pos/rec ERROR");
          }
        }
      }
    }
  }

  cc3000.stop();

  //----------------------------------------------------------
  return NET_WEBSITE_sta;
}

bool voCC3000_reconnect()
{
  cc3000.reboot();
  //----------------------------------------------------------
  Serial.println(F("==========SmartConfig Start"));
  if (!cc3000.begin(false))
  {
    Serial.println(F("Unable to initialise the CC3000! Check your wiring?"));
    while(1);
  }

  /* Try to use the smart config app (no AES encryption), saving */
  /* the connection details if we succeed */
  Serial.println(F("Waiting for a SmartConfig connection (~60s) ..."));
  volcdsetup("Wait SmartConfig 60s ",0,15);

  if (!cc3000.startSmartConfig(false))
  {
    Serial.println(F("==========SmartConfig ERROR"));
    return false;
  }

  Serial.println(F("Saved connection details and connected to AP!"));

  Serial.println(F("==========SmartConfig OK"));
  return true;
}

bool voCC3000_get_dhcp()
{
  bool cache;
  //  Serial.println(F("Request DHCP"));

  for(int a=0;a<2;a++)
  {
    if(!cc3000.checkDHCP())
    {
      cache=false;
      delay(100); // ToDo: Insert a DHCP timeout!
    }
    else
    {
      cache=true;
      break;
    }
  }

  if(!cache)
    return false;
  else
  {
    for(int a=0;a<2;a++)
    {
      /* Display the IP address DNS, Gateway, etc. */
      if (! displayConnectionDetails())
      {
        cache=false;
        delay(500);
      }
      else
      {
        cache=true;
        break;
      }
    }
    return cache ? true :false;
  }
}

bool voCC3000_get_server_ip()  
{
  ip = 0;
  // Try looking up the website's IP address
  Serial.print(WEBSITE); 
  Serial.print(F(" -> "));
  for(int a=0;a<4;a++)
  {
    if (! ip) 
    {
      Serial.println(F("\r\n While ing "));
      if (! cc3000.getHostByName(WEBSITE, &ip)) 
        Serial.println(F("Couldn't resolve!"));
      delay(500);
    }
    else
      break;
  }

  cc3000.printIPdotsRev(ip);
  Serial.println(F("  "));

  return ip ? true :false;

}

bool voCC3000_ping()
{
  // Optional: Do a ping test on the website
  /*
  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
   replies = cc3000.ping(ip, 5);
   Serial.print(replies); Serial.println(F(" replies"));
   */

  /* Try connecting to the website */

  www = cc3000.connectTCP(ip, 80);
  if (www.connected()) {
    www.print(F("GET "));
    www.print(WEBPAGE);
    www.print(F(" HTTP/1.0\r\n"));
    www.print(F("Host: ")); 
    www.println(WEBSITE);
    www.println(F("Connection: close"));
    www.println();
    return true;
  }
  else {
    Serial.println(F("Connection failed"));    
    return false;
  }
}


void voCC3000_rec()
{
  Serial.println(F("connect start--------------------------------"));

  while (www.connected()) 
  {
    while (www.available()) 
    {
      char c=www.read();

      Serial.print(c);

      if(c!=' ')
      {
        i_data_num++;

        if( i_data_num>startbuf)
        {
          if(c=='<')
          {
            i_cache0_num++;
            b_cache=false;
          }

          if(c=='>')
          {
            i_cache1_num++;
            b_cache=true;
          }

          if(b_cache && c!='>') 
            s_data[i_cache1_num/2]+=c;

        }
      }

    }
  }
  www.close();

  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  cc3000.disconnect();
  Serial.println(F("\n\nDisconnecting"));

  //  Serial.println(s_data);
  Serial.println(F("connect end--------------------------------"));
}


void voDATA_get()
{
  /*
  for(int a=0;a<NUM;a++)
   {
   Serial.print("s_data[");
   Serial.print(a);
   Serial.print("] is:");
   Serial.println(s_data[a]);
   }
   */
  tem=s_data[19].toInt();
  hum=s_data[31].toInt();

  wind_level=s_data[27].toInt();
  wind_angle=s_data[29].toInt();

  char c_char[5][20];

  s_data[17].toCharArray(c_char[1], i_data_num);

  sscanf((char *)strstr((char *)c_char[1], "<lsup>"), "<lsup>%d/%d/%2d%d:%d", &updata[3],&updata[2],&updata[4],&updata[1],&updata[0]);
  /*
  Serial.print("updata: ");
   Serial.print(updata[4]);
   Serial.print("/");
   Serial.print(updata[3]);
   Serial.print("/");
   Serial.print(updata[2]);
   Serial.print(" ");
   Serial.print(updata[1]);
   Serial.print(":");
   Serial.println(updata[0]);
   */
}


void volcdsetup(char* zi,unsigned int x,unsigned int y)
{
  u8g.firstPage();  
  do {
    setFont_M;    
    u8g.setPrintPos(x, y); 
    u8g.print(zi);
  } 
  while( u8g.nextPage() );
}


void setup(void)
{

  //  u8g.setRot180();

  //-------------------------------------------------------

  Serial.begin(115200);
  Serial.println(F("Hello, CC3000!\n")); 

  displayFreeRam();

  //----------------------------------------------------------
  Serial.println(F("\nInitializing..."));
  volcdsetup("1/4 begin READY",10,15);
  while (!cc3000.begin(false,true))
  {
    Serial.println("\n 1/4 begin ERROR");
    volcdsetup("1/4 begin ERROR",10,15);
    delay(1000);
    //=========================
    {
      Serial.println("\n ---reconnect---");
      volcdsetup("---reconnect---",10,15);

      if(voCC3000_reconnect())
      {
        volcdsetup("--recon. OK--",10,15);
        Serial.println("\n 4--recon. OK--");
      }
      else
      {
        Serial.println("\n -recon. ERROR-");
        volcdsetup("-recon. ERROR-",10,15);
        delay(1000);
      }
    }
    //=========================
  }
  Serial.println(F("Connected!"));

  volcdsetup("1/4 begin OK",10,15);
  Serial.println("\n 1/4 begin OK");
  {
    volcdsetup("2/4 dhcp READY",10,15);
    if(!voCC3000_get_dhcp())
    {
      Serial.println("\n 2/4 dhcp ERROR");
      volcdsetup("2/4 dhcp ERROR",10,15);
      delay(1000);

      //=========================
      //=========================
    }
    else
    {
      Serial.println("\n 2/4 dhcp OK");
      volcdsetup("2/4 dhcp OK",10,15);
      delay(1000);

      {
        //----------------------------------------------------------
        volcdsetup("3/4 server READY",10,15);
        if(!voCC3000_get_server_ip())
        {
          Serial.println("\n 3/4 server ERROR");
          volcdsetup("3/4 server ERROR",10,15);
          delay(1000);

          //=========================

          //=========================
        }
        else
        {
          Serial.println("\n 3/4 server OK");
          volcdsetup("3/4 server OK",10,15);
          delay(1000);
          {
            //----------------------------------------------------------
            volcdsetup("4/4 pos/rec READY",10,15);

            if(voCC3000_ping())
            {
              NET_WEBSITE_sta = true;
              Serial.println("\n 4/4 pos/rec OK");
              volcdsetup("4/4 pos/rec OK",10,15);
              delay(1000);

              voDATA_begin();
              voCC3000_rec();
              voDATA_get();
            }
            else
            {
              NET_WEBSITE_sta = false;
              Serial.println("\n 4/4 pos/rec ERROR");
              volcdsetup("4/4 pos/rec ERROR",10,15);
              delay(1000);
            }
          }
        }
      }
    }
  }

  cc3000.stop();

  //----------------------------------------------------------
  // Optional SSID scan
  // listSSIDResults();

  displayFreeRam();

}


void loop(void)
{
  if (lcdtime > millis()) lcdtime = millis();
  if(millis()-lcdtime>INTERVAL_LCD)
  {
    volcd();
    lcdtime=millis();
  }

  if (lcd_switch_time > millis()) lcd_switch_time = millis();
  if(millis()-lcd_switch_time>INTERVAL_LCD_SWITCH)
  {
    lcd_switch_num++;
    if(lcd_switch_num>3)
      lcd_switch_num=0;

    lcd_switch_time=millis();
  }


  if (updatatime > millis()) updatatime = millis();
  if(millis()-updatatime>INTERVAL_UPDATA)
  {
    volcdsetup("-----UPDATA-----",10,15);

    if(voCC3000_updata())
      volcdsetup("---UPDATA OK---",10,15);
    else
      volcdsetup("--UPDATA REEOR--",10,15);

    delay(1000);
    updatatime=millis();
  }

}


void volcd()
{
  u8g.firstPage();  
  do {
    setFont_M;
    u8g.setPrintPos(0, 10); 
    u8g.print(s_data[9]);

    u8g.setPrintPos(0, 10+12*1); 
    u8g.print(s_data[21]);

    u8g.setPrintPos(0, 10+12*2); 
    u8g.print(tem);
    u8g.print("`");
    u8g.print(s_data[3]);
    u8g.print(" ");
    u8g.print(hum);
    u8g.print("%");

    u8g.setPrintPos(0, 14+12*3); 
    switch(lcd_switch_num)
    {
    case 0:    
      u8g.print("sunr: ");
      u8g.print(s_data[13]);
      break;

    case 1:    
      u8g.print("suns: ");
      u8g.print(s_data[14]);
      break;

    case 2:    
      u8g.print("uv: ");
      u8g.print(s_data[35]);
      break;

    case 3:    
      u8g.print("vis: ");
      u8g.print(s_data[32]);
      u8g.print(s_data[4]);
      break;

    }


    //-----------------------------------------
    float hudu=3.14*(wind_angle/180.0);
    int x,y;
    x=12*sin(hudu);
    y=12*cos(hudu);

#define xqishi 104
#define yqishi 42

    u8g.drawCircle(xqishi,yqishi,14);

    u8g.drawLine(xqishi, yqishi,   xqishi-x, yqishi+y);
    u8g.drawLine(xqishi, yqishi-1,   xqishi-x, yqishi+y);
    u8g.drawLine(xqishi, yqishi+1,   xqishi-x, yqishi+y);
    u8g.drawLine(xqishi-1, yqishi,   xqishi-x, yqishi+y);
    u8g.drawLine(xqishi+1, yqishi,   xqishi-x, yqishi+y);

    //-----------------------------------------

    setFont_S;
    if(wind_angle>140 && wind_angle<220)
    {
      u8g.setPrintPos(102, 54); 
      u8g.print("S");    
    }
    else
    {
      u8g.setPrintPos(102, 36); 
      u8g.print("N");    
    }


    u8g.setPrintPos(90, 63); 
    u8g.print(wind_level);
    u8g.print(s_data[5]);


    //    u8g.drawFrame(0, 54,128,10);
    u8g.setPrintPos(0, 63); 
    u8g.print("updata: ");
    u8g.print(updata[3]);
    u8g.print("/");
    u8g.print(updata[2]);
    u8g.print("  ");
    u8g.print(updata[1]);
    u8g.print(":");
    u8g.println(updata[0]);
  } 
  while( u8g.nextPage() );
}



void listSSIDResults(void)
{
  uint8_t valid, rssi, sec, index;
  char ssidname[33]; 

  index = cc3000.startSSIDscan();

  Serial.print(F("Networks found: ")); 
  Serial.println(index);
  Serial.println(F("================================================"));

  while (index) {
    index--;

    valid = cc3000.getNextSSID(&rssi, &sec, ssidname);

    Serial.print(F("SSID Name    : ")); 
    Serial.print(ssidname);
    Serial.println();
    Serial.print(F("RSSI         : "));
    Serial.println(rssi);
    Serial.print(F("Security Mode: "));
    Serial.println(sec);
    Serial.println();
  }
  Serial.println(F("================================================"));

  cc3000.stopSSIDscan();
}

/**************************************************************************/
/*!
 @brief  Tries to read the IP address and other connection details
 */
/**************************************************************************/
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if(!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("\r\nUnable to retrieve the IP Address!"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); 
    cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); 
    cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); 
    cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); 
    cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); 
    cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

