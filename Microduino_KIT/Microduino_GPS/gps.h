#include "arduino.h"

boolean STA;	//GPS状态

float f_latitude,f_longitude;	//经纬度
char c_lat,c_lon;		//经纬极向

int itime[3];	//时间
int idate[3];	//日期

float f_Speed;	//速度
int i_Speed[2];	//速度格式化

float f_Height;	//海拔

int i_satellites;	//卫星数

float f_fixquality;	//信号质量

#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
Adafruit_GPS GPS(&GPS_Serial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

//lat_lon_transform================================
void lat_lon_transform()
{
  f_latitude=(int(f_latitude)/100)+((int(f_latitude)%100)/60.0)+((f_latitude-int(f_latitude))/60.0);
  if(c_lat=='S')  f_latitude=-f_latitude;  //南纬

  f_longitude=(int(f_longitude)/100)+((int(f_longitude)%100)/60.0)+((f_longitude-int(f_longitude))/60.0);
  if(c_lon=='W')  f_longitude=-f_longitude;  //西经
}

//GPS========================================
void vogps_databegin()
{
  digitalWrite(PIN_LED2,true);	//LED2亮：GPS开始读取

  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef DEBUG
  if (GPSECHO)  if(c)  Serial.print(c);
#endif
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) 
  {
    // a tricky thing here is if we print the NMEA sentence, or data
    // we end up not listening and catching other sentences!
    // so be very wary if using OUTPUT_ALLDATA and trytng to print out data
    //    Serial.println(GPS.lastNMEA()); // this also sets the newNMEAreceived() flag to false
    if (!GPS.parse(GPS.lastNMEA())) // this also sets the newNMEAreceived() flag to false
      return; // we can fail to parse a sentence in which case we should just wait for another
  }

  digitalWrite(PIN_LED2,false);	//LED2灭：GPS结束读取
}

void vogps_dataread()
{
  if (timer > millis()) timer = millis();
  if (millis() - timer > init_updata)
  {
    timer = millis(); // reset the timer

    itime[0]=GPS.hour;
    itime[1]=GPS.minute;
    itime[2]=GPS.seconds;

    idate[0]=GPS.year;
    idate[1]=GPS.month;
    idate[2]=GPS.day;

    f_fixquality=GPS.fixquality;	//信号质量
    STA=GPS.fix;					//GPS定位状态

#ifdef DEBUG
    Serial.println("=== GPS READ ===");

    Serial.print("=== GPS Time: ");
    Serial.print(itime[0]);
    Serial.print(":");
    Serial.print(itime[1]);
    Serial.print(":");
    Serial.print(itime[2]);
    Serial.print("=== GPS Date: ");
    Serial.print(idate[2]); 
    Serial.print('/');
    Serial.print(idate[1]); 
    Serial.print("/20");
    Serial.println(idate[0]);
    Serial.print("=== GPS Fix: "); 
    Serial.print(STA);
    Serial.print(" quality: "); 
    Serial.println(f_fixquality);
#endif

    if (STA)		//当GPS定位上
    {
      f_latitude=GPS.latitude;
      f_longitude=GPS.longitude;
      c_lat=GPS.lat;
      c_lon=GPS.lon;

      lat_lon_transform();		//经纬度转化

      f_Speed=1.852*GPS.speed;			//速度转化
      i_Speed[0]=int(f_Speed*10)%10;	//速度格式化
      i_Speed[1]=int(f_Speed);			//速度格式化

      f_Height=GPS.altitude;			//海拔

      i_satellites=GPS.satellites;      //卫星数

#ifdef DEBUG
      Serial.println(" ");
      Serial.print("=== GPS lat/lon data: ");
      Serial.print(f_latitude,6);
      Serial.print("   ");
      Serial.print(f_longitude,6);
      Serial.println(" "); 
      Serial.print("=== GPS f_Speed: ");
      Serial.print(f_Speed,1);
      Serial.println(" ");
      Serial.print("=== GPS Altitude: "); 
      Serial.print(f_Height);
      Serial.println(" ");
      Serial.print("=== GPS Satellites: "); 
      Serial.println(i_satellites);
      /*
       Serial.print("Angle: "); 
       Serial.println(GPS.angle);
       */

      Serial.println("=== GPS READ Complete === ");
      Serial.println(" ");
#endif
    }
  }
}
