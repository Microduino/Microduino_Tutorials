const int TrigPin = 3;
const int EchoPin = 2;
float cm;

void setup()
{
  Serial.begin(9600);
  pinMode(TrigPin, OUTPUT);
  pinMode(EchoPin, INPUT);
}
void loop()
{
  digitalWrite(TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(TrigPin, LOW);

  cm = pulseIn(EchoPin, HIGH) / 58.0; //Echo time will be converted into cm
  cm = (int(cm * 100.0)) / 100.0; //Two decimal places
  Serial.println(cm);
  delay(100);
}

