/*
 Musical Notes
 Constant Name	Frequency (Hz)
 NOTE_B2	 123
 NOTE_C3	 131
 NOTE_CS3	 139
 NOTE_D3	 147
 NOTE_DS3	 156
 NOTE_E3	 165
 NOTE_F3	 175
 NOTE_FS3	 185
 NOTE_G3	 196
 NOTE_GS3	 208
 NOTE_A3	 220
 NOTE_AS3	 233
 NOTE_B3	 247
 NOTE_C4	 262
 NOTE_CS4	 277
 NOTE_D4	 294
 NOTE_DS4	 311
 NOTE_E4	 330
 NOTE_F4	 349
 NOTE_FS4	 370
 NOTE_G4	 392
 NOTE_GS4	 415
 NOTE_A4	 440
 NOTE_AS4	 466
 NOTE_B4	 494
 NOTE_C5	 523
 NOTE_CS5	 554
 NOTE_D5	 587
 NOTE_DS5	 622
 NOTE_E5	 659
 NOTE_F5	 698
 NOTE_FS5	 740
 NOTE_G5	 784
 NOTE_GS5	 831
 NOTE_A5	 880
 NOTE_AS5	 932
 NOTE_B5	 988
 NOTE_C6	 1047
 NOTE_CS6	 1109
 NOTE_D6	 1175
 NOTE_DS6	 1245
 NOTE_E6	 1319
 NOTE_F6	 1397
 NOTE_FS6	 1480
 NOTE_G6	 1568
 NOTE_GS6	 1661
 NOTE_A6	 1760
 NOTE_AS6	 1865
 NOTE_B6	 1976
 NOTE_C7	 2093
 NOTE_CS7	 2217
 NOTE_D7	 2349
 NOTE_DS7	 2489
 NOTE_E7	 2637
 NOTE_F7	 2794
 NOTE_FS7	 2960
 NOTE_G7	 3136
 NOTE_GS7	 3322
 NOTE_A7	 3520
 NOTE_AS7	 3729
 NOTE_B7	 3951
 NOTE_C8	 4186
 NOTE_CS8	 4435
 NOTE_D8	   4699
 NOTE_DS8	 4978
 
 */

#include <Tone.h>
const int TrigPin = 3;
const int EchoPin = 2;
const int speakerPin =9 ;

Tone toneVoice;
int toneFrequency[]={415,440,466,494,523,554,587,622};
int duration=100;
int cm;

void setup()
{
  // Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
  pinMode(speakerPin,OUTPUT);
  toneVoice.begin(speakerPin);
}
void loop()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58; //Echo time will be converted into cm

  toneVoice.play(toneFrequency[cm/2], duration);
  delay(duration);//wait for the tone to finish

  // Serial.println(cm);
}




