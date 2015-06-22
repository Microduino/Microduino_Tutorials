#include "key.h"

#define voice 400

#define mic_pin A0
#define buzzer_pin 6
#define key_pin 4

int voice_data;
long time, timer;

boolean buzzer_speak;

boolean add;
int i = 200;

void setup() {
  Serial.begin(115200);
  pinMode(mic_pin, INPUT);
  pinMode(key_pin, INPUT);
  pinMode(buzzer_pin, OUTPUT);

  key_init();
}

void loop() {

  voice_data = analogRead(mic_pin);
  Serial.println(voice_data);
  if (voice_data > voice)
  {
    if (millis() - time > 1000 )
    {
      voice_data = analogRead(mic_pin);
      if (voice_data > voice)
      {
        buzzer_speak = true;
        i = 200;
      }
      time = millis();
    }
  }
  else
  time = millis();
  if (key_get(key_pin, 0))
  {
    delay(200);
    buzzer_speak = false;
    time = millis();
  }
  if (buzzer_speak)
    buzzer();
  else
    noTone(buzzer_pin);
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
