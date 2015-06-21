#define buzzer_pin 8


const int vibrationPin = 6;     // the number of the pushbutton pin

int vibrationState = 0;

void setup() {
  Serial.begin(9600);
  pinMode(vibrationPin, INPUT);
  pinMode(buzzer_pin,OUTPUT);
}

void loop() {
  vibrationState = digitalRead(vibrationPin);
  if (vibrationState == HIGH) {
    Serial.println("ONONONON");
    tone(buzzer_pin,500); 
  } else {
    Serial.println("---");
    noTone(buzzer_pin); 
  }
}