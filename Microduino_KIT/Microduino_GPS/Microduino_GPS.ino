/*
=================================
 本作品采用知识共享 署名-非商业性使用-相同方式共享 3.0 未本地化版本 许可协议进行许可
 访问 http://creativecommons.org/licenses/by-nc-sa/3.0/ 查看该许可协议
 版权所有：老潘orz
 =================================
 
 Microduino维基： wiki.microduino.cc
 
 原帖：http://www.geek-workshop.com/thread-11199-1-1.html 
 */

//==========================
//#define DEBUG		//是否开启串口DEBUG功能
#define GPS_Serial Serial1		//选择GPS通讯串口

#define PIN_LED1 -1	//SD状态提示LED1
#define PIN_LED2 -1	//GPS状态提示LED2
#define PIN_key1 5	//按键1
#define PIN_bat A7	//BAT

#define init_bat 5000			//电池电量刷新时间
#define init_updata 500			//gps数据刷新时间
#define init_sdwrite 3000		//SD卡写入间隔
#define init_oled 600			//OLED刷新时间
#define init_oled_updata 2000	//提示生成文件名持续时间

#define _V_max 42		//锂电池满电电压4.2V
#define _V_min 30		//锂电池没电电压3.0V

//==========================

int _V_bat=_V_min;
void vobat()
{
  _V_bat=10*((5.0f*analogRead(PIN_bat)/1023.0f)/(33.0f/(51.0f+33.0f)));
  if(_V_bat<_V_min) _V_bat=_V_min;
  if(_V_bat>_V_max) _V_bat=_V_max;

#ifdef DEBUG
  Serial.print("_V_bat: ");
  Serial.println(_V_bat);
#endif
}

//==========================
unsigned long time_oled = millis();
unsigned long time_oled_updata = millis();
unsigned long timer = millis();
unsigned long time_sdwrite = millis();
unsigned long time_bat = millis();

boolean b_oled_updata=false;	//按键动作触发OLED

//==========================
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


//PKJ=======================================
#define num_name 13		//文件名字长

char file_name[num_name]="";	//文件名
String file_name_cache ="";		//文件名缓存

boolean file_updata;		//是否更新新的GPX文件
boolean sd_sta,file_sta;	//SD卡状态、文件状态
int file_num;				//文件名后两位序号

int idate_cache;			//日期（日）缓存


//oled=======================================
#include <U8glib.h>
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);	// HW SPI Com: CS = 10, A0 = 9 (Hardware Pins are  SCK = 13 and MOSI = 11)

//-------字体设置，大、中、小
#define setFont_L  u8g.setFont(u8g_font_courB14)
#define setFont_M u8g.setFont(u8g_font_fixed_v0r)
#define setFont_S u8g.setFont(u8g_font_chikitar)
/*
font:
 u8g.setFont(u8g_font_7x13)
 u8g.setFont(u8g_font_fixed_v0r);
 u8g.setFont(u8g_font_chikitar);
 u8g.setFont(u8g_font_osb21);
 */

#define u8g_logo_width 128
#define u8g_logo_height 18

const unsigned char u8g_logo_bits[] U8G_PROGMEM =
{
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC0, 0x01, 0xE0,
  0x03, 0x00, 0x00, 0x00, 0x00, 0x7E, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00,
  0x00, 0xFE, 0xF9, 0xF7, 0x07, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x00, 0xF8,
  0x03, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xF9, 0xE1, 0x03, 0x00, 0x00, 0x00,
  0x00, 0x38, 0x00, 0xF0, 0x01, 0x00, 0x00, 0x00, 0x00, 0xFC, 0xFF, 0x01,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x38, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0xFC, 0xEF, 0xF9, 0x8F, 0xD7, 0x73, 0xF1, 0xC1, 0x3B, 0x9F, 0xFF,
  0xFF, 0x1E, 0x3E, 0x00, 0x00, 0xBC, 0xEF, 0xC1, 0xE1, 0x9F, 0xFF, 0xDD,
  0xE3, 0x3F, 0xCC, 0xE1, 0xF0, 0xBF, 0x7B, 0x00, 0x00, 0x3C, 0xF7, 0xE1,
  0xE1, 0x9F, 0xFF, 0xC6, 0xF7, 0x3E, 0x8E, 0xF3, 0xF0, 0xFF, 0xF8, 0x00,
  0x00, 0x3C, 0xF3, 0xE1, 0xF1, 0x93, 0xFF, 0xE6, 0xF7, 0x3C, 0x8F, 0xF7,
  0xF0, 0xFF, 0xFC, 0x00, 0x00, 0x7C, 0xF2, 0xE1, 0xF1, 0x83, 0x87, 0xFE,
  0xF7, 0x39, 0xFF, 0xF7, 0xF0, 0xFF, 0xFF, 0x00, 0x00, 0x7C, 0xF0, 0xE3,
  0xF3, 0xA3, 0x03, 0xFE, 0xF7, 0x3F, 0xFF, 0xF7, 0x71, 0xFC, 0xFF, 0x00,
  0x00, 0x7C, 0xF8, 0xE3, 0xF3, 0xBF, 0x03, 0xFE, 0xE3, 0x3F, 0xFF, 0xF3,
  0x71, 0xDC, 0x7F, 0x00, 0x00, 0x7E, 0xFC, 0xE7, 0xE3, 0xBF, 0x03, 0xFC,
  0xE3, 0x3F, 0xFE, 0xF3, 0x71, 0x9C, 0x7F, 0x00, 0x00, 0xC1, 0x03, 0xF8,
  0xCF, 0xE7, 0x0F, 0xF0, 0x00, 0x7F, 0xFC, 0xFC, 0xFF, 0x3E, 0x1E, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

//==========================
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
Adafruit_GPS GPS(&GPS_Serial);

// Set GPSECHO to 'false' to turn off echoing the GPS data to the Serial console
// Set to 'true' if you want to debug and listen to the raw GPS sentences
#define GPSECHO false

//SD=======================================
#include <SD.h>

File myFile;

//EEPROM====================================
#include <EEPROM.h>
#define EEPROM_write(address, p) {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) EEPROM.write(address+i, pp[i]);}
#define EEPROM_read(address, p)  {int i = 0; byte *pp = (byte*)&(p);for(; i < sizeof(p); i++) pp[i]=EEPROM.read(address+i);}

struct config_type
{
  long l_long;
  float f_float;
  int i_int_idate;			//日期缓存
  int i_int_num;			//文件序号
  char c_char[num_name];	//文件名
};

void eeprom_read()
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("===EEPROM===");
#endif
  config_type config_readback;
  EEPROM_read(0, config_readback);		//从0地址处读取

  // 4) 通过串口输出读取回来的资料

  idate_cache=config_readback.i_int_idate;	//EEPROM读出日期（日）缓存
  file_num=config_readback.i_int_num;		//EEPROM读出文件序号

  for(int a=0;a<(num_name-1);a++)			//EEPRO读出文件名
  {
    file_name[a]=config_readback.c_char[a];
  }

  file_name_cache=file_name;		//启用文件名缓存

#ifdef DEBUG
  Serial.print("=== EEPROM file_name:");
  Serial.println(file_name);
  Serial.print("=== EEPROM file_num:");
  Serial.println(file_num);
  Serial.print("=== EEPROM idate_cache:");
  Serial.println(idate_cache);


  Serial.println("=== EEPROM Complete===");
  Serial.println(" ");
#endif
}  

void eeprom_write()
{
  config_type config;

  config.l_long=9999999;
  config.f_float = 3.14;
  config.i_int_idate = idate_cache;		//赋值：日期（日）缓存
  config.i_int_num = file_num;			//赋值：文件序号
  strcpy(config.c_char, file_name);		//赋值：文件名

  EEPROM_write(0, config);		//从0地址处写入
}

// FileName_StringtoChar================================
void  vostring()
{
  String s_file_name="";

  for(int a=0;a<3;a++)
  {
    if(idate[a]<10)
      s_file_name+="0";
    s_file_name+=idate[a];
  }
  if(file_num<10)
    s_file_name+="0";
  s_file_name+=file_num;
  s_file_name+=".gpx";

  for(int a=0;a<(num_name-1);a++)
  {
    file_name[a]=s_file_name[a];
  }
}

//KEY==========================================
boolean key1;			//按键
boolean key1_cache;		//检测按键松开缓存

boolean vokey(int _vokey)
{
  key1=digitalRead(_vokey);

  if(key1 && key1_cache)		//按下松开后
  {
    key1_cache=!key1;		//缓存作判断用
    return true;
  }
  else
  {
    key1_cache=!key1;		//缓存作判断用
    return false;
  }
}

//GPS========================================
void vogps_databegin()
{
  digitalWrite(PIN_LED2,true);	//LED2亮：GPS开始读取

  char c = GPS.read();
  // if you want to debug, this is a good time to do it!
#ifdef DEBUG
  if (GPSECHO)
    if (c) Serial.print(c);
#endif
  // if a sentence is received, we can check the checksum, parse it...
  if (GPS.newNMEAreceived()) {
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

    /*
    Serial.print("int Time: ");
     Serial.print(itime[0]);
     Serial.print(":");
     Serial.print(itime[1]);
     Serial.print(":");
     Serial.print(itime[2]);
     Serial.println(" ");
     */

#ifdef DEBUG
    Serial.println("=== GPS READ ===");

    Serial.print("=== GPS Time: ");
    Serial.print(GPS.hour, DEC); 
    Serial.print(':');
    Serial.print(GPS.minute, DEC); 
    Serial.print(':');
    Serial.print(GPS.seconds, DEC); 
    Serial.print('.');
    Serial.println(GPS.milliseconds);
    Serial.print("=== GPS Date: ");
    Serial.print(GPS.day, DEC); 
    Serial.print('/');
    Serial.print(GPS.month, DEC); 
    Serial.print("/20");
    Serial.println(GPS.year, DEC);
    Serial.print("=== GPS Fix: "); 
    Serial.print((int)GPS.fix);
    Serial.print(" quality: "); 
    Serial.println((int)GPS.fixquality);
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
      Serial.print(GPS.altitude);
      Serial.println(" ");
      Serial.print("=== GPS Satellites: "); 
      Serial.println((int)GPS.satellites);

      Serial.println("=== GPS READ Complete === ");
      Serial.println(" ");
#endif
      /*
      Serial.print("Location: ");
       Serial.print(GPS.latitude, 4); 
       Serial.print(GPS.lat);
       Serial.print(", ");
       Serial.print(GPS.longitude, 4); 
       Serial.println(GPS.lon);
       Serial.print("Speed (knots): "); 
       Serial.println(GPS.speed);
       Serial.print("Angle: "); 
       Serial.println(GPS.angle);
       */
    }
  }
}

//lat_lon_transform================================
void lat_lon_transform()
{
  f_latitude=(int(f_latitude)/100)+((int(f_latitude)%100)/60.0)+((f_latitude-int(f_latitude))/60.0);
  if(c_lat=='S')		//南纬
    f_latitude=-f_latitude;

  //---------------------------------

  f_longitude=(int(f_longitude)/100)+((int(f_longitude)%100)/60.0)+((f_longitude-int(f_longitude))/60.0);
  if(c_lon=='W')		//西经
    f_longitude=-f_longitude;
}

//SD==========================================
void vosdbegin(char* name_cache)		//SD卡文件初始检测
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD BEGIN ===");
#endif
  if (SD.exists(name_cache)) 	//如果SD卡内有当前需记录的文件
  {
#ifdef DEBUG
    Serial.println("=== SD exists.OK");
#endif
    file_sta=true;    
  }
  else 							//如果SD卡内无当前需记录的文件
  {
#ifdef DEBUG
    Serial.println("=== SD exist.ERROR");

    Serial.println("=== SD Creating NEW.HEAD...");
#endif
    vosdwrite(file_name,'H');	//创建新文件头
    //    vosdread(file_name);

#ifdef DEBUG
    Serial.println("  ");
    Serial.print("=== SD The NUM ");
    Serial.print(file_num);
    Serial.print(" File:");
    Serial.print(file_name);
    Serial.print(" is ");
    Serial.print("Create");
    Serial.println("  ");
#endif
    // open a new file and immediately close it:

    if (SD.exists(name_cache)) 	//再次验证有无当前需记录的文件
    {
#ifdef DEBUG
      Serial.println("=== SD exists.OK");
#endif
      file_sta=true;    
    }
    else 
    {
#ifdef DEBUG
      Serial.println("=== SD exist.ERROR");
#endif
      file_sta=false;    
    }
  }

#ifdef DEBUG
  Serial.println(" ");
  Serial.println("===SD BEGIN Complete===");
#endif
}

void vosdread(char* name_cache)		//SD卡文件读取
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD READ ===");
#endif

  myFile = SD.open(name_cache);		//读取完成写入的上一个文件
  if (myFile) 						//如果有对应文件
  {
    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      char c=myFile.read();
#ifdef DEBUG
      Serial.write(c);	//串口输出
#endif
    }
    myFile.close();					//结束
  } 
#ifdef DEBUG
  else 								//找不到对应文件
  {
    Serial.println("=== SD error opening test.");
  }
  Serial.println("=== SD READ Complete ===");
  Serial.println(" ");
#endif
}

void vosdwrite(char* name_cache,char choose)		//SD卡文件写入
{
#ifdef DEBUG
  Serial.println(" ");
  Serial.println("=== SD WRITE ===");
#endif

  myFile = SD.open(name_cache, FILE_WRITE);		//打开需要更新文件

  if (myFile) 					//如果有对应文件
  {
    digitalWrite(PIN_LED1,true);		//LED1亮，SD卡开始写入

    switch(choose)			//写入文件头、内容、尾
    {
    case 'H':
      myFile.println("<?xml version=\"1.0\"?>");
      myFile.println("<gpx creator=\"Geoinfor Scientek Consultant Inc.\" version=\"1.1\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\" xmlns=\"http://www.topografix.com/GPX/1/0\" xsi:schemaLocation=\"http://www.topografix.com/GPX/1/0 http://www.topografix.com/GPX/1/0/gpx.xsd\">");
      myFile.println("<time>2013-09-27T13:10:31Z</time>");
      myFile.println("<trk>");
      myFile.println("<trkseg>");

#ifdef DEBUG
      Serial.println("=== SD ~~~choose Head");      
#endif
      break;
    case 'M':
      {
        static char buf1[46];
        static char buf2[20];
        static char buf3[46];
        static char buf4[20];
        static char buf5[20];
        static char buf6[20];
        static char buf7[20];

        {
          //buf1_float-------------
          char c_latitude[20],c_longitude[20];

          dtostrf(f_latitude,4,6,c_latitude);  //将获取的数值转换为字符型数组
          dtostrf(f_longitude,4,6,c_longitude);  //将获取的数值转换为字符型数组

          sprintf(buf1, "<trkpt lat=\"%s\" lon=\"%s\">", c_latitude,c_longitude); 

          //buf2_float-------------
          float s_Height;
          char c_Height[10];

          s_Height=f_Height;

          dtostrf(s_Height,4,1,c_Height);  //将获取的数值转换为字符型数组          

          sprintf(buf2, "<ele>%s</ele>", c_Height); 

          //buf3_int--------------
          sprintf(buf3, "<time>20%d-%d-%dT%d:%d:%dZ</time>", idate[0], idate[1], idate[2],itime[0],itime[1],itime[2]); 

          //buf4_normal--------------
          sprintf(buf4, "<sym>Dot</sym>"); 

          //buf5_int--------------
          sprintf(buf5, "<sat>%d</sat>",i_satellites); 

          //buf6_float--------------
          float s_fixquality;
          char c_fixquality[10];

          s_fixquality=f_fixquality;

          dtostrf(s_fixquality,4,1,c_fixquality);  //将获取的数值转换为字符型数组          

          sprintf(buf6, "<pdop>%s</pdop>",c_fixquality); 

          //buf7_normal--------------
          sprintf(buf7, "</trkpt>"); 
        }

        myFile.println(buf1);
        myFile.println(buf2);
        myFile.println(buf3);
        myFile.println(buf4);
        myFile.println(buf5);
        myFile.println(buf6);
        myFile.println(buf7);

#ifdef DEBUG
        Serial.println("=== SD ~~~choose Main");
#endif
      }
      break;
    case 'B':
      myFile.println("</trkseg>");
      myFile.println("</trk>");
      myFile.println("</gpx>");

#ifdef DEBUG
      Serial.println("=== SD ~~~choose Body");
#endif
      break;
    }

    myFile.close();			//结束

#ifdef DEBUG
    Serial.println("===SD Write DONE===");
#endif
  }
#ifdef DEBUG
  else 			//找不到对应文件
  {
    Serial.println("===SD Write ERROR===");
  }
#endif


  digitalWrite(PIN_LED1,false);		//LED1灭，写入结束

#ifdef DEBUG
  Serial.println("=== SD WRITE Complete ===");
  Serial.println(" ");
#endif
}

void vosd_datawrite()		//常规：GPX数据写入
{
  if(STA)
  {
    //      vostring();
    vosdbegin(file_name);		//SD卡文件初始检测
    if(sd_sta && file_sta)
    {
#ifdef DEBUG
      Serial.println(" ");
      Serial.println("=== SD DATAWRITE === ");
#endif

      vosdwrite(file_name,'M');
      //        vosdread(file_name);    

#ifdef DEBUG
      Serial.print("=== SD The NUM ");
      Serial.print(file_num);
      Serial.print(" File:");
      Serial.print(file_name);
      Serial.print(" is ");
      Serial.print("Updata");
      Serial.println("=== SD DATAWRITE Complete === ");
      Serial.println(" ");
#endif
    }
  }
}

void  vosd_dataupdata()		//更新新的GPX文件
{
  eeprom_read();				//读取EEPROM，获取上个文件日期
  if(idate_cache!=idate[2])		//如果日期变化
  {
    file_num=1;		//文件序号从1计
    idate_cache=idate[2];
  }

  //    Serial.print("idate[2]:");
  //    Serial.println(idate[2]);

  vostring();				//更新文件名
  vosdbegin(file_name);     //SD卡文件初始检测

  if(sd_sta && file_sta)	//SD及其文件正常时
  {
#ifdef DEBUG
    Serial.println(" ");
    Serial.println("== SD updata ==");
#endif

    vosdwrite(file_name,'B');	//写入文件尾
    //    vosdread(file_name);


#ifdef DEBUG
    Serial.print("The NUM ");
    Serial.print(file_num);
    Serial.print(" File:");
    Serial.print(file_name);
    Serial.print(" is ");
    Serial.print("Complete");
#endif

    //-------------------
    file_num++;					//更新文件序号
    if(file_num>99)
      file_num=1;

    vostring();					//更新文件名
    vosdbegin(file_name); 	    //SD卡文件初始检测
    //    vosdread(file_name);

#ifdef DEBUG
    Serial.println(" ");
    Serial.println("== SD updata Complete ==");
#endif
  }

  //  Serial.print("idate_cache:");
  //  Serial.println(idate_cache);

  eeprom_write();  			//写入EEPROM
}


//OLED===================================================
void vooled()
{
  u8g.firstPage();
  do
  {
    setFont_L;

    u8g.setPrintPos(2, 18);

    u8g.print("Speed:");
    if(STA)
    {
      u8g.print(i_Speed[1]);
      setFont_M;
      u8g.print(".");
      u8g.print(i_Speed[0]);
    }
    else
    {
      u8g.print("N/A");
      setFont_S;
      u8g.print("  no fix");
    }

    setFont_M;
    u8g.setPrintPos(2, 32);
    u8g.print("Lat.: ");
    u8g.print( c_lat);
    u8g.print(" ");
    u8g.print( f_latitude,4);

    u8g.setPrintPos(2, 41);
    u8g.print("Lon.: ");
    u8g.print( c_lon);
    u8g.print(" ");
    u8g.print(f_longitude,4);

    u8g.drawLine(0, 44, 128, 44);

    u8g.drawLine(0, 55, 128, 55);

    u8g.setPrintPos(2, 53);
    if(!b_oled_updata)	//按键是否动作
    {
      u8g.print("20");
      u8g.print(idate[0]);
      u8g.print("-");
      u8g.print(idate[1]);
      u8g.print("-");
      u8g.print(idate[2]);

      u8g.print("  ");
      u8g.print(itime[0]);
      u8g.print(":");
      u8g.print(itime[1]);
      u8g.print(":");
      u8g.print(itime[2]);
    }
    else
    {
      u8g.print("Compl.:\"");
      u8g.print(file_name_cache);
      u8g.print("\"");
    }

    //------------------
    u8g.drawFrame(2, 57, 14, 7);
    u8g.drawBox(2, 57, map(_V_bat,_V_min,_V_max,1,14) , 7);
    u8g.drawFrame(16, 59, 2, 3);

    u8g.setPrintPos(24, 64);
    u8g.print("STA :");
    u8g.print(i_satellites);    

    u8g.setPrintPos(72, 64);
    u8g.print("ELE.:");
    u8g.print(int(f_Height));
  }
  while( u8g.nextPage() );
}

void volcdlogo(unsigned int x, unsigned int y)
{
  u8g.firstPage();
  do
  {
    u8g.drawXBMP( x, y, u8g_logo_width, u8g_logo_height, u8g_logo_bits);
  }
  while( u8g.nextPage() );
}


void setup()
{
  Serial.begin(115200);

  GPS.begin(38400);

  pinMode(PIN_key1,INPUT_PULLUP); 		//上拉

  pinMode(PIN_LED1,OUTPUT); 		//LED
  pinMode(PIN_LED2,OUTPUT); 		//LED

  eeprom_read();					//读取EEPROM

  //  vostring();

  Serial.print("file_name: ");  
  Serial.println(file_name);

  //------------------------------
  Serial.println("Initializing SD card...");

  pinMode(10, OUTPUT);	

  if (!SD.begin(7)) 		//初始化SD卡
  {
    sd_sta=false;			//SD卡不正常
    Serial.println("initialization failed!");
    return;
  }
  else
  {
    sd_sta=true;				//SD卡正常
    vosdbegin(file_name);		//SD卡文件初始检测
    Serial.println("initialization done.");
  }

  //  u8g.setRot180();
  volcdlogo(0, 10);
  delay(2000);
}


void loop()
{
  //按键检测-------------------------------
  file_updata=vokey(PIN_key1);
  if(file_updata)
  {
    vosd_dataupdata();
    time_oled_updata=millis();
  }
  b_oled_updata=! boolean (millis()-time_oled_updata>init_oled_updata);		//按键动作时

  //GPS-------------------------------
  vogps_databegin();

  vogps_dataread();


  //SD-------------------------------
  if(time_sdwrite>millis()) time_sdwrite=millis();
  if(millis()-time_sdwrite>init_sdwrite)
  {
    vosd_datawrite();

    time_sdwrite=millis();
  }

  //BAT--------------------------------
  if (time_bat > millis()) time_bat = millis();
  if(millis()-time_bat>init_bat)
  {
    vobat();

    time_bat=millis();
  }

  //OLED-------------------------------
  if (time_oled > millis()) time_oled = millis();
  if(millis()-time_oled>init_oled)
  {
    vooled();

    time_oled=millis();
  }

}





