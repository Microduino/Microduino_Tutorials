
#include "U8glib.h" //调用库
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE); //设置OLED型号
//-------字体设置，大、中、小
#define setFont_L u8g.setFont(u8g_font_7x13)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar) 

#include <Servo.h>
#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include "utility/debug.h"
#include "utility/socket.h"

#include <Wire.h> //调用库


#define INTERVAL_LCD 1000 //定义OLED刷新时间间隔 

// These are the interrupt and control pins
#define ADAFRUIT_CC3000_IRQ   2  // MUST be an interrupt pin!
// These can be any two pins
#define ADAFRUIT_CC3000_VBAT  9
#define ADAFRUIT_CC3000_CS    10
// Use hardware SPI for the remaining pins
// On an UNO, SCK = 13, MISO = 12, and MOSI = 11
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIV2); // you can change this clock speed but DI



#define WLAN_SSID       "rayHome"        // cannot be longer than 32 characters!
#define WLAN_PASS       "34689127Ray"
// Security can be WLAN_SEC_UNSEC, WLAN_SEC_WEP, WLAN_SEC_WPA or WLAN_SEC_WPA2
#define WLAN_SECURITY   WLAN_SEC_WPA2

#define LISTEN_PORT           777    // What TCP port to listen on for connections.  The echo protocol uses port 777.

Adafruit_CC3000_Server carServer(LISTEN_PORT);

unsigned long lcd_time = millis(); //OLED刷新时间计时器 


Servo myservo;
//int pos = 90; // initial position
int angle=90, speed=0;

String result;
String currentInfo;
String IPAdressShow;

void setup() {

  Serial.begin(115200);

  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(7, OUTPUT);

  Serial.println(F("Hello, CC3000!\n")); 

  Serial.print("Free RAM: "); Serial.println(getFreeRam(), DEC);
  
  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
    Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
   
  Serial.println(F("Connected!"));
  
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  

  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(1000);
  }

  //start control your car
  Serial.println("Start Control Your Car");
  
  // Start listening for connections
  carServer.begin();
  
  Serial.println(F("Listening for connections..."));


  digitalWrite(7, HIGH);
  digitalWrite(5, LOW);


	// initialize serial communication at 9600 bits per second:
	myservo.attach(3); // attaches the servo on pin 11 to the servo object

	myservo.write(angle);
	delay(200); // a 2 seconds delay while we position the solar panel



}
void loop() {

  // Try to get a client which is connected.
  Adafruit_CC3000_ClientRef client = carServer.available();
  if (client) {
     boolean currentLineIsBlank = true;
     // Check if there is data available to read.
     while (client.available()) {
     
       char c = client.read();
       result = result + c;
       //Serial.write(c);
       
       // Delete HTTP headers
      if(result.endsWith("end"))
      {
        currentInfo=result;
        result="";
      }
       
       if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          // client.println("HTTP/1.1 200 OK");
          // client.println("Content-Type: text/html");
          // client.println("Connection: close");
          // client.println();
       }
       if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
       
     }

     // give the web browser time to receive the data
    //delay(5);
    // close the connection:
    //client.close();
    //Serial.println("client disconnected");
    
    
    // Reset result variable
    result = "";
  }


  //print currentInfo
  if(currentInfo!=NULL) {

      for (int i = 0; i < currentInfo.length(); i++) {
        if (currentInfo.substring(i, i+1) == ":") {
          angle = currentInfo.substring(0, i).toInt();
          speed = currentInfo.substring(i+1).toInt();
          break;
        }
      }



      // Serial.print(angle);
      // Serial.print("----");
      // Serial.println(speed);
      if(angle!=90) {
        myservo.write(angle);
      }
      
      if(speed>0) {
        //speedControlForward(abs(speed));
        fullForward();
      } else if(speed==0){
        stop();
      } else {
        //speedControlBackward(abs(speed));
        fullBackward();
      }

  }

    if (lcd_time > millis()) lcd_time = millis();
    if(millis()-lcd_time>INTERVAL_LCD) {
      volcd(); //调用显示库
      lcd_time = millis();
    } 




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
    //Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {

    char ipSegment[5];

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 24));
    IPAdressShow=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 16));
    IPAdressShow+=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress >> 8));
    IPAdressShow+=ipSegment;
    IPAdressShow+=".";

    snprintf(ipSegment, sizeof ipSegment, "%d", (uint8_t)(ipAddress));
    IPAdressShow+=ipSegment;

    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

//full forward
void fullForward() {
  digitalWrite(6, HIGH);
  digitalWrite(8, LOW);
  //delay(2000);
}

//stop
void stop() {
  digitalWrite(6, LOW);
  digitalWrite(8, LOW);
  //delay(2000);
}

//full backward
void fullBackward() {
  digitalWrite(6, LOW);
  digitalWrite(8, HIGH);
  //delay(2000);
}

void speedControlForward(int speed) {
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);
  analogWrite(6, speed);
}

void speedControlBackward(int speed) {
  digitalWrite(6, HIGH);
  digitalWrite(8, HIGH);
  analogWrite(8, speed);
}


//显示函数 
void volcd() {
  u8g.firstPage();
  do {
    setFont_M;
    u8g.setPrintPos(5, 10);
    u8g.print("IP: ");
    u8g.setPrintPos(20, 10);
    u8g.print(IPAdressShow);
    

    u8g.setPrintPos(5, 20);
    u8g.print("angle: ");
    u8g.setPrintPos(40, 20);
    u8g.print(angle);

    u8g.setPrintPos(5, 30);
    u8g.print("speed: ");
    u8g.setPrintPos(40, 30);
    u8g.print(speed);

  }
  while( u8g.nextPage() );
} 
