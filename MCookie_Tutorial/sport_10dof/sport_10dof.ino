#include <Rtc_Pcf8563.h>

#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5); // RX, TX

//#define my_Serial mySerial
#define my_Serial Serial1

Rtc_Pcf8563 rtc;//初始化实时时钟

#include "U8glib.h"

U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE);//定义OLED连接方式

#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

MPU6050 accelgyro;

#define buzzer_pin 6

int16_t ax, ay, az;
int16_t gx, gy, gz;

boolean sport_en, sport_change;

boolean time_en, time_change;

int num;
int data[4];

long time, timer;

byte cmdByte;
String msg = "Microduino";
int _year, _month, _day, _hour, _minute, _sec;
String dateStr, ret;

int hour_c[3], minute_c[3], sec_c[3];

int hour_en[4], minute_en[4], sec_en[4];

int time_last;

void setup() {
  pinMode(buzzer_pin, OUTPUT);

  Wire.begin();

  Serial.begin(115200);
  my_Serial.begin(9600);
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
}

void loop() {
  ret = getRtcTimeString();

  if (rtc.getSecond() % 2 == 0)
  {
    //Serial.println(ret + " -> " + msg);
    my_Serial.print(ret);
  }
  bluetooth();
  // read raw accel/gyro measurements from device
  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  // these methods (and a few others) are also available
  //accelgyro.getAcceleration(&ax, &ay, &az);
  //accelgyro.getRotation(&gx, &gy, &gz);

  // display tab-separated accel/gyro x/y/z values
  ax = ax / 180;
  ay = ay / 180;
  az = az / 180;
  gx = gx / 180;
  gy = gy / 180;
  gz = gz / 180;
  /*
  Serial.print("a/g:\t");
  Serial.print(ax); Serial.print("\t");
  Serial.print(ay); Serial.print("\t");
  Serial.print(az); Serial.print("\t");
  Serial.print(gx); Serial.print("\t");
  Serial.print(gy); Serial.print("\t");
  Serial.println(gz);
  */
  if (gy > 80 && ax > -50)
    sport_en = true;
  else if (gy < -80 && ax < -50)
    sport_en = false;

  if (sport_change != sport_en)
  {
    if (sport_en)
    {
      num += 1;
      tone(buzzer_pin, 300);
      Serial.println(num);
    }
    else
      noTone(buzzer_pin);

    sport_change = sport_en;
  }

  if (num > 0)
    time_en = true;

  if (time_change != time_en)
  {
    if (time_en)
    {
      hour_c[0] = rtc.getHour();
      minute_c[0] = rtc.getMinute();
      sec_c[0] = rtc.getSecond();
    }
    else
    {
      hour_c[1] = rtc.getHour();
      minute_c[1] = rtc.getMinute();
      sec_c[1] = rtc.getSecond();
    }
    time_change = time_en;
  }

  if (gx < 10 && gy < 10 && gz < 10)
  {
    if (millis() - time > 3000)
    {
      if (gx < 10 && gy < 10 && gz < 10)
      {
        time_en = false;
        if (hour_c[1] != 0 && minute_c[1] != 0 && sec_c[1] != 0)
        {
          time_long();
          large_time();
        }
        lagre();
        num = 0;
        noTone(buzzer_pin);
        time = millis();
      }
    }
  }
  else
    time = millis();

  if (millis() - timer > 1000)
  {
    u8g.firstPage();
    do {
      draw();
    } while ( u8g.nextPage() );
    timer = millis();
  }
}

void time_long()
{
  if (hour_c[1] - hour_c[0] >= 0)
    hour_c[2] = hour_c[1] - hour_c[0];
  else
    hour_c[2] = 24 - hour_c[0] + hour_c[1];
  if (hour_c[2] > 0)
    minute_c[2] = 60 - minute_c[0] + minute_c[1];
  else
    minute_c[2] = minute_c[1] - minute_c[0];
  if (minute_c[2] > 0)
    sec_c[2] = 60 - sec_c[0] + sec_c[1];
  else
    sec_c[2] = sec_c[1] - sec_c[0];

  Serial.print(hour_c[0]);
  Serial.print("h");
  Serial.print(minute_c[0]);
  Serial.print("m");
  Serial.print(sec_c[0]);
  Serial.print("s");
  Serial.print("->");
  Serial.print(hour_c[1]);
  Serial.print("h");
  Serial.print(minute_c[1]);
  Serial.print("m");
  Serial.print(sec_c[1]);
  Serial.println("s");

  Serial.print(hour_c[2]);
  Serial.print("h");
  Serial.print(minute_c[2]);
  Serial.print("m");
  Serial.print(sec_c[2]);
  Serial.println("s");
}

void large_time()
{
  if (time_last == 1)
  {
    if (hour_en[2] > 0 || minute_en[2] > 0 || sec_en[2] > 0)
    {
      hour_en[3] = hour_en[2];
      minute_en[3] = minute_en[2];
      sec_en[3] = sec_en[2];
    }
    if (hour_en[1] > 0 || minute_en[1] > 0 || sec_en[1] > 0)
    {
      hour_en[2] = hour_en[1];
      minute_en[2] = minute_en[1];
      sec_en[2] = sec_en[1];
    }
    if (hour_en[0] > 0 || minute_en[0] > 0 || sec_en[0] > 0)
    {
      hour_en[1] = hour_en[0];
      minute_en[1] = minute_en[0];
      sec_en[1] = sec_en[0];
    }
    hour_en[0] = hour_c[2];
    minute_en[0] = minute_c[2];
    sec_en[0] = sec_c[2];
  }
  else if (time_last == 2)
  {
    if (hour_en[2] > 0 || minute_en[2] > 0 || sec_en[2] > 0)
    {
      hour_en[3] = hour_en[2];
      minute_en[3] = minute_en[2];
      sec_en[3] = sec_en[2];
    }
    if (hour_en[1] > 0 || minute_en[1] > 0 || sec_en[1] > 0)
    {
      hour_en[2] = hour_en[1];
      minute_en[2] = minute_en[1];
      sec_en[2] = sec_en[1];
    }
    hour_en[1] = hour_c[2];
    minute_en[1] = minute_c[2];
    sec_en[1] = sec_c[2];
  }

  else if (time_last == 3)
  {
    if (hour_en[2] > 0 || minute_en[2] > 0 || sec_en[2] > 0)
    {
      hour_en[3] = hour_en[2];
      minute_en[3] = minute_en[2];
      sec_en[3] = sec_en[2];
    }
    hour_en[2] = hour_c[2];
    minute_en[2] = minute_c[2];
    sec_en[2] = sec_c[2];
  }
  if (time_last == 4)
  {
    hour_en[3] = hour_c[2];
    minute_en[3] = minute_c[2];
    sec_en[3] = sec_c[2];
  }
  time_last = 0;
}
void lagre()
{
  if (num > data[0] && num > data[1] && num > data[2] && num > data[3] )
  {
    if (data[2] > 0)
      data[3] = data[2];
    if (data[1] > 0)
      data[2] = data[1];
    if (data[0] > 0)
      data[1] = data[0];

    data[0] = num;
    time_last = 1;
  }
  else if (num < data[0] && num > data[1] && num > data[2] && num > data[3])
  {
    if (data[2] > 0)
      data[3] = data[2];
    if (data[1] > 0)
      data[2] = data[1];
    data[1] = num;
    time_last = 2;
  }
  else if (num < data[1] && num > data[2] && num > data[3])
  {
    if (data[2] > 0)
      data[3] = data[2];
    data[2] = num;
    time_last = 3;
  }
  else if (num < data[2] && num > data[3])
  {
    data[3] = num;
    time_last = 4;
  }
}

void draw() {
  for (int i; i < 4; i++)
  {
    u8g.setFont(u8g_font_7x13);
    u8g.setPrintPos(0, 12 * (i + 1));
    u8g.print(i + 1);
    u8g.print(": ");
    u8g.print(data[i]);
    u8g.setFont(u8g_font_6x13);
    u8g.setPrintPos(44, 12 * (i + 1));
    u8g.print("Time:");
    u8g.print(hour_en[i]);
    u8g.print("h");
    u8g.print(minute_en[i]);
    u8g.print("m");
    u8g.print(sec_en[i]);
    u8g.print("s");
  }
  u8g.setPrintPos(0, 64);
  u8g.setFont(u8g_font_6x13);
  u8g.print(rtc.formatDate(RTCC_DATE_ASIA));
  u8g.print("  ");
  u8g.print(rtc.formatTime(RTCC_TIME_HMS));
}

void bluetooth() {
  if (my_Serial.available() > 0) {
    cmdByte = my_Serial.read();

    switch (cmdByte) {
      case 't' :
        _year = my_Serial.parseInt();
        _month = my_Serial.parseInt();
        _day = my_Serial.parseInt();
        _hour = my_Serial.parseInt();
        _minute = my_Serial.parseInt();
        _sec = my_Serial.parseInt();
        setRtcTime(_year, _month, _day, _hour, _minute, _sec);
        break;
      case 'm' :
        msg = my_Serial.readStringUntil('\n');
        break;
      default:
        while (my_Serial.available() > 0) {
          mySerial.read();
        }
        break;
    }
  }
}

void setRtcTime (byte _year, byte _month, byte _day, byte _hour, byte _minute, byte _sec)
{
  //clear out all the registers
  rtc.initClock();
  rtc.setDate(_day, 0, _month, 0, _year);
  rtc.setTime(_hour, _minute, _sec);
}

String getRtcTimeString() {
  dateStr = rtc.formatDate(RTCC_DATE_ASIA);
  dateStr += " ";
  dateStr += rtc.formatTime(RTCC_TIME_HMS);

  return dateStr;
}
