#include <Adafruit_NeoPixel.h>

#include "key.h"

#define PIXEL_PIN    A0    // Digital IO pin connected to the NeoPixels.
#define PIXEL_COUNT  6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978

int key_Pin = 2;
int speakerPin = 6;
//Tone makeTone;

boolean play_pause;

long timer;
int val, add;

#define val_max 255
#define val_min 0

// notes to play; see Tone.h for frequencies;
int notes[] = {
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_F4, NOTE_E4,
  NOTE_C4, NOTE_C4, NOTE_D4, NOTE_C4, NOTE_G4, NOTE_F4,
  NOTE_C4, NOTE_C4, NOTE_C5, NOTE_A4, NOTE_F4, NOTE_E4, NOTE_D4,
  NOTE_AS4, NOTE_AS4, NOTE_A4, NOTE_F4, NOTE_G4, NOTE_F4
};

// number of beats for each note
int beats[] = {
  1, 1, 2, 2, 2, 4,
  1, 1, 2, 2, 2, 4,
  1, 1, 2, 2, 2, 2, 2,
  1, 1, 2, 2, 2, 4
};

// Calculate song length
int songLength = sizeof(notes) / sizeof(int);
int tempo = 220; // in milliseconds

void playNote(int note, int beat) {
  noTone(speakerPin); // speaker reset
  tone(speakerPin, note); // play tone
  delay(tempo * beat); // for specified number of beats
  noTone(speakerPin); // speaker reset
  delay(tempo / 4); // pause between notes
}

void setup() {
  Serial.begin(115200);
  pinMode(key_Pin, INPUT_PULLUP);
  attachInterrupt(4, blink, FALLING);
  strip.begin();
  strip.show();
}

void loop() {
  if (play_pause)
  {
    for (int i = 0; i < songLength; i++) {
      add++;
      if (add == 5)
        add = 1;
      if (add == 1)
        colorSet(strip.Color(i * 10, 0, 0));
      else if (add == 2)
        colorSet(strip.Color(0, i * 10, 0));
      else if (add == 3)
        colorSet(strip.Color(0, 0, i * 10));
      else if (add == 4)
        colorSet(strip.Color(i * 10, i * 10, 0));
      if (!play_pause)
      {
        play_pause = false;
        noTone(speakerPin);
        return;
      }
      playNote(notes[i], beats[i]); // make sound
    }
  }
  else
    colorSet(0);
}

void blink()
{
  delay(300);
  play_pause = !play_pause;
}

void colorSet(uint32_t c) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
}
