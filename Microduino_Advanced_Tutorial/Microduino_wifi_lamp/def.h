#define PIXEL_PIN    A0    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  6

String result;
String currentInfo;
String IPAdressShow;

char buffer[2000];

boolean buffer_sta=false;

int buffer_num=0;

int sta[3];

int red,green,blue;

#define INTERVAL_LCD 10 //定义OLED刷新时间间隔 

#define WLAN_SSID       "QCMaker Guest"        // cannot be longer than 32 characters!
#define WLAN_PASS       "qingchengchuangke"

#define LISTEN_PORT    777    // What TCP port to listen on for connections.  The echo protocol uses port 777.

Adafruit_CC3000_Server carServer(LISTEN_PORT);

