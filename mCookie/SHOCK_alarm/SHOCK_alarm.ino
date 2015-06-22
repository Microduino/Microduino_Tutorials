#define pushButton 4
#define speakPin 6

int pushState;

long time, timer;

boolean shock;

// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  // make the pushbutton's pin an input:
  pinMode(pushButton, INPUT);
  pinMode(speakPin, OUTPUT);
}

void loop() {
  if (digitalRead(pushButton))
    shock = true;

  if (millis() - time > 2000)
  {
    if (!digitalRead(pushButton))
      shock = false;
    time = millis();
  }

  //  Serial.println(shock);
  if (shock)
  {
    for (int j = 0; j < 2; j++)
      buzzer();
  }
  else
    noTone(speakPin);
}

void buzzer()
{
  for (int i = 200; i <= 800; i++)
  {
    tone(speakPin, i);
    delay(5);
  }
  delay(1000);
  for (int i = 800; i >= 200; i--)
  {
    tone(speakPin, i);
    delay(10);
  }
}
