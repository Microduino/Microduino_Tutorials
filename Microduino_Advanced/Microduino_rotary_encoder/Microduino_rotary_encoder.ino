const int encoderPinA = 4;
const int encoderPinB = 5;

int encoderPos;

boolean encoderALast = LOW;  // remembers the previous pin state

void setup()
{
  pinMode(encoderPinA,  INPUT_PULLUP);
  pinMode(encoderPinB,  INPUT_PULLUP);
  pinMode(A0,  INPUT_PULLUP);
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);
  Serial.begin (9600);
}

void loop()
{
  int sensorValue = analogRead(A0);

  boolean encoderA = digitalRead(encoderPinA);
  if ((encoderALast == HIGH) && (encoderA == LOW))
  {
    if (digitalRead(encoderPinB) == LOW)
    {
      encoderPos--;
    }
    else
    {
      encoderPos++;
    }
  }
  encoderALast = encoderA;
  Serial.print(sensorValue);
  Serial.print("        ");
  Serial.println(encoderPos);
}


