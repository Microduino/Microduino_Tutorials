//small sanlun
#define motor_pin0A 8  //PWM
#define motor_pin0B 5
#define motor_pin1A 7  //PWM 
#define motor_pin1B 6


String result;
String currentInfo;
String IPAdressShow;

int angle=90, speed=0;

#define INTERVAL_LCD 10 //定义OLED刷新时间间隔 

#define WLAN_SSID       "HiWiFi_ylb"        // cannot be longer than 32 characters!
#define WLAN_PASS       "microduino"

#define LISTEN_PORT           777    // What TCP port to listen on for connections.  The echo protocol uses port 777.

Adafruit_CC3000_Server carServer(LISTEN_PORT);

