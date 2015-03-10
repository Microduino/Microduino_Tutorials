//small sanlun

#define motor_pinB 6
#define motor_pinA 8  //PWM


#define servo_pinB 5
#define servo_pinA 7  //PWM 



String result;
String currentInfo;
String IPAdressShow;

int angle=90, speed=0;

#define INTERVAL_LCD 10 //定义OLED刷新时间间隔 

#define WLAN_SSID       "rayHome"        // cannot be longer than 32 characters!
#define WLAN_PASS       "34689127Ray"

#define LISTEN_PORT           777    // What TCP port to listen on for connections.  The echo protocol uses port 777.

Adafruit_CC3000_Server carServer(LISTEN_PORT);