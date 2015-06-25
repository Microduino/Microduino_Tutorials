#define INTERVAL_LCD             100             //定义OLED刷新时间间隔  
#define INTERVAL_SENSOR          1000             //定义传感器采样时间间隔  
#define INTERVAL_NET             30000             //时间间隔  
#define IDLE_TIMEOUT_MS          1000 
#define INTERVAL_BUTTON          1000

#define WLAN_SSID       "Makermodule"        // cannot be longer than 32 characters!
#define WLAN_PASS       "microduino"


#include"AirQuality.h"
#include"Arduino.h"
AirQuality airqualitysensor;


SoftwareSerial mySerial(4, 5); //RX,TX

#define my_Serial mySerial
//#define my_Serial Serial1  //Core+


uint32_t ip;

String my_app_kit_id="qwaBFu7se48GNAD3q";

//String my_app_kit_id="MHPQyfDmxypZfoDDR";

Adafruit_CC3000_Client www;


// #define WEBSITE "192.168.199.240"
// #define WEBSITEPORT 3000
#define WEBSITE "mcotton-01.chinacloudapp.cn"
#define WEBSITEPORT 80


#define WEBPAGE "/api/v1.0/d"

#define WEBUTTONPAGE "/api/v1.0/ce"

bool NET_WEBSITE_sta;
bool stopFlag=false;

int lastButtonState = 1;         // variable for reading the pushbutton status

const int buttonPin = 6;     // the number of the pushbutton pin
//const int ledPin =  5;      //7,8,11,12,13// the number of the LED pin
const int ledPin =  7;

bool buttonState = false;         // variable for reading the pushbutton status

bool statusChange=false;

bool led_sta=false;
//char s_data;
char buf[10];
char c;

//char json_string[150];
void freeMem(char* message) 
{
  extern unsigned int __heap_start, *__brkval; 
  unsigned int v;
  Serial.print(message);
  Serial.print(":");
  Serial.println((unsigned int) &v - (__brkval == 0 ? (unsigned int) &__heap_start : (unsigned int) __brkval));
}

int start=0;
int end=0;

String returnStateData;
String stateValue;

//3,传感器值的设置    
float sensor_tem, sensor_hum,sensor_light,Sensor_cal,Sensor_etoh; //温度、湿度,光照,PM2.5,甲烷

unsigned long lcd_time = millis();                 //OLED刷新时间计时器  
unsigned long sensor_time = millis();             //传感器采样时间计时器  
unsigned long net_time = millis();             //计时器  
unsigned long button_time = millis();             //计时器  

String dataToSend;

int cal[7];

int sum;

int num;

int change[2];

int current_quality =-1;

char s_data;

int cal_data;

