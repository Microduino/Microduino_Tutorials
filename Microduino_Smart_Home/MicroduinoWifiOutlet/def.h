//small sanlun

#define outletPin 6

String result;
String currentInfo;
String IPAdressShow;

boolean outletState=false;
String outletStateInfo="OFF";

#define INTERVAL_LCD 10 //定义OLED刷新时间间隔 

#define WLAN_SSID       "your ssid"        // cannot be longer than 32 characters!
#define WLAN_PASS       "your Password"

#define LISTEN_PORT           888    // What TCP port to listen on for connections.  The echo protocol uses port 777.

Adafruit_CC3000_Server outletServer(LISTEN_PORT);
