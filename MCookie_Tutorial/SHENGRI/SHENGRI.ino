#include <Adafruit_NeoPixel.h>

#define PIN  A6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

int zone=0;

int zone_vol[3]=
{
  400,200,100
};

void setup() {
  Serial.begin(9600);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

unsigned long timer[3];
int vol[3];

unsigned long timer_color;
int color;

boolean sta,sta_cache;

void loop() 
{
  int v=analogRead(A0);
  Serial.println(v);
  
  if(millis()-timer_color>50)
  {
    timer_color=millis();
    if(v>400)
      sta=true;
    else
      sta=false;

    if(sta && !sta_cache)
      color++;

    sta_cache=sta;

    if(color>2)
      color=0;
  }

  if(millis()-timer[0]>100)
  {
    timer[0]=millis();
    if(v>zone_vol[0])
      zone=0;
    else if(v>zone_vol[1])
      zone=1;
    else if(v>zone_vol[2])
      zone=2;
    else
      zone=9;
  }

  switch(zone)
  {
  case 0:
    if(color!=0)
      vol[0]=map(v,0,1023,0,255);
    else
      vol[0]=10;
    break;
  case 1:
    if(color!=1)
      vol[1]=map(v,0,1023,0,255);
    else
      vol[1]=10;
    break;
  case 2:
    if(color!=2)
      vol[2]=map(v,0,1023,0,255);
    else
      vol[2]=10;
    break;
  }

  // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(vol[0],vol[1], vol[2]), 20); 
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}
