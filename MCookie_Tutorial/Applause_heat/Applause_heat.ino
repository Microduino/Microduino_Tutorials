#include <Adafruit_NeoPixel.h>

#define PIN 4

Adafruit_NeoPixel strip = Adafruit_NeoPixel(6, PIN, NEO_GRB + NEO_KHZ800);


#define mic_pin A0
#define buzzer_pin 6

#define voice 400

int voice_data;

int num = 10;

long time,timer;

boolean add;
int i = 200;

void setup() {
  Serial.begin(115200);
  pinMode(mic_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  strip.begin();
  strip.show();
}

void loop() {
  voice_data = analogRead(mic_pin);
  //Serial.println(voice_data);
  if (voice_data > voice)
  {
    num++;
    if (num > 255)
    {
      num = 255;
      buzzer();
    }
    colorWipe(strip.Color(num, 0, 0));
    //delay(10);
    Serial.println(num);
    time = millis();
  }

  if (millis() - time > 3000 )
  {
    voice_data = analogRead(mic_pin);
    if (voice_data < voice)
    {
      colorWipe(strip.Color(0, 0, 0));
      num = 10;
      noTone(buzzer_pin);
    }
    time = millis();
  }
}

void colorWipe(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
  }
}

void buzzer()
{
  if (millis() - timer > 10)
  {
    if (!add)
    {
      i++;
      if (i >= 800)
        add = true;
    }
    else
    {
      i--;
      if (i <= 200)
        add = false;
    }
    tone(buzzer_pin, i);
    timer = millis();
  }
}
