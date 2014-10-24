#define INTERVAL_LCD             100             //定义OLED刷新时间间隔  
#define INTERVAL_SENSOR          500             //定义传感器采样时间间隔  

unsigned long lcd_time = millis();                 //OLED刷新时间计时器  
unsigned long sensor_time = millis();             //传感器采样时间计时器  

//1,屏幕显示部分=============================   
#include "U8glib.h"                               //调用库  
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);     //设置OLED型号  
//-------字体设置，大、中、小   
#define setFont_L u8g.setFont(u8g_font_7x13)   
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)   
#define setFont_S u8g.setFont(u8g_font_chikitar)   

//2,传感器部分================================   
#include <Wire.h>                                 //调用库  
#include "I2Cdev.h"                             //调用库  
//温湿度   
#include <AM2321.h>                             //调用库  
AM2321 am2321;   
//气压   
#include "BMP085.h"                             //调用库  
BMP085 barometer;   
//光照   
#include <Adafruit_Sensor.h>                     //调用库  
#include <Adafruit_TSL2561_U.h>                 //调用库  
Adafruit_TSL2561_Unified tsl = Adafruit_TSL2561_Unified(TSL2561_ADDR_LOW, 12345);   

//3,传感器值的设置  
float sensor_tem, sensor_hum, sensor_alt, sensor_pre, sensor_lux; //温度、湿度、海拔、气压、光照   

void setup(void)     //初始化函数  
{       
  //初始化串口波特率  
  Serial.begin(115200);       

  //初始化-气压   
  barometer.initialize();   
  Serial.println(barometer.testConnection() ? "BMP085 successful" : "BMP085 failed");   

  //初始化-光照   
  Serial.println(tsl.begin() ? "TSL2561 successful" : "TSL2561 failed");   
  tsl.enableAutoGain(true);                                  // 在1x 与16x 增益中切换  
  tsl.setIntegrationTime(TSL2561_INTEGRATIONTIME_13MS);      //13MS的采样速度  

  delay(500);       //延时500ms   
}   





void loop(void)     //循环函数  
{   
  if (sensor_time > millis()) sensor_time = millis();  
  if(millis() - sensor_time > INTERVAL_SENSOR)              //传感器采样时间间隔  
  {   
    //获取温湿度============================================== 
    am2321.read();   
    sensor_tem = am2321.temperature / 10.0;   
    sensor_hum = am2321.humidity / 10.0;   

    //获取气压、海拔========================================== 
    barometer.setControl(BMP085_MODE_TEMPERATURE);   
    unsigned long lastMicros = micros();      
    //先获取温度，用来补偿气压值  
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());    
    barometer.getTemperatureC();  
    barometer.setControl(BMP085_MODE_PRESSURE_3);   
    //得出气压值  
    while (micros() - lastMicros < barometer.getMeasureDelayMicroseconds());  
    sensor_pre = barometer.getPressure();  
    //结合气压值，以标准大气压得出海拔值  
    sensor_alt = barometer.getAltitude(sensor_pre);    

    //获取光照===============================================    
    sensors_event_t event;   
    tsl.getEvent(&event);   
    (event.light)?  sensor_lux = event.light : Serial.println("Sensor overload");   

    sensor_time = millis();   
  }   
  if (lcd_time > millis()) lcd_time = millis();          
  if(millis()-lcd_time>INTERVAL_LCD) 
  {   
    volcd();                       //调用显示库  
    lcd_time = millis();   
  }   
}   

void volcd()     //显示函数  
{    
  u8g.firstPage();   
  do  {   
    setFont_L;                             //设置字体为大  
    u8g.setPrintPos(0, 15 * 1);         //设置文字开始坐标  
    u8g.print(sensor_tem , 1);             //温度  
    u8g.print(" `C ");   
    u8g.print(sensor_hum , 1);             //湿度  
    u8g.print(" %");   
    u8g.setPrintPos(0, 15 * 2);          //设置文字开始坐标  
    u8g.print(sensor_pre/1000.0,3);     //大气压强  
    u8g.print(" kPa");   
    u8g.setPrintPos(0, 15 * 3);         //设置文字开始坐标  
    u8g.print(sensor_alt , 1);             //海拔  
    u8g.print(" Meters");   
    u8g.setPrintPos(0, 15 * 4);         //设置文字开始坐标  
    u8g.print(sensor_lux , 1);             //光照强度  
    u8g.print(" Lux");   
  }   
  while( u8g.nextPage() );   
}  
