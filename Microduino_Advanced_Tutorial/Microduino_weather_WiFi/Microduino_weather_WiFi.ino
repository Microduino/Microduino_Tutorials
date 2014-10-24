#define WLAN_SSID       "XXX"        // 你的WIFI名字
#define WLAN_PASS       "XXX"                //你的WIFI密码

#define WLAN_SECURITY   WLAN_SEC_WPA2    // 你的WIFI加密方式： WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
//YEELINK---------------------------------------------
char website[] = "api.yeelink.net";

char urlBuf[4][42] = {  //改为你的传感器地址
  "/v1.0/device/XXX/sensor/XXX/datapoints",
  "/v1.0/device/XXX/sensor/XXX/datapoints",
  "/v1.0/device/XXX/sensor/XXX/datapoints",
  "/v1.0/device/XXX/sensor/XXX/datapoints",
};  

char apiKey[]  = "XXX";  //API KEY,在YEELINK里的账户-我的账户设置中

#define NUM 4

boolean NET_WEBSITE_sta=false;

//=============================
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


uint32_t ip;

Adafruit_CC3000_Client www;

//=============================
#define INTERVAL_LCD             100
#define INTERVAL_LCD_SWITCH      2000
#define INTERVAL_UPDATA          30000
#define INTERVAL_SENSOR          500

//*60*2

unsigned long updata_time=millis();
unsigned long lcd_time=millis();
unsigned long lcd_switch_time=millis();
unsigned long sensor_time=millis();

int lcd_switch_num=0;

//传感器部分================================
#include <Wire.h>
#include "I2Cdev.h"
//温湿度
#include <AM2321.h>
AM2321 am2321;
//气压
#include "BMP085.h"
BMP085 barometer;
//光照
#include <Adafruit_Sensor.h>
#include <Adafruit_TSL2561_U.h>
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);
//传感器部分================================

//传感器的值设置
float sensor_tem,sensor_hum,sensor_alt,sensor_pre,sensor_lux; //温度、湿度、海拔、气压、光照
//传感器的值设置



void setup(void)
{
  //  u8g.setRot180();
  //-------------------------------------------------------

  Serial.begin(115200);

  barometer.initialize();
  Serial.println(barometer.testConnection() ? "BMP085 connection successful" : "BMP085 connection failed");

  Serial.println(tsl.begin() ? "TSL2561 connection successful" : "TSL2561 connection failed");

  /* You can also manually set the gain or enable auto-gain support */
  // tsl.setGain(TSL2561_GAIN_1X);      /* No gain ... use in bright light to avoid sensor saturation */
  // tsl.setGain(TSL2561_GAIN_16X);     /* 16x gain ... use in low light to boost sensitivity */
  tsl.enableAutoGain(true);          /* Auto-gain ... switches automatically between 1x and 16x */

  /* Changing the integration time gives you better sensor resolution (402ms = 16-bit data) */
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      /* fast but low resolution */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_101MS);  /* medium resolution and speed   */
  //tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_402MS);  /* 16-bit data but slowest conversions */

  delay(500);

  Serial.println(F("Hello, Microduino!")); 

  //1/4----------------------------------------------------------
  Serial.println(F("\nInitializing..."));
  Serial.println("\n 1/4 begin");
  volcdsetup("1/4 begin READY",10,15);
  delay(1000);
  if (!cc3000.begin())
  {
    Serial.println("\n 1/4 begin ERROR");
    volcdsetup("1/4 begin ERROR",10,15);
    while(1);
  }
  Serial.println("1/4 begin OK");
  volcdsetup("1/4 begin OK",10,15);
  delay(1000);

  //2/4=====================================
  Serial.println("\n 2/4 connect AP READY");
  volcdsetup("2/4 connect AP READY",10,15);
  delay(1000);
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println("2/4 connect AP ERROR");
    volcdsetup("2/4 connect AP ERROR",10,15);
    Serial.println(F("Failed!"));
    while(1);
  }
  Serial.println("\n2/4 connect AP OK");
  volcdsetup("2/4 connect AP OK",10,15);
  delay(1000);

  //3/4=====================================
  Serial.println("\n 3/4 dhcp READY");
  volcdsetup("3/4 dhcp READY",10,15);
  delay(1000);
  if(!voCC3000_get_dhcp())
  {
    Serial.println("3/4 dhcp ERROR");
    volcdsetup("2/3 dhcp ERROR",10,15);
    delay(1000);
  }
  else
  {
    Serial.println("3/4 dhcp OK");
    volcdsetup("3/4 dhcp OK",10,15);
    delay(1000);
  }
  //4/4=====================================
  Serial.println("\n 4/4 server READY");
  volcdsetup("4/4 server READY",10,15);
  delay(1000);
  if(!voCC3000_get_server_ip())
  {
    Serial.println("4/4 server ERROR");
    volcdsetup("4/4 server ERROR",10,15);
    delay(1000);
  }
  else
  {
    NET_WEBSITE_sta = true;
    Serial.println("4/4 server OK");
    volcdsetup("4/4 server OK",10,15);
    delay(1000);
  }

  //----------------------------------------------------------
  //  cc3000.stop();
  // Optional SSID scan
  // listSSIDResults();
}



void loop(void)
{
  if (sensor_time > millis()) sensor_time = millis();
  if(millis()-sensor_time>INTERVAL_SENSOR)
  {
    //获取温湿度
    {
      am2321.read();
      sensor_tem=am2321.temperature/10.0;
      sensor_hum=am2321.humidity/10.0;
    }

    //获取气压、海拔
    {
      barometer.setControl(BMP085_MODE_TEMPERATURE);
      // wait appropriate time for conversion (4.5ms delay)
      unsigned long lastMicros = micros();
      while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
      // read calibrated temperature value in degrees Celsius
      barometer.getTemperatureC();
      // request pressure (3x oversampling mode, high detail, 23.5ms delay)
      barometer.setControl(BMP085_MODE_PRESSURE_3);
      while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());
      // read calibrated pressure value in Pascals (Pa)
      sensor_pre = barometer.getPressure();

      // calculate absolute altitude in meters based on known pressure
      // (may pass a second "sea level pressure" parameter here,
      // otherwise uses the standard value of 101325 Pa)
      sensor_alt = barometer.getAltitude(sensor_pre);
      // that is equal to 101500 Pascals.
    }

    //获取光照
    {
      sensors_event_t event;
      tsl.getEvent(&event);

      /* Display the results (light is measured in lux) */
      if (event.light)
        sensor_lux=event.light;
      else
        Serial.println("Sensor overload");
    }
    sensor_time=millis();
  }


  if (lcd_time > millis()) lcd_time = millis();
  if(millis()-lcd_time>INTERVAL_LCD)
  {
    volcd();
    lcd_time=millis();
  }


  if(updata_time>millis()) updata_time=millis();
  if(millis()-updata_time>INTERVAL_UPDATA)
  {
    //----------------------------------------------------------
    for(int a=0;a<NUM;a++)
    {
      int pos_vol=0;

      switch(a)
      {
      case 0:
        pos_vol=sensor_tem*10;
        break;
      case 1:
        pos_vol=sensor_hum*10;
        break;
      case 2:
        pos_vol=sensor_pre/1000.0*10;
        break;
      case 3:
        pos_vol=sensor_lux*10;
        break;
      }

      String Serial_DATA="Num " + (String)a + " Vol is: " + (String)pos_vol;
      Serial.println(Serial_DATA);
      Serial_DATA="";

      NET_WEBSITE_sta=voCC3000_pos(a,pos_vol);
      voCC3000_rec();
    }

    if(NET_WEBSITE_sta)
      Serial.println("\n ---UPDATA OK---");
    else
      Serial.println("\n --UPDATA REEOR--");

    updata_time=millis();
  }

}


bool voCC3000_get_dhcp()
{
  bool cache;
  //  Serial.println(F("Request DHCP"));

  while (!cc3000.checkDHCP())
  {
    cache=false;
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
  cache=true;

  if(!cache)
    return false;
  else
  {
    /* Display the IP address DNS, Gateway, etc. */
    while (! displayConnectionDetails())
    {
      cache=false;
      delay(500);
    }
    cache=true;

    return cache ? true :false;
  }
}

bool voCC3000_get_server_ip()  
{
  ip = 0;
  // Try looking up the website's IP address
  Serial.print(website); 
  Serial.print(F(" -> "));
  while (ip == 0) {
    if (! cc3000.getHostByName(website, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    Serial.println(F("while!"));

    delay(500);
  }

  cc3000.printIPdotsRev(ip);
  Serial.println(F("  "));

  return ip ? true :false;
}

boolean voCC3000_pos(int a,int b)  //num,vol
{
  // Optional: Do a ping test on the website
  /*
  Serial.print(F("\n\rPinging ")); cc3000.printIPdotsRev(ip); Serial.print("...");  
   replies = cc3000.ping(ip, 5);
   Serial.print(replies); Serial.println(F(" replies"));
   */

  // calculate the length of the sensor reading in bytes:
  // 8 bytes for {"value":} + number of digits of the data:
  int thisLength = 12 + getLength(b);

  www = cc3000.connectTCP(ip, 80);
  Serial.println("Ping");

  if (www.connected()) {
    Serial.println("connecting.start..");
    // send the HTTP PUT request:
    www.print("POST ");
    www.print(urlBuf[a]);
    www.println(" HTTP/1.1");
    www.print("Host: "); 
    www.println(website);
    www.println("Accept: */*");
    www.print("U-ApiKey: ");
    www.println(apiKey);
    www.print("Content-Length: ");
    www.println(thisLength);

    www.println("Content-Type: application/x-www-form-urlencoded");
    www.println("Connection: close");
    www.println(); 

    // here's the actual content of the PUT request:
    www.print("{\"value\":");
    www.print(b/10);
    www.print(".");
    www.print(b%10);
    www.println("}");

    Serial.println("connecting.end..");

    return true;
  }
  else  
    return false;
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
    }
  }
  www.close();

  /* You need to make sure to clean up after yourself or the CC3000 can freak out */
  /* the next time your try to connect ... */
  //  cc3000.disconnect();
  //  Serial.println(F("\n\nDisconnecting"));

  Serial.println(F("connect end--------------------------------"));
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

int getLength(int someValue) {
  // there's at least one byte:
  int digits = 1;
  // continually divide the value by ten,
  // adding one to the digit count for each
  // time you divide, until you're at 0:
  int dividend = someValue /10;
  while (dividend > 0) {
    dividend = dividend /10;
    digits++;
  }
  // return the number of digits:
  return digits;
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

void volcd()
{
  u8g.firstPage();  
  do {
    setFont_L;

    u8g.setPrintPos(0, 15*1); 
    u8g.print(sensor_tem ,1);
    u8g.print(" `C");
    u8g.print(" ");
    u8g.print(sensor_hum ,1);
    u8g.print(" %");

    u8g.setPrintPos(0, 15*2); 
    u8g.print(sensor_pre/1000.0 ,3);
    u8g.print(" kPa");

    /*
     u8g.setPrintPos(0, 15*3); 
     u8g.print(sensor_alt ,1);
     u8g.print(" Meters");
     */

    u8g.setPrintPos(0, 15*3); 
    u8g.print(sensor_lux ,1);
    u8g.print(" Lux");

    setFont_M;

    u8g.setPrintPos(0, 15*4); 
    if(NET_WEBSITE_sta)
      u8g.print("Online.");
    else
      u8g.print("Offline.");

    if(millis()/500%2!=0)
      u8g.print(".");

  } 
  while( u8g.nextPage() );
}
