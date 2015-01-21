#include "key.h"
#include "audio.h"

#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(7, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color[9]=
{
  strip.Color(0, 0, 0), strip.Color(255, 0, 0),strip.Color(255, 165, 0),strip.Color(255, 255, 0),strip.Color(0, 255, 0),strip.Color(0, 127, 255),strip.Color(0, 0, 255),strip.Color(139, 0, 255),strip.Color(255, 255, 255)
  };
  ///==========================
  int sensor_pin[7]=
{
  4,8,10,12,A0,A2,A6
};

int sensor_vol[7]={
  1,2,3,4,5,6,7};
int sensor_vol_cache[7];

//-------------------------
#define music_num_MIN 1		//歌曲最小数，固定
#define music_num_MAX 2	//歌曲最大数，可改

int music_vol=30;  //初始音量0~30


int pin_light[7]=
{
  A6,A5,A4,A3,A2,A1,A0
};

int pin_tone=9;

#define mode 1
int num=0;
int num_cache=0;

boolean sta=false;

boolean led_sta[7]={
  false,false,false,false,false,false,false};

unsigned long time1[7];
unsigned long time2[7];

void setup()
{
  Serial.begin(9600);

  for(int a=0;a<7;a++)
    pinMode(sensor_pin[a],INPUT);
  // There's nothing to set up for this sketch
  // iterate over the notes of the melody:

  key_init();

  strip.begin();	//初始化LED
  strip.show(); // Initialize all pixels to 'off'

  audio_init(DEVICE_TF,MODE_One_END,music_vol);		//初始化mp3模块
  delay(500);

  for(int a=0;a<7;a++)
  {
    colorWipe(color[0], a);
  }

  for(int a=0;a<7;a++)
  {
    colorWipe(color[8], a);
    Serial.println(a);

    audio_choose(a+1);
    audio_play();

    delay(800);

    // colorWipe(color[0], a);
  }

}

void loop() 
{

  for(int a=0;a<7;a++)
  {

    //-----------------
    //    if(!)
    {
      if(key_get(sensor_pin[a],1))		//触发时
      {
        //      colorWipe(strip.Color(255, 255, 0), 10);
        sensor_vol_cache[a]++;	//计数
        // Serial.print("+");
        if(sensor_vol_cache[a]>3)
          sensor_vol_cache[a]=3;

        {
          colorWipe(color[8], a);

          num=a;

          audio_choose(a+1);
          audio_play();

          colorWipe(color[0], a);
        }
        //        led_sta[a]=true;

        //      paper_sta=true;	//读取模式

        time1[a]=millis();
      }

      if(millis()-time1[a]>500 && sensor_vol_cache[a]!=0)	//触发，并且闲置超过一秒，结束扫描
      {
        //       led_sta[a]=false;

        //      audio_choose(paper_num);	//播放扫描歌曲
        //      audio_play();
        if(sensor_vol_cache[a]==1)
        {
          colorWipe(color[0], a);
          Serial.println("LED_OFF");
        }
        else if(sensor_vol_cache[a]==2)
        {
          colorWipe(color[8], a);
        }
        else
        {
          randomSeed(millis()%1000);
          sensor_vol[a]=random(1,7);
          //sensor_vol[a]=sensor_vol_cache[a]-1;
          Serial.println("");
          Serial.print("num:");
          Serial.print(a);
          Serial.print(":");
          Serial.print(sensor_vol[a]);

          colorWipe(color[sensor_vol[a]], a);
        }

        sensor_vol_cache[a]=0;		//归零 等待下次扫描

        //      paper_sta=false;	//读取模式结束
      }
    }
  }

  delay(1);
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t i) {
  // for(uint16_t i=0; i<strip.numPixels(); i++)
  {
    strip.setPixelColor(i, c);
    strip.show();
  }
}































