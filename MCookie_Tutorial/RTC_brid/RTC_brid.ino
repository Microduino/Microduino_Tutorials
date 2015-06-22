#include <Wire.h>
#include <Rtc_Pcf8563.h>

#include <SoftwareSerial.h>

#include <Adafruit_NeoPixel.h>

Rtc_Pcf8563 rtc;//初始化实时时钟

#include "audio.h"

#define Light_PIN A2
#define Light_val  200

#define PIN A0
#define PIXEL_COUNT  6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIN, NEO_GRB + NEO_KHZ800);

byte cmdByte;
String msg = "Microduino";
int _year, _month, _day, _hour, _minute, _sec;
String dateStr, ret;

int i, j, k;

long time1;

boolean voice_en;

void setup()
{
  Serial.begin(115200);
  my_Serial.begin(9600);
  pinMode(Light_PIN, INPUT);
  strip.begin();
  strip.show();
  audio_init(DEVICE_Flash, MODE_One_END, music_vol);
  setRtcTime(15, 6, 18, 15, 59, 55);
}

void loop() {
  ret = getRtcTimeString();

  if (rtc.getSecond() % 2 == 0)
  {
    Serial.println(ret + " -> " + msg);
    Serial.println(analogRead(Light_PIN));
    my_Serial.print(ret);
  }

  bluetooth();

  if (rtc.getMinute() == 0 && rtc.getSecond() == 0)
  {
    delay(1000);
    audio_play();
  }
  
  if (analogRead(Light_PIN) < Light_val)
    rainbow(30);
  else
    colorWipe(0);
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

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void colorSet(uint32_t c, uint8_t i) {
  strip.setPixelColor(i, c);
  strip.show();
}

void rainbow(uint8_t wait) {

  if (millis() - time1 > wait)
  {
    j++;
    if (j > 255)
      j = 0;
    time1 = millis();
  }
  if (k != j)
  {
    k = j;
    for (i = 0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i + j) & 255));
    }
    strip.show();
  }
}

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
